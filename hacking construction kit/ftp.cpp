/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include <cstdio>
#include <exception>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Wininet.h>
#pragma comment(lib, "Wininet.lib")

#include "macro.h"
#include "print.h"
#include "wsa_init.h"
#include "ftp.h"

#include "memory_debug.h"

static WsaInit init;

Ftp::Ftp(TCHAR *url, int port, TCHAR *user, TCHAR *password): m_h(0), m_handle(0){

	HINTERNET m_h = InternetOpen(TEXT("mozilla"), INTERNET_OPEN_TYPE_DIRECT, 0, 0, 0);
	if (m_h == 0) {
		int error = GetLastError();
		MYPRINTF("error %d\n", error);
		throw std::exception("Unable to open handle");
	}

	DWORD_PTR context = 1;
	m_handle = InternetConnect(m_h, url, port, user, password, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, context);
	if (m_handle == 0) {
		int error = GetLastError();
		MYPRINTF("error InternetConnect: %d\n", error);
		Sleep(10000);
		m_handle = InternetConnect(m_h, url, port, user, password, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, context);
		if (m_handle == 0) {
			int error = GetLastError();
			MYPRINTF("error InternetConnect: %d\n", error);
			InternetCloseHandle(m_h);
			throw std::exception("Unable to open ftp handle");
		}
	}
}

Ftp::~Ftp(){
	if (m_h != 0) {
		if (InternetCloseHandle(m_h) == FALSE){
			int error = GetLastError();
			MYPRINTF("error InternetCloseHandle: %d\n", error);
		}
	}
	if (m_handle != 0) {
		if (InternetCloseHandle(m_handle) == FALSE){
			int error = GetLastError();
			MYPRINTF("error InternetCloseHandle: %d\n", error);
		}
	}
}

std::tstring Ftp::getCurrentDirectory(){
	TCHAR buffer[MAX_PATH];
	DWORD size = MAX_PATH;

	if (FtpGetCurrentDirectory(m_handle, buffer, &size) == FALSE){
		int error = GetLastError();
		MYPRINTF("error FtpGetCurrentDirectory: %d\n", error);
		throw std::exception("Unable to get current directory");
	}

	//A std:string  using the char* constructor.
	std::tstring ss(buffer);
	return ss;
}

void Ftp::setCurrentDirectory(std::tstring directory){

	if (FtpSetCurrentDirectory(m_handle, directory.c_str()) == FALSE){
		int error = GetLastError();
		MYPRINTF("error FtpSetCurrentDirectory: %d\n", error);
		throw std::exception("Unable to set current directory");
	}
}

void Ftp::createDirectory(std::tstring directory){

	if (FtpCreateDirectory(m_handle,  directory.c_str()) == FALSE){
		int error = GetLastError();
		MYPRINTF("error FtpCreateDirectory: %d\n", error);
		throw std::exception("Unable to create current directory");
	}
}

void Ftp::uploadFile(std::tstring local, std::tstring remote){

	DWORD_PTR context = 1;

	if (FtpPutFile(m_handle, local.c_str(), remote.c_str(), FTP_TRANSFER_TYPE_BINARY, context) == FALSE){
		int error = GetLastError();
		MYPRINTF("local file: %s, remote file %s\n", local.c_str(), remote.c_str());
		MYPRINTF("error FtpPutFile: %d\n", error);
		throw std::exception("Unable to upload file");
	}
}