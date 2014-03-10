/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "file.h"

#include <stack>
#include <iostream>
#include <fstream>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "macro.h"

using namespace std;

bool File::findFile(std::vector<std::string>& files, std::string path, std::string filename, DWORD &totalSize, DWORD minSize, DWORD maxSize) {
	WIN32_FIND_DATA ffd;

	stack<string> directories;
	directories.push(path);

	while (!directories.empty()) {
		path = directories.top();
		string spec = path + "\\*";
		directories.pop();

		HANDLE hFind = FindFirstFile(spec.c_str(), &ffd);
		if (hFind == INVALID_HANDLE_VALUE)  {
			MYPRINTF("FindNextFile: INVALID_HANDLE_VALUE\n");
			continue;
		} 

		do {
			string tempFilename = ffd.cFileName;
			if (strcmp(ffd.cFileName, ".") != 0 && 
				strcmp(ffd.cFileName, "..") != 0) {
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						directories.push(path + "\\" + ffd.cFileName);
					} else {
						if (filename.empty() || filename.compare(tempFilename) == 0){
							if (minSize < ffd.nFileSizeLow){
								if (maxSize == 0 || maxSize > ffd.nFileSizeLow){
									totalSize += ffd.nFileSizeLow;
									files.push_back(path + "\\" + ffd.cFileName);
								}
							}
						}
					}
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		if (GetLastError() != ERROR_NO_MORE_FILES) {
			MYPRINTF("FindNextFile: %d\n", GetLastError());
			FindClose(hFind);
			return false;
		}

		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}

	return true;
}


bool File::replaceInFile(std::string filein, std::string fileout, char *searched, unsigned int size, char *replace){

	string replaceString(replace);
	if (replaceString.size() < size){
		replaceString.resize(size, '\0');
	} else if (replaceString.size() > size){
		return false;
	}

	ofstream ofile;
	ofile.open (fileout, ios::binary|ios::trunc);
	if (!ofile.is_open()){
		return false;
	}

	ifstream ifile;
	ifile.open(filein, ios::in|ios::binary|ios::ate);
	if (!ifile.is_open()){
		return false;
	}

	streampos length = ifile.tellg();
	
	char *buffer = new char [static_cast<unsigned int>(length)];

	cout << "Reading " << length << " characters... ";
	// read data as a block:
	ifile.seekg(0, ios::beg);
	ifile.read (buffer, length);

	const char *replaceConst = replaceString.c_str();

	bool res = false;

	for (char *it = buffer; it < buffer + static_cast<unsigned int>(length); it++){
		if (it < buffer + static_cast<unsigned int>(length) - size){
			if(strncmp(it, searched, size) == 0){
				ofile.write(replaceConst, size);
				it = it + size - 1;
				res = true;
			} else {
				ofile.write(it, 1);
			}
		} else {
			ofile.write(it, 1);
		}
	}

	delete [] buffer;

	return res;
}


std::string File::displayVolumePaths(PCHAR volumeName) {
	DWORD  charCount = MAX_PATH + 1;
	PCHAR names     = 0;
	PCHAR nameIdx   = 0;
	BOOL   success   = FALSE;

	for (;;) {
		//
		//  Allocate a buffer to hold the paths.
		names = (PCHAR) new BYTE [charCount * sizeof(CHAR)];
		//
		//  Obtain all of the paths
		//  for this volume.
		success = GetVolumePathNamesForVolumeName(volumeName, names, charCount, &charCount);
		if (success) {
			break;
		}
		if (GetLastError() != ERROR_MORE_DATA){
			break;
		}
		//
		//  Try again with the
		//  new suggested size.
		delete [] names;
		names = 0;
	}
	std::string path;
	if (success) {
		//
		//  Display the various paths.
		for (nameIdx = names; nameIdx[0] != L'\0'; nameIdx += strlen(nameIdx) + 1) {
			//wprintf(L"  %s", nameIdx);
			path = nameIdx;
		}
		//wprintf(L"\n");
	}

	if (names != NULL) {
		delete [] names;
		names = 0;
	}

	return path;
}

bool File::getDiskPath(std::vector<std::string> &volumes, std::vector<std::string> &devices, std::vector<std::string> &paths){
	DWORD  charCount            = 0;
	CHAR  deviceName[MAX_PATH] = "";
	DWORD  error                = ERROR_SUCCESS;
	HANDLE findHandle           = INVALID_HANDLE_VALUE;
	BOOL   found                = FALSE;
	size_t index                = 0;
	BOOL   success              = FALSE;
	CHAR  volumeName[MAX_PATH] = "";

	//
	//  Enumerate all volumes in the system.
	findHandle = FindFirstVolume(volumeName, ARRAYSIZE(volumeName));

	if (findHandle == INVALID_HANDLE_VALUE) {
		error = GetLastError();
		MYPRINTF("FindFirstVolumeW failed with error code %d\n", error);
		return false;
	}

	for (;;) {
		//
		//  Skip the \\?\ prefix and remove the trailing backslash.
		index = strlen(volumeName) - 1;

		if (volumeName[0]     != L'\\' ||
			volumeName[1]     != L'\\' ||
			volumeName[2]     != L'?'  ||
			volumeName[3]     != L'\\' ||
			volumeName[index] != L'\\') {
				error = ERROR_BAD_PATHNAME;
				MYPRINTF("FindFirstVolumeW/FindNextVolumeW returned a bad path: %s\n", volumeName);
				break;
		}

		//
		//  QueryDosDeviceW does not allow a trailing backslash,
		//  so temporarily remove it.
		volumeName[index] = L'\0';

		charCount = QueryDosDevice(&volumeName[4], deviceName, ARRAYSIZE(deviceName)); 

		volumeName[index] = L'\\';

		if ( charCount == 0 ) {
			error = GetLastError();
			MYPRINTF("QueryDosDeviceW failed with error code %d\n", error);
			break;
		}

		devices.push_back(deviceName);
		volumes.push_back(volumeName);
		string path = File::displayVolumePaths(volumeName);
		if (!path.empty()){
			if (path != "A:\\" && path != "B:\\"){
				paths.push_back(path);
			}
		}

		//
		//  Move on to the next volume.
		success = FindNextVolume(findHandle, volumeName, ARRAYSIZE(volumeName));

		if ( !success ) {
			error = GetLastError();

			if (error != ERROR_NO_MORE_FILES) {
				MYPRINTF("FindNextVolumeW failed with error code %d\n", error);
				break;
			}

			//
			//  Finished iterating
			//  through all the volumes.
			error = ERROR_SUCCESS;
			break;
		}
	}

	FindVolumeClose(findHandle);
	findHandle = INVALID_HANDLE_VALUE;

	return true;

}

bool File::listDirectory(std::string path, std::vector<std::string> &files, std::vector<std::string> &directories){
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;   

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	if (path.length() > (MAX_PATH - 3))	{
		MYPRINTF("\nDirectory path is too long.\n");
		return false;
	}

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	path.append("\\*");

	// Find the first file in the directory.

	hFind = FindFirstFile(path.c_str(), &ffd);

	if (INVALID_HANDLE_VALUE == hFind) {
		MYPRINTF("INVALID_HANDLE_VALUE");
		return false;
	} 

	// List all the files in the directory with some info about them.

	do {
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
			directories.push_back(ffd.cFileName);
		} else {
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
			files.push_back(ffd.cFileName);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) {
		MYPRINTF("ERROR_NO_MORE_FILES");
	}

	FindClose(hFind);
	return true;
}