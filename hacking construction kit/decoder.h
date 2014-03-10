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

// This class manage crypt and encode functions
class Decoder {

public:
	// decode this string with RSA
	std::string rsaDecode(char *encoded);

	// RSA modulus
	static std::string modulus;

	// RSA exponent
	static std::string exponent;

	// do a simple encoding: add +1 to all alpha chars
	static char *simpleEncode(char *encodedTarget);

	// do a simple encoding: add +1 to all alpha chars
	static std::string simpleEncode(std::string line);


};

