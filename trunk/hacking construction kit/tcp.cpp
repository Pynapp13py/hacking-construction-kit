/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "tcp.h"

#include <cstdio>

#pragma comment(lib, "ws2_32")

#include <ws2tcpip.h>

#include "macro.h"
#include "wsa_init.h"

#include "memory_debug.h"

static WsaInit init;

using namespace std;

#pragma warning( disable : 4290 )

#define BUFFER_SIZE 1024

Tcp::Tcp(Tcp& tcp): m_socket(tcp.m_socket), m_port(tcp.m_port){
	tcp.m_socket = 0;
}

Tcp::Tcp(std::string ip, int port): m_ip(ip), m_port(port), m_socket(INVALID_SOCKET) {
	m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);
	if (m_socket == INVALID_SOCKET) {
		MYPRINTF( "Error at socket(): %d\n", WSAGetLastError());
		throw exception("socket creation error");
	}
	u_long iMode = 1;

	int iResult = ioctlsocket(m_socket, FIONBIO, &iMode);
	if (iResult != NO_ERROR){
		MYPRINTF( "ioctlsocket failed with error: %d\n", iResult);
		throw exception("ioctlsocket failed");
	}
}

bool Tcp::setBloquingMode(SOCKET socket){
	u_long iMode = 0;

	int iResult = ioctlsocket(socket, FIONBIO, &iMode);
	if (iResult != NO_ERROR){
		MYPRINTF( "ioctlsocket failed with error: %d\n", iResult);
		return false;
	}
	return true;
}

Tcp::Tcp(SOCKET socket): m_socket(socket){

}

Tcp::~Tcp(){
	if (m_socket != 0){
		closesocket(m_socket);
	}
}

void Tcp::readResponseFromSocket(SOCKET socket, bool print){
	char buffer[8192 * 2];
	while (Tcp::tcpselect(1, true, socket)){		
		int iResult = recv(socket, buffer, COUNTOF(buffer), 0);
		while(iResult > 0){
			//MYPRINTF("bytes read: %d\n", iResult);
			for (int bufferIndex = 0; bufferIndex < iResult; bufferIndex++) {
				if (print){
					MYPRINTF("%c", buffer[bufferIndex]);
				}
			}
			iResult = recv(socket, buffer, COUNTOF(buffer), 0);
		}
		if (Tcp::tcpselect(0, true, socket, 200) == false){
			return;
		}
	}


}

void Tcp::tcpbind(){
	int iResult;
	struct sockaddr_in clientService;

	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(m_ip.c_str());
	clientService.sin_port = (u_short)htons(m_port);

	// Setup the TCP listening socket
	iResult = bind( m_socket, (struct sockaddr *) &clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		MYPRINTF("bind failed with error: %d\n", WSAGetLastError());
		throw exception("bind failed");
	}
}

void Tcp::tcplisten() throw (exception){
	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK){
			MYPRINTF("listen failed with error: %d\n", error);
			throw exception("listen failed");
		}

	}
}

SOCKET Tcp::tcpaccept() throw (exception){
	SOCKET clientSocket;
	clientSocket = accept(m_socket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		int error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK){
			return 0;
		}
		MYPRINTF("accept failed with error: %d\n", error);
		throw exception("accept failed");
	}
	return clientSocket;
}

int Tcp::tcpsend(char *tosend) throw (exception) {
	int iResult = send(m_socket, tosend, strlen(tosend), 0);
	if (iResult == SOCKET_ERROR) {
		MYPRINTF("send failed with error: %d\n", WSAGetLastError());
		throw exception("send failed");
	}
	return iResult;
}

int Tcp::tcpsend(char *tosend, int size) throw (exception) {
	int iResult = send(m_socket, tosend, size, 0);
	if (iResult == SOCKET_ERROR) {
		MYPRINTF("send failed with error: %d\n", WSAGetLastError());
		throw exception("send failed");
	}
	return iResult;
}

bool Tcp::selectread(int seconds)  throw (exception) {
	return tcpselect(seconds, true);
}

bool Tcp::selectwrite(int seconds)  throw (exception) {
	return tcpselect(seconds, false);
}

bool Tcp::tcpselect(int seconds, bool read, SOCKET socket, int milisecond)  throw (exception) {
	// Setup timeval variable
	struct timeval timeout;
	struct fd_set fds;
	// assign the second and microsecond variables
	timeout.tv_sec = seconds;
	timeout.tv_usec = milisecond;
	// Setup fd_set structure
	FD_ZERO(&fds);
	FD_SET(socket, &fds);
	// Possible return values:
	// -1: error occurred
	// 0: timed out
	// > 0: data ready to be read
	int res;
	if (read){
		res =  select(0, &fds, 0, 0, &timeout);
	} else {
		res =  select(0, 0, &fds, 0, &timeout);
	}
	if (res == SOCKET_ERROR){
		MYPRINTF("select function failed with error: %d\n", WSAGetLastError());
		throw exception("select function failed");
	} else if (res == 0){
		return false;
	}
	return true;
}

bool Tcp::tcpselect(int seconds, bool read)  throw (exception) {
	return Tcp::tcpselect(seconds, read, m_socket);
}

int Tcp::tcprecv(char *buffer, int size) throw (exception){
	int iResult = recv(m_socket, buffer, size, 0);
	if (iResult == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK){
			return 0;
		} else {
			MYPRINTF("recv failed with error: %d\n", error);
			throw exception("recv failed");
		}
	}
	return iResult;
}

int Tcp::readall(char *buffer, int size){
	if (!selectread()){
		return 0;
	}
	int read = tcprecv(buffer, size - 1);
	buffer[read] = '\0';
	return read;
}

void Tcp::tcpconnect() throw (exception) {

	struct sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(m_ip.c_str());
	clientService.sin_port = (u_short)htons(m_port);

	int iResult = connect(m_socket, (struct sockaddr *) &clientService, sizeof(clientService));
	if (iResult != 0) {
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK){
			MYPRINTF( "connect error: %d\n", error);
			throw exception("connect error");
		}
	}

}