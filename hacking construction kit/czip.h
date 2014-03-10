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
#include <vector>

// Compress files
class Zip {

public:
	// compress files from sourcepath to output file
	// use zlib with default compression
	// sourcePath is the path for files. if files is a complete path, sourcePath must be empty
	static int compressFiles(std::string sourcePath, std::vector<std::string> files, std::string output);

	static const int WRITE_BUFFER_SIZE = 2048;

};