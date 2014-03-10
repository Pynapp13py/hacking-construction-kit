/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "Icmp.h"

#include <winsock2.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")
#include <Icmpapi.h>

#include "macro.h"
#include "wsa_init.h"

#include "memory_debug.h"

static WsaInit init;

bool Icmp::ping(const char * target, DWORD timeout){
	// Declare and initialize variables

	char sendData[32] = "Data Buffer";

	unsigned long ipaddr = inet_addr(target);
	if (ipaddr == INADDR_NONE) {
		return false;
	}

	HANDLE hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE) {
		return false;
	}    

	DWORD replySize = sizeof(ICMP_ECHO_REPLY) + sizeof(sendData) + 8;
	LPVOID replyBuffer = (VOID*) malloc(replySize);

	bool result = false;

	DWORD dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, sendData, sizeof(sendData), 
		NULL, replyBuffer, replySize, timeout);

	if (dwRetVal != 0) {
		PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)replyBuffer;

		if (pEchoReply->Status == IP_SUCCESS){
			result = true;
		}
	}

	free(replyBuffer);
	IcmpCloseHandle(hIcmpFile);
	return result;
}