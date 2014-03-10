/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "print.h"

#include "cwebcam.h"
#include "xwebcam.h"
#include "macro.h"

#include "memory_debug.h"

XWebcam::XWebcam(){}

XWebcam::~XWebcam(){
	freeGlobals();
}

bool XWebcam::snap(const TCHAR *name, int index, int quality, int width, int height, int initialization){

	if (request_webcam_start(width, height, index) == 0){
		Sleep(initialization);// wait for initialisation ?
		if (request_webcam_get_frame(name, quality) == 0){
			request_webcam_stop();
		} else {
			MYPRINTF( "Unable to get frame\n");
			return false;
		}
	} else {
		MYPRINTF( "Unable to start webcam\n");
		return false;
	}
	return true;

}

bool XWebcam::snapMotionDetection(const char *name,
	int index,
	int quality,
	int initialization,
	int width,int height,
	int threshold, int diffInPercent, int pixels){
		MYPRINTF("Try to start webcam\n");
		if (request_webcam_start(width, height, index) == 0){
			MYPRINTF("start webcam ok\n");
			Sleep(initialization);// wait for initialisation ?
#ifdef _DEBUG
			int res = request_webcam_motion_detection(name, quality, threshold, diffInPercent, pixels, "diff.jpg");
#else
			int res = request_webcam_motion_detection(name, quality, threshold, diffInPercent, pixels);
#endif
			request_webcam_stop();
			if (res == 0) {
				return true;
			} else if (res == -1) {			
				return false;
			} else {
				MYPRINTF( "Unable to get frame\n");
				return false;
			}
		}
		MYPRINTF( "Unable to start webcam\n");
		return false;
}

bool XWebcam::snapMotionDetectionToMemory(char **buffer, DWORD &size, int index, int quality,
	int initialization,
	int width, int height,
	int threshold, int motion, int pixels){
		MYPRINTF("Try to start webcam\n");
		if (request_webcam_start(width, height, index) == 0){
			MYPRINTF("start webcam ok\n");
			Sleep(initialization);// wait for initialisation ?
#ifdef _DEBUG
			int res = request_webcam_motion_detection_to_buffer(buffer, size, quality, threshold, motion, pixels, "diff.jpg");
#else
			int res = request_webcam_motion_detection_to_buffer(buffer, size, quality, threshold, motion, pixels);
#endif
			request_webcam_stop();
			if (res == 0) {
				return true;
			} else if (res == -1) {			
				return false;
			} else {
				MYPRINTF( "Unable to get frame\n");
				return false;
			}
		}
		MYPRINTF( "Unable to start webcam\n");
		return false;
}

void XWebcam::listWebcam(){
	request_webcam_list();
}

int XWebcam::webcamCount(){
	int count = 0;
	request_webcam_count(count);

	return count;
}