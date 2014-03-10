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

#include "print.h"

// Get array size
#define COUNTOF(ary) (sizeof((ary)) / sizeof((ary)[0]))

// print to screen and to windows/temp/log
#ifdef LOG
#define MYPRINTF(s,...) do{\
	Print::myprintf(s,__VA_ARGS__);\
} while(0);
#else
#define MYPRINTF(s,...);
#endif
