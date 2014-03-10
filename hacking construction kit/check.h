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

// Try to find an host with a port open
// use random Ip, with public or private network
class Check {
public:
	// if true, founded has a port open
	// priv to use only private address (192.168)
	// founded MUST be a buffer of 20 chars
	static bool findHostPortOpen(char *founded, int port, bool useOnlyPrivateAddress = false);
	// open maxTargets sockets simultaneously
	static int m_maxTargets;

private:
	Check();

	
};