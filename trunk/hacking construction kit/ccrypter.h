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
#include <Imagehlp.h>

// A simple xor crypter
class Crypter {

public:
	/**
	* Crypt infile.
	* This function will search for .text, xor the section, and add a new section with the stub.
	* The new section will have a random name.
	*/
	static bool crypt(const char *infile, const char *outfile);

	static DWORD freeSpaceInHeader(PVOID pvPEBase);

	static PIMAGE_SECTION_HEADER getVirtualyLastSectionHeader(PIMAGE_NT_HEADERS pNtHeaders);

	static PIMAGE_SECTION_HEADER getPhysicalyLastSectionHeader(PIMAGE_NT_HEADERS pNtHeaders);

	static PIMAGE_SECTION_HEADER insertSectionHeader(PVOID pvPEBase, LPCSTR lpName, DWORD dwVirtualSize, DWORD dwCharacteristics, PDWORD pdwSize);

	static BOOL insertSectionConfigInPE(PVOID pvPEBase, DWORD dwPESize, PVOID pvData, DWORD dwDataSize, PVOID *ppvNewPE, DWORD *pdwNewPESize);

	static bool saveFile(const char* nameFile, char* data, int sz);

	// the new section name
	static char *section;

	// try to evade sandbox by doing some calculations
	static bool evadeSandbox;

};