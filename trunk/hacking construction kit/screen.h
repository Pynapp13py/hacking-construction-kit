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

#include <Windows.h>

/**
* Get desktop screenshot
*/
class Screen {

public:
	// show desktop screenshot on console
	static bool screenshotToConsole();

	// save screenshot to file (in jpeg format)
	static bool screenshot(const char *filename);

	// get screenshot as a buffer (in jpeg format)
	// you need to free the buffer after the call
	static bool Screen::screenshotToBuffer(char **buffer, DWORD &size);

private:
	static bool takeScreenshot(int outType, const char *filename, char **buffer, DWORD &size);

};