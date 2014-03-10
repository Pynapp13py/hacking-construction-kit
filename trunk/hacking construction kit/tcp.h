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

#include <exception>

#include <WinSock2.h>

#pragma warning( disable : 4290 )
/**
* Class for manage a non-bloquing socket
* TODO: review all this class
*/
class Tcp {
public:
	// create a non-bloquing socket
	Tcp(std::string ip, int port) throw (std::exception);
	// create a Tcp class from this socket
	Tcp(SOCKET socket);

	Tcp(Tcp& tcp);

	~Tcp();

	// print response
	static void Tcp::readResponseFromSocket(SOCKET socket, bool print = true);

	// bind a local socket
	void tcpbind() throw (std::exception);

	// read all and create a string
	int readall(char *buffer, int size);

	// start listening for incoming connection
	void tcplisten() throw (std::exception);

	// accept a connection
	SOCKET tcpaccept() throw (std::exception);

	// send a string
	int tcpsend(char *) throw (std::exception);

	// send a string
	int tcpsend(char *, int size) throw (std::exception);

	// receive a string
	int Tcp::tcprecv(char *buffer, int size) throw (std::exception);

	// connect the socket
	void tcpconnect() throw (std::exception);

	// select for write or read
	bool tcpselect(int seconds = 1, bool read = true) throw (std::exception);

	// select read
	bool selectread(int seconds = 1);

	// select write
	bool selectwrite(int seconds = 1);

	static bool tcpselect(int seconds, bool read, SOCKET socket, int milisecond = 0);

	static bool setBloquingMode(SOCKET socket);

	SOCKET m_socket;
private:

	std::string m_ip;
	int m_port;
	
};

