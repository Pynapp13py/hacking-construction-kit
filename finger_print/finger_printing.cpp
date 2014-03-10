/*
 * Author: thirdstormofcythraul@outlook.com
 */
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include <tchar.h>

#include <WinSock2.h>

#include <finger_print.h>
#include <random.h>
#include <Icmp.h>
#include <check.h>
#pragma comment(lib, "hacking construction kit.lib")


int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	if (argc != 2){
		printf("Try to find os version from ip or random ips if 'all'\n");
		printf("usage: %s ip|all\n", argv[0]);
		return 1;
	}

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	Check::m_maxTargets = 10;

	if (strcmp(argv[1], "all") == 0){
		for(int i = 0; i < 10; i++){
			char buffer[20];
			if (Check::findHostPortOpen(buffer, 139, false)){

				//char ip[17];
				//Random::createRandomIp(ip, 17, 0);
				//Icmp icmp;
				//if (icmp.ping(ip)){
				FingerPrint::connectAndGetLanguage(buffer);
			}
			//}
		}
	} else {
		FingerPrint::connectAndGetLanguage(argv[1]);
	}
	Sleep(5000);
	return 0;
}

