/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include <stdlib.h>
#include <tchar.h>
#include <stdio.h>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <xwebcam.h>

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	if (argc != 3){
#ifdef _DEBUG
		printf("take a snapshot from webcam and save to file\n");
		printf("usage: %s filename details[0-100]\n", argv[0]);
#endif
		return 0;
	}
	XWebcam cam;
	int detail = atoi(argv[2]); 
	cam.snap(argv[1], 1, detail);
	return 0;
}

