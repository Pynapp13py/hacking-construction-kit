/*
 * Hacking construction kit -- A GPL Windows security library.
 * While this library can potentially be used for nefarious purposes, 
 * it was written for educational and recreational
 * purposes only and the author does not endorse illegal use.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: thirdstormofcythraul@outlook.com
 */
#pragma once

#include <string>

#include <WinSock2.h>

#include "tstring.h"

/**
* Class Network
* network utily class
*/
class Network {

public:
	// get internet IP from api.hostip.info
	static std::tstring getInternetIp();
	// get public (used for internet), or if not found, private IP
	static std::tstring getPublicOrPrivate();
	// get 192.168 address
	static std::string getLocalIpPrivate();
	// this host have an address on 192.168 network
	static bool is192Network();

	static SOCKET MyWSASocket();

	static int setIoctl(SOCKET socket);

	static int myconnect(SOCKET clientSocket, char *ip, int port);
	// test if this port is open on target
	static bool isPortOpen(char *target, int port);

	//static void getCurrentName(char *szExeName);


};