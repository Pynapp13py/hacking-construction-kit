/*
 * Author: thirdstormofcythraul@outlook.com
 */
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <tchar.h>
#include <iostream>

#include <service.h>
#pragma comment(lib, "hacking construction kit.lib")
#pragma comment(lib, "jpeg.lib")
#include <macro.h>
#include <screen.h>

#include <memory_debug.h>

int _tmain(int argc, _TCHAR* argv[])
{
#if defined (WIN32) && defined (_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
#endif
		
	if (argc != 2){
#ifdef _DEBUG
		Screen::screenshotToConsole();
		printf("take a screenshot\n");
		printf("usage: %s filename\n", argv[0]);
#endif
		return 1;
	}

	Screen::screenshot(argv[1]);

	//char i = 0;
	//std::cin >> i;

	return 0;
}

