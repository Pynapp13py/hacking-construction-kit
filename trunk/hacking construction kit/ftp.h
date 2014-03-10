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

#include "tstring.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Wininet.h>

/**
* Class Ftp
* Use this class to manage FTP connexion
*/
class Ftp {
public:
	Ftp(TCHAR *url, int port = 21, TCHAR *user = 0, TCHAR *password = 0);

	~Ftp();

	std::tstring getCurrentDirectory();

	void setCurrentDirectory(std::tstring directory);

	void createDirectory(std::tstring directory);

	void uploadFile(std::tstring local, std::tstring remote);

private:
	HINTERNET m_handle;
	HINTERNET m_h;
};