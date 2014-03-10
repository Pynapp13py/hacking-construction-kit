/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "network.h"

#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32")
#include <iphlpapi.h>
#include <Icmpapi.h>
#include <Winhttp.h>
#pragma comment(lib, "Winhttp")

#include <tchar.h>

#include "print.h"
#include "macro.h"
#include "wsa_init.h"

#include "memory_debug.h"

static WsaInit init;

std::tstring Network::getInternetIp(){
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL, 
		hConnect = NULL,
		hRequest = NULL;
	char *localIp = NULL;
	char *nextToken;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen( L"ip/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0 );

	// Specify an HTTP server.
	if( hSession )
		hConnect = WinHttpConnect( hSession, L"api.hostip.info", INTERNET_DEFAULT_HTTP_PORT, 0 );

	// Create an HTTP request handle.
	if( hConnect )
		hRequest = WinHttpOpenRequest( hConnect, L"GET", L"/get_html.php", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0 );

	// Send a request.
	if( hRequest )
		bResults = WinHttpSendRequest( hRequest,WINHTTP_NO_ADDITIONAL_HEADERS, 0,	WINHTTP_NO_REQUEST_DATA, 0, 0, 0 );


	// End the request.
	if( bResults )
		bResults = WinHttpReceiveResponse( hRequest, NULL );

	// Keep checking for data until there is nothing left.
	if( bResults )
	{
		char *p;
		// Check for available data.
		dwSize = 0;
		if( !WinHttpQueryDataAvailable( hRequest, &dwSize ) ){
			MYPRINTF( "Error %d in WinHttpQueryDataAvailable.\n", GetLastError( ) );
		}

		// Allocate space for the buffer.
		pszOutBuffer = (LPSTR)malloc(dwSize+1);

		// Read the data.
		ZeroMemory( pszOutBuffer, dwSize+1 );

		if( !WinHttpReadData( hRequest, (LPVOID)pszOutBuffer, 
			dwSize, &dwDownloaded ) ){
				MYPRINTF( "Error %d in WinHttpReadData.\n", GetLastError( ));
		} else {
			MYPRINTF("%s", pszOutBuffer);
			p = strtok_s(pszOutBuffer, "\n", &nextToken);
			if (p != NULL){
				p = strtok_s(NULL, "\n", &nextToken);
				if (p != NULL){
					p = strtok_s(NULL, "\n", &nextToken);
					if (p != NULL){
						p = strtok_s(p, " ", &nextToken);
						if (p != NULL){
							p = strtok_s(NULL, " ", &nextToken);
							localIp = (char *)malloc(strlen(p) + 1);
							strcpy_s(localIp, strlen(p) + 1, p);
						}
					}
				}				
			}
		}
		// Free the memory allocated to the buffer.
		free(pszOutBuffer);
	}
	// Report any errors.
	if( !bResults ){
		MYPRINTF("Error %d has occurred.\n", GetLastError( ));
	}
	
	// Close any open handles.
	if( hRequest ) WinHttpCloseHandle( hRequest );
	if( hConnect ) WinHttpCloseHandle( hConnect );
	if( hSession ) WinHttpCloseHandle( hSession );

		// store this IP address in sa:
	if (localIp != NULL){
		struct sockaddr sock;

		int addrSize = sizeof(struct sockaddr);

		memset(&sock, 0, addrSize);

		sock.sa_family = AF_INET;

		int iRetval = WSAStringToAddress(localIp, AF_INET, 0, &sock, &addrSize);
		if (iRetval != 0){
			return 0;
		}
	}
#ifdef UNICODE
	if (localIp != NULL){
		TCHAR out[1024];
		int len;
		len = MultiByteToWideChar(CP_ACP, 0, localIp, -1, out, 1024);
		if (len < 0) {
			return TEXT("");
		}
		std::tstring result = out;
		return result;
	} else {
		return "";
	}


#else
	if (localIp != NULL){
		std::string res = localIp;
		free(localIp);
		return res;
	} else {
		return "";
	}
#endif
}

std::tstring Network::getPublicOrPrivate(){
	char *localIp = NULL;
	char *localPrivate = NULL;
	int iResult;
	char hostName[128] = "";
	// host name
	char * temp;
	// for resolving current address
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;
	struct sockaddr_in  *sockaddr_ipv4 = NULL;

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	gethostname(hostName, sizeof(hostName));
	iResult = getaddrinfo(hostName, NULL, &hints, &result);
	if ( iResult != 0 ) {
		MYPRINTF( "\tgetaddrinfo failed with error: %d\n", iResult);
		return "";
	}
	for(ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		switch (ptr->ai_family) {
		case AF_INET:
			sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
			temp = inet_ntoa(sockaddr_ipv4->sin_addr);
			if (strncmp(temp, "192.168", 7) != 0){
				free(localIp);
				localIp = (char *)malloc(100);
				strcpy_s(localIp, 100, temp);
			} else {
				free(localPrivate);
				localPrivate = (char *)malloc(100);
				strcpy_s(localPrivate, 100, temp);
			}
			break;
		}
	}
	if (localIp == NULL){
		localIp = (char *)malloc(100);
		strcpy_s(localIp, 100, localPrivate);
	}
	free(localPrivate);

	freeaddrinfo(result);
#ifdef UNICODE
	if (localIp != NULL){
		TCHAR out[1024];
		int len;
		len = MultiByteToWideChar(CP_ACP, 0, localIp, -1, out, 1024);
		if (len < 0) {
			return TEXT("");
		}
		std::tstring sresult = out;
		return sresult;
	} else {
		return "";
	}
	
#else
	if (localIp != NULL){
		std::string res = localIp;
		free(localIp);
		return res;
	} else {
		return "";
	}
#endif
}

bool Network::is192Network(){
	bool res = false;
	int iResult;
	char hostName[128] = "";
	// host name
	char * temp;
	// for resolving current address
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;
	struct sockaddr_in  *sockaddr_ipv4 = NULL;

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	gethostname(hostName, sizeof(hostName));
	iResult = getaddrinfo(hostName, NULL, &hints, &result);
	if ( iResult != 0 ) {
		MYPRINTF( "\tgetaddrinfo failed with error: %d\n", iResult);
		return false;
	}
	for(ptr=result; ptr != NULL; ptr=ptr->ai_next) {

		switch (ptr->ai_family) {
		case AF_INET:
			sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
			temp = inet_ntoa(sockaddr_ipv4->sin_addr);
			if (strncmp(temp, "192.168", 7) == 0){
				res = true;
			}
			break;
		}
	}
	freeaddrinfo(result);
	return res;
}

std::string  Network::getLocalIpPrivate(){
	char *localIp = NULL;
	int iResult;
	char hostName[128] = "";
	// host name
	char * temp = NULL;
	// for resolving current address
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;
	struct sockaddr_in  *sockaddr_ipv4 = NULL;

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	gethostname(hostName, sizeof(hostName));
	iResult = getaddrinfo(hostName, NULL, &hints, &result);
	if ( iResult != 0 ) {
		MYPRINTF( "\tgetaddrinfo failed with error: %d\n", iResult);
		return "";
	}
	for(ptr=result; ptr != NULL; ptr=ptr->ai_next) {

		switch (ptr->ai_family) {
		case AF_INET:
			sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
			temp = inet_ntoa(sockaddr_ipv4->sin_addr);
			if (strncmp(temp, "192.168", 7) == 0){
				free(localIp);
				localIp = (char *)malloc(100);
				strcpy_s(localIp, 100, temp);
			}
			break;
		}
	}
	freeaddrinfo(result);
	if (localIp != 0){
		std::string res = localIp;
		free(localIp);
		return res;
	}
	return "";
}

SOCKET Network::MyWSASocket(){
	SOCKET listenSocket;
	listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);
	if (listenSocket == INVALID_SOCKET) {
		MYPRINTF( "Error at socket(): %ld\n", WSAGetLastError());
	}
	return listenSocket;
}

int Network::setIoctl(SOCKET socket){
	u_long iMode = 1;
	int iResult;

	iResult = ioctlsocket(socket, FIONBIO, &iMode);
	if (iResult != NO_ERROR){
		MYPRINTF( "ioctlsocket failed with error: %ld\n", iResult);
	}
	return iResult;
}

int Network::myconnect(SOCKET clientSocket, char *ip, int port){
	struct sockaddr_in clientService;
	int iResult;

	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(ip);
	clientService.sin_port = (u_short)htons(port);
	iResult = connect( clientSocket, (struct sockaddr *) &clientService, sizeof(clientService));
	return iResult;
}

bool Network::isPortOpen(char *target, int port){
	SOCKET socket;
	int iResult;

	// Create a SOCKET for connecting to server
	socket = MyWSASocket();
	iResult = myconnect( socket, target, port);
	closesocket(socket);
	return iResult != 0;
}

/**
void Network::getCurrentName(TCHAR *szExeName){
	TCHAR  FileNamePath [256];
	unsigned int i;
	TCHAR *pch;
	size_t length;

	GetModuleFileName(0, FileNamePath, MAX_PATH);
	pch = _tcsrchr(FileNamePath,'\\');

	length =  _tcslen(pch);
	for(i=0; i < length; i++)
	{
		*pch++;
		szExeName[i] = *pch;
	}
}
*/