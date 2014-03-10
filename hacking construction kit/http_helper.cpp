/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "http_helper.h"

#include <cstdio>
#include <stdlib.h>

#include <Windows.h>

#include <WinInet.h>

#undef BOOLAPI
#undef SECURITY_FLAG_IGNORE_CERT_DATE_INVALID
#undef SECURITY_FLAG_IGNORE_CERT_CN_INVALID

#define URL_COMPONENTS URL_COMPONENTS_ANOTHER
#define URL_COMPONENTSA URL_COMPONENTSA_ANOTHER
#define URL_COMPONENTSW URL_COMPONENTSW_ANOTHER

#define LPURL_COMPONENTS LPURL_COMPONENTS_ANOTHER
#define LPURL_COMPONENTSA LPURL_COMPONENTS_ANOTHER
#define LPURL_COMPONENTSW LPURL_COMPONENTS_ANOTHER

#define INTERNET_SCHEME INTERNET_SCHEME_ANOTHER
#define LPINTERNET_SCHEME LPINTERNET_SCHEME_ANOTHER

#define HTTP_VERSION_INFO HTTP_VERSION_INFO_ANOTHER
#define LPHTTP_VERSION_INFO LPHTTP_VERSION_INFO_ANOTHER

#include <winhttp.h>

#undef URL_COMPONENTS
#undef URL_COMPONENTSA
#undef URL_COMPONENTSW

#undef LPURL_COMPONENTS
#undef LPURL_COMPONENTSA
#undef LPURL_COMPONENTSW

#undef INTERNET_SCHEME
#undef LPINTERNET_SCHEME

#undef HTTP_VERSION_INFO
#undef LPHTTP_VERSION_INFO

#pragma comment(lib, "Winhttp.lib")
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")

#include <TCHAR.H>

#include "print.h"
#include "macro.h"

#include "memory_debug.h"

#define BUFFSIZE 1024
#define AGENT "Mozilla/5.0(WindowsNT6.2;WOW64;rv:21.0)Gecko/20100101Firefox/21.0"
#define CONTEXT_HEADER "Content-Type:multipart/form-data;boundary=---------------------------175382550117383"
#define HEADER_END "\r\n-----------------------------175382550117383--"
#define HEADER_TEMPLATE "-----------------------------175382550117383\r\nContent-Disposition:form-data;name=\"upload\";filename=\"%s\"\r\nContent-Type:application/octet-stream\r\n\r\n"

#define MAX_LINE_READ 1000

LPSTR* HttpHelper::get(char *uri){
	return callGet(uri, 0);
}

void HttpHelper::download(const char *uri, const char *filename) {
	callGet(uri, filename);
}

bool HttpHelper::parseUri(const char *uri, char **url, char **resource){


	const char * pch;
	pch = strchr(uri,'/');
	if (pch == NULL){
		*url = new char[strlen(uri) + 1];// 1 for \0 + 1 for index
		strncpy(*url, uri, strlen(uri) + 1);
		*resource = new char[2];
		strncpy(*resource, "/", 2);
		return true;
	}
	int found = pch - uri;
	int urlSize = found + 1;
	*url = new char[urlSize];// 1 for \0 + 1 for index

	strncpy(*url, uri, urlSize - 1);
	(*url)[urlSize - 1] = '\0';

	int resourceSize = strlen(uri) + 1 - (found);
	*resource = new char[resourceSize];
	strncpy(*resource, pch, resourceSize);

	return true;
}

LPSTR* HttpHelper::callGet(const char *uri, const char *filename) {
	BOOL result = false;
	HINTERNET  hSession = 0, hConnect = 0, hRequest = 0;	
	LPSTR* linesRead = 0;

	if (InternetAttemptConnect(0) != ERROR_SUCCESS){
		MYPRINTF("InternetAttemptConnect failed (%d)\n", GetLastError());
		return 0;
	}

	char *url = 0;
	char *resource = 0;

	if (!parseUri(uri, &url, &resource)){
		return 0;
	}

	TCHAR uriEncoded[BUFFSIZE];
	DWORD size = BUFFSIZE;
	if (InternetCanonicalizeUrl(resource, uriEncoded, &size, 0) == false){
		MYPRINTF("InternetCanonicalizeUrlA failed (%d)\n", GetLastError());
		delete[] url;
		delete[] resource;
		return 0;
	}

	int wchars_num =  MultiByteToWideChar( CP_OEMCP , 0 , url , -1, NULL , 0 );
	WCHAR* urlWide = new WCHAR[wchars_num];
	MultiByteToWideChar( CP_OEMCP , 0 , url , -1, urlWide , wchars_num );

	wchars_num =  MultiByteToWideChar( CP_OEMCP , 0 , uriEncoded , -1, NULL , 0 );
	WCHAR* resourceWide = new WCHAR[wchars_num];
	MultiByteToWideChar( CP_OEMCP , 0 , resource , -1, resourceWide , wchars_num );

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"WinHTTP Example/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if(hSession) {
		hConnect = WinHttpConnect(hSession, urlWide, INTERNET_DEFAULT_HTTP_PORT, 0);
	}

	// Create an HTTP request handle.
	if(hConnect) {
		hRequest = WinHttpOpenRequest(hConnect, L"GET", resourceWide, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
	}

	// Send a request.
	if(hRequest) {
		result = WinHttpSendRequest( hRequest,	WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	}

	// End the request.
	if(result) {
		result = WinHttpReceiveResponse(hRequest, NULL);
	}

	// Keep checking for data until there is nothing left.
	if( result ) {
		if (filename == 0){
			linesRead = parseResponse(hRequest);
		}else{
			writeToFile(hRequest, filename);
		}
	}

	// Report any errors.
	if(!result) {
		MYPRINTF("Error %d has occurred.\n", GetLastError());
	}

	// Close any open handles.
	if(hRequest) WinHttpCloseHandle(hRequest);
	if(hConnect) WinHttpCloseHandle(hConnect);
	if(hSession) WinHttpCloseHandle(hSession);

	delete[] urlWide;
	delete[] resourceWide;
	delete[] url;
	delete[] resource;

	return linesRead;
}

LPSTR* HttpHelper::parseResponse(HINTERNET hRequest){
	DWORD size;
	LPSTR outBuffer;
	DWORD downloaded = 0;
	LPSTR* linesRead = new LPSTR[MAX_LINE_READ + 1];
	ZeroMemory(linesRead, sizeof(linesRead) * (MAX_LINE_READ + 1));
	int i = 0;
	do {
		// Check for available data.
		size = 0;
		if(!WinHttpQueryDataAvailable(hRequest, &size)) {
			MYPRINTF("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
		}

		// Allocate space for the buffer.
		outBuffer = new char[size+1];
		if(!outBuffer) {
			MYPRINTF( "Out of memory\n" );
			size = 0;
		} else {
			// Read the data.
			ZeroMemory(outBuffer, size + 1);

			if( !WinHttpReadData(hRequest, (LPVOID)outBuffer, size, &downloaded)) {
				MYPRINTF("Error %u in WinHttpReadData.\n", GetLastError());
			}
			char* context	= NULL;
			char  delims[]	= "\n";

			// During the first read, establish the char string and get the first token.
			char* token = strtok_s(outBuffer, delims, &context);

			// While there are any tokens left in "char_line"...
			while (token != NULL && i < MAX_LINE_READ) {
				char *str = new char[strlen(token) + 1];
				strcpy_s(str, strlen(token) + 1, token);
				if (linesRead == 0){
					linesRead = new LPSTR[MAX_LINE_READ + 1];
				}
				linesRead[i] = str;
				i++;
				// NOTE: NULL, function just re-uses the context after the first read.
				token = strtok_s(NULL, delims, &context); 
			}
			delete [] outBuffer;
		}
	} while(size > 0 && i < MAX_LINE_READ);
	return linesRead;
}

bool HttpHelper::writeToFile(HINTERNET hRequest, const char *filename){
	DWORD size;
	LPSTR outBuffer;
	DWORD downloaded = 0;
	FILE * pFile;
	errno_t err;	

	err  = fopen_s (&pFile, filename, "wb");
	if(err != 0) {
		MYPRINTF( "Can't open file %s!\n", filename);
		return false;
	}
	do {
		// Check for available data.
		size = 0;
		if(!WinHttpQueryDataAvailable(hRequest, &size)) {
			MYPRINTF("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
		}

		// Allocate space for the buffer.
		outBuffer = new char[size+1];
		if(!outBuffer) {
			MYPRINTF( "Out of memory\n" );
			size = 0;
		}
		else {
			// Read the data.
			ZeroMemory(outBuffer, size + 1);

			if( !WinHttpReadData(hRequest, (LPVOID)outBuffer, size, &downloaded)) {
				MYPRINTF("Error %u in WinHttpReadData.\n", GetLastError());
			}
			fwrite(outBuffer, 1, size, pFile);

			delete [] outBuffer;
		}
	} while(size > 0);
	fclose(pFile);
	return true;
}

bool HttpHelper::readInternetFile(HINTERNET hRequest, char **response, int &responseSize){

	char pcBuffer[BUFFSIZE];
	DWORD dwBytesRead = 0;
	int i = 1;
	*response = 0;
	responseSize = 0;

	do {
		dwBytesRead = 0;
		i++;
		if (InternetReadFile(hRequest, pcBuffer, BUFFSIZE - 1, &dwBytesRead)) {
			if (dwBytesRead != 0){
				pcBuffer[dwBytesRead] = 0x00; // Null-terminate buffer
				*response = (char *)realloc(*response, BUFFSIZE * i);
				memcpy(*response + responseSize, pcBuffer, dwBytesRead);
				MYPRINTF("%s\n", pcBuffer);
			}
		} else {
			MYPRINTF("\nInternetReadFile failed %d\n", GetLastError());
		}
		responseSize = responseSize + dwBytesRead;
	} while(dwBytesRead > 0);
	return true;
}


bool HttpHelper::sendGet(const TCHAR *server, const TCHAR *uri, char **response, int &responseSize){

	HINTERNET   hSession, hConnection, hRequest;
	TCHAR uriEncoded[BUFFSIZE];
	DWORD size = BUFFSIZE;

	if (InternetAttemptConnect(0) != ERROR_SUCCESS){
		MYPRINTF("InternetAttemptConnect failed (%d)\n", GetLastError());
		return 0;
	}

	if (InternetCanonicalizeUrl(uri, uriEncoded, &size, ICU_ENCODE_SPACES_ONLY) == false){
		MYPRINTF("InternetCanonicalizeUrlA failed (%d)\n", GetLastError());
		return FALSE;
	}

	hSession = InternetOpen( TEXT(AGENT), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );

	if( hSession == NULL ) {
		MYPRINTF("HTTP Open failed\n");
		return FALSE;
	}

	hConnection = InternetConnect( hSession, server,
		INTERNET_DEFAULT_HTTP_PORT, NULL, NULL,//
		INTERNET_SERVICE_HTTP, 0, 0 );
	if( hConnection == NULL ) {
		MYPRINTF("HTTP Connection failed (%d)\n", GetLastError());
		InternetCloseHandle( hSession );
		return FALSE;
	}

	hRequest = HttpOpenRequest( hConnection, TEXT("GET"), uriEncoded,
		HTTP_VERSION, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION &&
		INTERNET_FLAG_NO_CACHE_WRITE, 0 );
	if( hRequest == NULL ) {
		MYPRINTF("HTTP OpenRequest(POST) failed (%d)\n", GetLastError());
		InternetCloseHandle( hConnection );
		InternetCloseHandle( hSession );
		return FALSE;
	}

	static TCHAR hdrs[] = _T("Content-Type: application/x-www-form-urlencoded");

	if (HttpSendRequest(hRequest, hdrs, _tcslen(hdrs), 0, 0) == false){
		MYPRINTF("HTTP HttpSendRequest(POST) failed (%d)\n", GetLastError());
		InternetCloseHandle(hRequest);
		InternetCloseHandle( hConnection );
		InternetCloseHandle( hSession );
		return FALSE;
	}
	
	readInternetFile(hRequest, response, responseSize);
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hSession);

	return true;
}

bool HttpHelper::sendPost(const TCHAR *server, const TCHAR *uri, const char *post, char **response, int &responseSize){

	HINTERNET   hSession, hConnection, hRequest;
	char postEncoded[BUFFSIZE];
	TCHAR uriEncoded[BUFFSIZE];
	DWORD size = BUFFSIZE;

	if (InternetAttemptConnect(0) != ERROR_SUCCESS){
		MYPRINTF("InternetAttemptConnect failed (%d)\n", GetLastError());
		return 0;
	}

	if (InternetCanonicalizeUrl(uri, uriEncoded, &size, ICU_ENCODE_SPACES_ONLY) == false){
		MYPRINTF("InternetCanonicalizeUrlA failed (%d)\n", GetLastError());
		return FALSE;
	}

	if (InternetCanonicalizeUrlA(post, postEncoded, &size, ICU_ENCODE_SPACES_ONLY) == false){
		MYPRINTF("InternetCanonicalizeUrlA failed (%d)\n", GetLastError());
		return FALSE;
	}

	hSession = InternetOpen( TEXT(AGENT), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
	if( hSession == NULL ) {
		MYPRINTF("HTTP Open failed\n");
		return FALSE;
	}

	hConnection = InternetConnect( hSession, server,
		INTERNET_DEFAULT_HTTP_PORT, NULL, NULL,//
		INTERNET_SERVICE_HTTP, 0, 0 );
	if( hConnection == NULL ) {
		MYPRINTF("HTTP Connection failed (%d)\n", GetLastError());
		InternetCloseHandle( hSession );
		return FALSE;
	}

	hRequest = HttpOpenRequest( hConnection, TEXT("POST"), uriEncoded,
		HTTP_VERSION, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION &&
		INTERNET_FLAG_NO_CACHE_WRITE, 0 );
	if( hRequest == NULL ) {
		MYPRINTF("HTTP OpenRequest(POST) failed (%d)\n", GetLastError());
		InternetCloseHandle( hConnection );
		InternetCloseHandle( hSession );
		return FALSE;
	}

	static TCHAR hdrs[] = _T("Content-Type: application/x-www-form-urlencoded");

	if (HttpSendRequest(hRequest, hdrs, _tcslen(hdrs), postEncoded, strlen(postEncoded)) == false){
		MYPRINTF("HTTP HttpSendRequest(POST) failed (%d)\n", GetLastError());
		InternetCloseHandle(hRequest);
		InternetCloseHandle( hConnection );
		InternetCloseHandle( hSession );
		return FALSE;
	}

	readInternetFile(hRequest, response, responseSize);

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hSession);
	return true;
}

bool HttpHelper::createHeader(INTERNET_BUFFERS &internetBuffers, int size, const char *filename){

	LPCTSTR header = TEXT(CONTEXT_HEADER);
	char *dataHeaderTemplate = HEADER_TEMPLATE;
	char dataHeader[BUFFSIZE];
	sprintf_s(dataHeader, BUFFSIZE, dataHeaderTemplate, filename);
	
	BYTE *dataEnd = (BYTE *)HEADER_END;

	ZeroMemory(&internetBuffers, sizeof(INTERNET_BUFFERS));
	internetBuffers.dwStructSize = sizeof(INTERNET_BUFFERS);
	internetBuffers.lpcszHeader = header;
	internetBuffers.dwHeadersLength = _tcslen(header);
	internetBuffers.lpvBuffer = NULL;
	internetBuffers.dwBufferLength = 0;
	internetBuffers.dwBufferTotal = size + strlen((char *)dataHeader) + strlen((char *)dataEnd); // content-length of data to post

	return true;
}

bool HttpHelper::uploadFile(const TCHAR *uri, const TCHAR *localFile, const char *remoteFile, char **response, int &responseSize){
	char *url = 0;
	char *resource = 0;

	if (!parseUri(uri, &url, &resource)){
		return false;
	}
	bool res = uploadFile(url, resource, localFile, remoteFile, response, responseSize);

	delete[] url;
	delete[] resource;

	return res;
}

/**
* @brief uploadFile
* @param server like google.com or 127.0.0.1
* @param uri like /upload.php
* @param LocalFile
* @param remoteFile
* @param response a pointer to the response page. you need to free this pointer.
* @param responseSize size of response page
* @return
*/
bool HttpHelper::uploadFile(const TCHAR *server, const TCHAR *uri, const TCHAR *localFile, const char *remoteFile, char **response, int &responseSize){

	HINTERNET   hSession, hConnection, hRequest;
	DWORD dwBytesRead = 0;
	DWORD dwBytesWritten = 0;
	BYTE pBuffer[BUFFSIZE];
	TCHAR uriEncoded[BUFFSIZE];
	DWORD size = BUFFSIZE;

	if (InternetAttemptConnect(0) != ERROR_SUCCESS){
		MYPRINTF("InternetAttemptConnect failed (%d)\n", GetLastError());
		return 0;
	}

	if (InternetCanonicalizeUrl(uri, uriEncoded, &size, ICU_ENCODE_SPACES_ONLY) == false){
		MYPRINTF("InternetCanonicalizeUrlA failed (%d)\n", GetLastError());
		return FALSE;
	}

	hSession = InternetOpen( TEXT(AGENT), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
	if( hSession == NULL ) {
		MYPRINTF("HTTP Open failed\n");
		return FALSE;
	}

	hConnection = InternetConnect( hSession, server,
		INTERNET_DEFAULT_HTTP_PORT, NULL, NULL,//
		INTERNET_SERVICE_HTTP, 0, 0 );
	if( hConnection == NULL ) {
		MYPRINTF("HTTP Connection failed (%d)\n", GetLastError());
		InternetCloseHandle( hSession );
		return FALSE;
	}

	hRequest = HttpOpenRequest( hConnection, TEXT("POST"), uriEncoded,
		HTTP_VERSION, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION &&
		INTERNET_FLAG_NO_CACHE_WRITE, 0 );
	if( hRequest == NULL ) {
		MYPRINTF("HTTP OpenRequest(POST) failed (%d)\n", GetLastError());
		InternetCloseHandle( hConnection );
		InternetCloseHandle( hSession );
		return FALSE;
	}

	HANDLE hFile = CreateFile(localFile, //File name
		GENERIC_READ, //Desired Access
		FILE_SHARE_READ, //Share Mode
		NULL, //Security Attribute
		OPEN_EXISTING, //Creation Disposition
		FILE_ATTRIBUTE_NORMAL, //Flags and Attributes
		NULL); //Template File

	if(hFile == INVALID_HANDLE_VALUE) {
		MYPRINTF("file open failed (%d)\n", GetLastError());
		InternetCloseHandle(hRequest);
		InternetCloseHandle( hConnection );
		InternetCloseHandle( hSession );
		return FALSE;
	}

	char *dataHeaderTemplate = HEADER_TEMPLATE;
	char dataHeader[BUFFSIZE];
	sprintf_s(dataHeader, BUFFSIZE, dataHeaderTemplate, remoteFile);

	BYTE *dataEnd = (BYTE *)HEADER_END;

	INTERNET_BUFFERS BuffersIn;
	DWORD fileSize = GetFileSize(hFile, NULL);
	createHeader(BuffersIn, fileSize, remoteFile);

	if(HttpSendRequestEx(hRequest, &BuffersIn, 0, NULL, 0) == TRUE) {
		DWORD sum = 0;
		if(!(InternetWriteFile(hRequest, dataHeader, strlen((char *)dataHeader), &dwBytesWritten))) {
			MYPRINTF("Error Write header File\n");
		}

		dwBytesWritten = 0;
		do {
			if(!(ReadFile(hFile, pBuffer, sizeof(pBuffer), &dwBytesRead, NULL))) {
				MYPRINTF("Error Read File\n");
				break;
			}
			
			if(!(InternetWriteFile(hRequest, pBuffer, dwBytesRead, &dwBytesWritten))) {
				MYPRINTF("Error Write File\n");
				break;
			}
			
			sum += dwBytesWritten;
		} while(dwBytesRead == sizeof(pBuffer));

		dwBytesWritten = 0;

		if(!(InternetWriteFile(hRequest, dataEnd, strlen((char *)dataEnd), &dwBytesWritten))) {
			MYPRINTF("Error Write end header File\n");
		}

		if(!HttpEndRequest(hRequest, NULL, 0, 0)) {
			MYPRINTF( "Error on HttpEndRequest %d \n", GetLastError());
		}
		
		readInternetFile(hRequest, response, responseSize);
	} else {
		MYPRINTF( "Error on HttpSendRequestEx %d \n", GetLastError());
	}

	CloseHandle(hFile);

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hSession);
	return true;
}


/**
* @brief fileToBuffer read a file to a buffer
* @param file
* @param buffer a pointer to char*. you need to free the buffer after.
* @param size
* @return
*/

bool HttpHelper::fileToBuffer(const TCHAR *file, char **buffer, int &size){

	HANDLE hFile = CreateFile(file, //File name
		GENERIC_READ, //Desired Access
		FILE_SHARE_READ, //Share Mode
		NULL, //Security Attribute
		OPEN_EXISTING, //Creation Disposition
		FILE_ATTRIBUTE_NORMAL, //Flags and Attributes
		NULL); //Template File

	if(hFile == INVALID_HANDLE_VALUE) {
		MYPRINTF("file open failed (%d)\n", GetLastError());
		return FALSE;
	}

	DWORD dwBytesRead = 0;
	char pBuffer[BUFFSIZE];
	size = 0;
	*buffer = 0;
	do {
		if(!(ReadFile(hFile, pBuffer, sizeof(pBuffer), &dwBytesRead, NULL))) {
			MYPRINTF("Error Read File\n");
			break;
		}

		size += dwBytesRead;
		*buffer = (char *)realloc(*buffer, size);

		memcpy(*buffer + (size - dwBytesRead), pBuffer, dwBytesRead);

	} while(dwBytesRead == sizeof(pBuffer));

	CloseHandle(hFile);
	return true;
}


/**
* @brief uploadFile a buffer to server
* @param server like 127.0.0.1
* @param uri like /upload
* @param buffer buffer to upload
* @param size
* @param remoteFile
* @param response you need to free this after
* @param responseSize
* @return
*/

bool HttpHelper::uploadBuffer(const TCHAR *server, const TCHAR *uri, const char* bufferToSend, int sizeToSend, const char *remoteFile, char **response, int &responseSize){

	HINTERNET   hSession, hConnection, hRequest;
	DWORD dwBytesWritten = 0;
	TCHAR uriEncoded[BUFFSIZE];
	DWORD size = BUFFSIZE;

	if (InternetAttemptConnect(0) != ERROR_SUCCESS){
		MYPRINTF("InternetAttemptConnect failed (%d)\n", GetLastError());
		return 0;
	}

	if (InternetCanonicalizeUrl(uri, uriEncoded, &size, ICU_ENCODE_SPACES_ONLY) == false){
		MYPRINTF("InternetCanonicalizeUrlA failed (%d)\n", GetLastError());
		return FALSE;
	}


	hSession = InternetOpen( TEXT(AGENT), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
	if( hSession == NULL ) {
		MYPRINTF("HTTP Open failed\n");
		return FALSE;
	}

	hConnection = InternetConnect( hSession, server,
		INTERNET_DEFAULT_HTTP_PORT, NULL, NULL,//
		INTERNET_SERVICE_HTTP, 0, 0 );
	if( hConnection == NULL ) {
		MYPRINTF("HTTP Connection failed (%d)\n", GetLastError());
		InternetCloseHandle( hSession );
		return FALSE;
	}

	hRequest = HttpOpenRequest( hConnection, TEXT("POST"), uriEncoded,
		HTTP_VERSION, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION &&
		INTERNET_FLAG_NO_CACHE_WRITE, 0 );

	if( hRequest == NULL ) {
		MYPRINTF("HTTP OpenRequest(POST) failed (%d)\n", GetLastError());
		InternetCloseHandle( hConnection );
		InternetCloseHandle( hSession );
		return FALSE;
	}

	char *dataHeaderTemplate = HEADER_TEMPLATE;
	char dataHeader[BUFFSIZE];
	sprintf_s(dataHeader, BUFFSIZE, dataHeaderTemplate, remoteFile);

	BYTE *dataEnd = (BYTE *)HEADER_END;

	INTERNET_BUFFERS buffersIn;

	createHeader(buffersIn, sizeToSend, remoteFile);
	if (HttpSendRequestEx(hRequest, &buffersIn, 0, 0, 0) == TRUE) {
		if (!(InternetWriteFile(hRequest, dataHeader, strlen((char *)dataHeader), &dwBytesWritten))) {
			MYPRINTF("Error Write header File: %u\n", GetLastError());
		}
		
		dwBytesWritten = 0;

		if (!(InternetWriteFile(hRequest,  bufferToSend, sizeToSend, &dwBytesWritten))) {
			MYPRINTF("Error Write File: %u\n", GetLastError());
		}

		if (dwBytesWritten != sizeToSend) {
			MYPRINTF("Upload is not complete : size: %d copyed: %d\n", sizeToSend, dwBytesWritten);
		}
		
		dwBytesWritten = 0;

		if(!(InternetWriteFile(hRequest, dataEnd, strlen((char *)dataEnd), &dwBytesWritten))) {
			MYPRINTF("Error Write end header File: %u\n", GetLastError());
		}


		if(!HttpEndRequest(hRequest, NULL, 0, 0)) {
			MYPRINTF( "Error on HttpEndRequest %u \n", GetLastError());
		}
		
		readInternetFile(hRequest, response, responseSize);
	} else {
		MYPRINTF( "Error on HttpSendRequestEx %u \n", GetLastError());
	}


	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hSession);
	return true;
}

