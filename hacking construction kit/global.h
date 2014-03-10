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

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define BUFFER_STRING_SIZE 1024

// Some globals variables
class Global {
public:
	static Global &get();

	Global::~Global();

	// Create mutex and events
	void init();

	// get public internet ip
	std::string getInternetIp();

	// get lan ip
	std::string getPrivateIp();

	// get ip used for internet or lan ip
	std::string getPublicIp();

	// test SERVICE_RUNNING_EVENT for running state
	bool isRunning();

	// set event on SERVICE_RUNNING_EVENT
	void stopRunning();

	// test if the host has a 192.168 network
	bool hasAPrivateNetwork();

	// event use to see if service is running
	HANDLE SERVICE_RUNNING_EVENT;

	static Global m_instance;

private:
	Global();

	std::string m_internetIp;
	std::string m_privateIp;
	std::string m_publicIp;

	HANDLE MUTEX;

	// init host ip
	void initLocalIp();
	// retrieve internet ip from web api
	void initInternetIp();

	bool m_isInit;
	bool m_isPrivateInit;
	bool m_isInternetInit;
};
