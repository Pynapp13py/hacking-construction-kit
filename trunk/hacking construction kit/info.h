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

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "tstring.h"

/**
* Get infos on computer
*/
class Info {

public:
	/**
	* Get current SID
	*/
	static std::string getSid();

	/**
	* Get module complete path 
	* ex: foo/bar/baz.dll
	*/
	static std::string getModulePath();

	/**
	* Get module name
	* ex: baz.dll
	*/
	static std::string getModuleName();

	/**
	* Get volume serial number
	*/
	static bool getUniqueId(DWORD& lpVolumeSerialNumber);

	/**
	* Get architectyure of processor: 32 or 64 bits
	*/
	static std::tstring getProcessor();

	/**
	* Get os version
	*/
	static std::tstring getVersion();

	/**
	* Get os version
	*/
	static bool getOSDisplayString(LPTSTR pszOS);

	/**
	* Get computer name
	*/
	static bool getComputerName(TCHAR *buffer, DWORD &size);

	/**
	* Get up time
	*/
	static DWORD getUptime();

	/**
	* Get all users on this host
	*/
	static std::tstring getAllUsers();

	/**
	* Get language (EN, US...)
	*/
	static std::tstring getWindowsLanguage();

	/**
	* Get all IPs for this host
	*/
	static std::string getAllIps();

	/**
	* Get all infos concatened
	*/
	static std::string getAllInfos();


	/**
	* Get the name from this SID
	* ex: S-1-5-32-544 return Administrators
	*/
	static std::string getNameFromSid(const std::string sid);

};

