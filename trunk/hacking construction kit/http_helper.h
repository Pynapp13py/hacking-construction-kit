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
#include <windows.h>
#include <WinInet.h>

typedef LPVOID HINTERNET;

class HttpHelper {
public:
	// Send HTTP get and retrieve page as array of LPSTR
	LPSTR* get(char *uri);

	// download from uri and save as filename
	void download(const char *uri, const char *filename);

	// send HTTP get, and read response
	bool sendGet(const TCHAR *server, const TCHAR *uri, char **response, int &responseSize);

	// send HTTP post, and read response
	bool sendPost(const TCHAR *server, const TCHAR *uri, const char *post, char **response, int &responseSize);

	/**
	* @brief uploadFile upload a file with POST
	* @param server like google.com or 127.0.0.1
	* @param resource like /upload.php
	* @param LocalFile source file name
	* @param remoteFile remote file name
	* @param response a pointer to the response page. you need to free this pointer.
	* @param responseSize size of response page
	* @return
	*/
	bool uploadFile(const TCHAR *server, const TCHAR *resource, const TCHAR *localFile, const char *remoteFile, char **response, int &responseSize);
	bool uploadFile(const TCHAR *uri, const TCHAR *localFile, const char *remoteFile, char **response, int &responseSize);

	/**
	* @brief fileToBuffer read a file to a buffer
	* @param file source file name
	* @param buffer a pointer to char*. you need to free the buffer after.
	* @param size
	* @return
	*/
	bool fileToBuffer(const TCHAR *file, char **buffer, int &size);

	/**
	* @brief uploadBuffer upload a buffer to server with POST request
	* @param server like 127.0.0.1
	* @param resource like /upload
	* @param buffer buffer to upload
	* @param size size of buffer
	* @param remoteFile remote file name
	* @param response you need to free this after
	* @param responseSize size of response
	* @return
	*/
	bool uploadBuffer(const TCHAR *server, const TCHAR *resource, const char* bufferToSend, int sizeToSend, const char *remoteFile, char **response, int &responseSize);

private:
	bool parseUri(const char *uri, char **url, char **resource);

	LPSTR* parseResponse(HINTERNET hRequest);

	LPSTR* callGet(const char *uri, const char *filename);

	bool writeToFile(HINTERNET hRequest, const char *filename);
	
	bool readInternetFile(HINTERNET hRequest, char **response, int &responseSize);

	bool createHeader(INTERNET_BUFFERS &internetBuffers, int size, const char *filename);

};

