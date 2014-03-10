/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "wsa_init.h"

#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32")

bool WsaInit::isInit = false;

WsaInit::WsaInit(){
	if (!isInit){
		isInit = true;
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2,2), &wsaData);
	}
}

WsaInit::~WsaInit(){
	if (!isInit){
		WSACleanup();
	}
}