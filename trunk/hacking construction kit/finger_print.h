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

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Use RPC on spoolss pipe to find language of os
class FingerPrint {
public:
	typedef enum {
					English, Spanish, Italian, French, German, Brazilian,
					Portuguese, Hungarian, Finnish, Dutch, Danish, Swedish,
					Polish ,Czech ,Turkish, Japanese, Chinese, Taiwan, Korean ,Russian, Dontknow, Nofinger
	} TARGETS;

	// call EnumPrinters to find language
	// you must be already connect to target (WNetAddConnection2)
	static TARGETS getLanguage(const char *ipOfTarget);

	// connect to smb, call EnumPrinters, and disconnect from smb
	static TARGETS connectAndGetLanguage(const char *ipOfTarget);

private:
	FingerPrint();

	static const char *searchInBuffer(const char *haystack, size_t haystacklen, const char *needle, size_t needlelen);

	static int fingerTest(const unsigned char *buffer, int count, wchar_t* search);

	static TARGETS parseBuffer(const unsigned char *buffer, int count);

};