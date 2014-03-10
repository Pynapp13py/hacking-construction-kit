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
#include <Rpc.h>
#pragma comment(lib, "Mpr.lib")
#pragma comment(lib, "rpcrt4.lib")

// from srvsvc.h
extern RPC_BINDING_HANDLE srvsvc__MIDL_AutoBindHandle;
// from spoolss2.h
extern RPC_BINDING_HANDLE spoolss__MIDL_AutoBindHandle;

// create SMB connection
class Smb {

public:
	// connect to target with SMB
	static bool connect(const char *target);

	// close connection
	static bool closeConnection(const char *target);

	// bind to SPOOLSS pipe
	static bool bindToSpoolssPipe(unsigned const char* target);

	// bind to BROWSER pipe
	static bool bindToBrowserPipe(unsigned const char* target);

private:
	Smb();

	static bool bindToPipe(const unsigned char* target, unsigned char *pipe, RPC_BINDING_HANDLE handle);

};