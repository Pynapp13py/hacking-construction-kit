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

// From metasploit

#define TLV_TYPE_EXTENSION_WEBCAM	0

#define TLV_TYPE_WEBCAM_IMAGE				\
	MAKE_CUSTOM_TLV(					\
	TLV_META_TYPE_RAW,			\
	TLV_TYPE_EXTENSION_WEBCAM,	\
	TLV_EXTENSIONS + 1)

#define TLV_TYPE_WEBCAM_INTERFACE_ID		\
	MAKE_CUSTOM_TLV(					\
	TLV_META_TYPE_UINT,			\
	TLV_TYPE_EXTENSION_WEBCAM,	\
	TLV_EXTENSIONS + 2)

#define TLV_TYPE_WEBCAM_QUALITY				\
	MAKE_CUSTOM_TLV(					\
	TLV_META_TYPE_UINT,			\
	TLV_TYPE_EXTENSION_WEBCAM,	\
	TLV_EXTENSIONS + 3)

#define TLV_TYPE_WEBCAM_NAME				\
	MAKE_CUSTOM_TLV(					\
	TLV_META_TYPE_STRING,		\
	TLV_TYPE_EXTENSION_WEBCAM,	\
	TLV_EXTENSIONS + 4)

// print webcam list
DWORD request_webcam_list();

// start webcam, with a maximum of width and height
DWORD request_webcam_start(LONG width, LONG height, UINT index);

// get a frame to file name and quality (0-100)
DWORD request_webcam_get_frame(const TCHAR* name, int quality);

// get a frame to buffer
DWORD request_webcam_get_frame_to_buffer(char **buffer, DWORD &size, int quality);

// stop webcam
DWORD request_webcam_stop();

// get a frame, with motion detection
// threshold is difference between two pixels, diffInPercent is percent of pics which has changed
// pixels is horizontal pixel average
// set savediff to 0 to avoid saving file differences
// return 0 on success, -1 on no frame detection, >1 on error
DWORD request_webcam_motion_detection(const TCHAR *name, int quality, int threshold = 10, int diffInPercent = 5, int pixels = 3, char *savediff = 0);

// get a frame and copy it to buffer
DWORD request_webcam_motion_detection_to_buffer(char **buffer, DWORD &size, int quality, int threshold = 10, int diffInPercent = 5, int pixels = 3, char *savediff = 0);

// get frame difference between two buffers
int frame_difference(PBYTE previous, PBYTE current, long imgsize, int threshold, int nbPixel);

// count webcam
DWORD request_webcam_count(int &count);

// print some stats between previous and current
int print_frame_stats(PBYTE previous, PBYTE current, long imgsize);

// write to difference array differences between previous and current
int show_frame_difference(PBYTE previous, PBYTE current, PBYTE difference, long imgsize, int threshold, int nbPixel, bool deleteMove);

// get average pixel luminosity
int avgPixel(PBYTE pixel, int i);

// get average for several pixels
int avgAdjacentPixel(PBYTE pixel, int i, int nb);

// delete globals array
void freeGlobals();

// create a bmp header for img
DWORD createBmp(PBYTE img, DWORD size);

