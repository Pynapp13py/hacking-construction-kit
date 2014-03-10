/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "check.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "random.h"
#include "macro.h"
#include "Icmp.h"
#include "network.h"

#include "global.h"

#include "memory_debug.h"

#define MAX_SOCKET 100

int Check::m_maxTargets = 10;

bool Check::findHostPortOpen(char * founded, int port, bool useOnlyPrivateAddress){
	if (Check::m_maxTargets > MAX_SOCKET){
		return false;
	}
	SOCKET sockets[MAX_SOCKET];
	char ips[MAX_SOCKET][20];
	char buffer[100];
	int iResult;
	struct fd_set sockets_set_write = {0};
	struct fd_set sockets_set_except = {0};
	int i;
	bool success = false;

	for(i = 0; i < Check::m_maxTargets; i++){
		Random::createRandomIp(buffer, 100, useOnlyPrivateAddress);
		MYPRINTF( "test %s\n", buffer);
		while(!Icmp::ping(buffer, 500) && Global::get().isRunning()){
			Random::createRandomIp(buffer, 100, useOnlyPrivateAddress);
			MYPRINTF( "test %s\n", buffer);
		}
		if (!Global::get().isRunning()){
			return false;
		}
		MYPRINTF("found %s (ping test)\n", buffer);
		strcpy_s(ips[i], 20, buffer);
		sockets[i] = Network::MyWSASocket();
		if (sockets[i] == INVALID_SOCKET) {
			return false;
		}

		iResult = Network::setIoctl(sockets[i]);
		if (iResult != NO_ERROR){
			return false;
		}
		
		iResult = Network::myconnect(sockets[i], ips[i], port);
		if (iResult == SOCKET_ERROR) {
			iResult = WSAGetLastError();
			if (iResult != WSAEWOULDBLOCK) {
				MYPRINTF("connect function failed with error: %d\n", WSAGetLastError());
				iResult = closesocket(sockets[i]);
				if (iResult == SOCKET_ERROR) {
					MYPRINTF("closesocket function failed with error: %d\n", WSAGetLastError());
				}
				return false;
			}
		}
	}

	for(i = 0; i < Check::m_maxTargets; i++){
		FD_ZERO(&sockets_set_write);
		FD_SET(sockets[i], &sockets_set_write);

		FD_ZERO(&sockets_set_except);
		FD_SET(sockets[i], &sockets_set_except);

		if (!success){
			iResult = select(0, 0, &sockets_set_write, &sockets_set_except, NULL);
			if (iResult == 0) {
				MYPRINTF("\tConnect failed.(timeout)\n");
			}else if (iResult == SOCKET_ERROR) {
				MYPRINTF("\tConnect failed.(%d)\n", WSAGetLastError());
			} else {
				if (FD_ISSET(sockets[i], &sockets_set_except) == 0){
					MYPRINTF("\tconnected to target !\n");
					strcpy_s(founded, 20, ips[i]);
					success = true;
				}
			}
		}
		closesocket(sockets[i]);
	}

	return success;
}