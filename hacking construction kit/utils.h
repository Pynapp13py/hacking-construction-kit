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

class Utils {
public:
	// Return true if the date is obsolete
	static bool isDateObsolete(int year, int month);
	
	// Try to test if we are in a sandbox
	// it just wait 30 sec and compare GetTickCount
	// maybe a sandbox replace Sleep() with NOP
	static bool isSandbox();

	// Used in isSandbox
	static DWORD WINAPI checkSandbox(LPVOID);

	// Used in isSandbox
	static HANDLE sandboxEvent;

	// Simulate a Sleep() with a ping to google
	// try to avoid sandboxing by waiting some seconds
	static int waitALittle(int count);

private:
	Utils();

};