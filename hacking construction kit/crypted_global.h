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
#include <vector>

// rc4 key
// it'll be used in encode_string.py for crypt the strings
#define CRYPTED_STRING_KEY "abcdefghijkl"
#define KEY1 "abcdef"
#define KEY2 "ghijkl"

// the python script will encode a with rc4
// usage: SnE("10.0.0.254", "") will be translated in SnE("qsdqs5df4qd", "10.0.0.254")
// SnE will decrypt the string
#define SnE(a, b) CryptedGlobal::decodeNewString(a)

// This class is used to crypt with rc4 strings
// for obfuscate them in the executable
// the script encode_string.py must be called before the compilation
class CryptedGlobal {
public:
	static const CryptedGlobal& get();

	~CryptedGlobal();

	// decode this string with rc4
	// you have to free the result string
	static char *decodeNewString(const char *str);

	std::vector<char *> *m_strings;

private:
	CryptedGlobal();
		
	static const CryptedGlobal m_instance;
};