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
/**
* Utility class for printing and saving strings and buffers
*/
class Print {

public:
	// printf function for logging in file + out
	// write to windows\temp\log
	static void myprintf(char *format, ...);

	// print a buffer of char
	// count is the size of the buffer
	static void printBuffer(const char* buffer, int size);
	static void printBuffer(const unsigned char* buffer, int size);
	static void printBufferAsHexa(const unsigned char* buffer, int size);

	// save a buffer to a file
	static bool saveBuffer(const char* buffer, int size, const char* name);
};