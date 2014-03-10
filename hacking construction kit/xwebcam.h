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
#include <Windows.h>

// Manage webcam with directx
class XWebcam {
public:
	XWebcam();

	~XWebcam();

	// get a picture from webcam
	// initialization: wait for webcam init, in miliseconds
	bool snap(const TCHAR *name, int index = 1, 
		int quality = 100, int width = 640, int height = 480, int initialization = 2000);

	// get a picture from webcam
	// snap a first picture and
	// after only if motion is detected
	// threshold: difference between two pixels
	// diffInPercent: difference between two images in percent
	// pixels: aggregate adjacents pixels to avoid false detection
	bool snapMotionDetection(const char *name, int index = 1, int quality = 100,
		int initialization = 2000,
		int width = 640, int height = 480,
		int threshold = 10, int diffInPercent = 5, int pixels = 3);

	// get a picture from webcam
	// snap a first picture and
	// after only if motion is detected
	// save jpg in memory buffer
	// buffer: you need to free the buffer after the call
	// size of buffer
	bool snapMotionDetectionToMemory(char **buffer, DWORD &size, int index = 1, int quality = 100,
		int initialization = 2000,
		int width = 640, int height = 480,
		int threshold = 10, int diffInPercent = 5, int pixels = 3);

	// print all webcam names
	void listWebcam();

	// get webcam count
	int webcamCount();

private:

};