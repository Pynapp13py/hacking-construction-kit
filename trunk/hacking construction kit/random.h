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

// Utility class for creating random values
class Random {

public:
	Random();

	// create random ip address
	// ip must be a buffer of at least 20 bytes
	static int createRandomIp(char *ip, int size, bool useOnlyPrivateAddress);

	// create random ip address on 192.168 network
	// ip must be a buffer of at least 20 bytes
	static int createRandomIpPrivate(char *ip, int size);

	// create random ip address
	// ip must be a buffer of at least 20 bytes
	static int createRandomIpPublic(char *ip, int size);

	// TODO: move
	static int sp2Ip(char *target, int size);

	// return a random name of size
	static std::string createRandomName(int size);

	// create a random name of size characters
	static void createRandomName(int size, char *name);

	// create a random name of size characters
	static void createRandomName(int size, wchar_t *name);

	// create a random file name of size characters with extension
	static std::string createRandomFileName(int size, std::string extension);

	static const char *internetMask;
	static const char *privateMask;

private:

	static Random random;

};