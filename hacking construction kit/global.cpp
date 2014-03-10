/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "global.h"

#include <stdio.h>

#include "base64.h"
#include "rc4.h"
#include "macro.h"
#include "network.h"

#include "memory_debug.h"

Global Global::m_instance = Global();


Global::Global():
	m_isInit(false),
	m_isInternetInit(false),
	m_isPrivateInit(false),
	SERVICE_RUNNING_EVENT(0) {
	MUTEX = CreateMutex(NULL, FALSE, NULL);
}

Global::~Global(){
	CloseHandle(SERVICE_RUNNING_EVENT);
	CloseHandle(MUTEX);
}

std::string Global::getInternetIp(){
	if (m_isInternetInit == false){
		WaitForSingleObject(Global::m_instance.MUTEX, INFINITE);
		if (m_isInternetInit == false){
			initInternetIp();
		}
		ReleaseMutex(Global::m_instance.MUTEX);
	}
	return m_internetIp;
}

bool Global::hasAPrivateNetwork(){
	return !Global::get().getPrivateIp().empty();
}

std::string Global::getPrivateIp(){	
	if (m_isPrivateInit == false){
		WaitForSingleObject(Global::m_instance.MUTEX, INFINITE);
		if (m_isPrivateInit == false){
			initLocalIp();
		}
		ReleaseMutex(Global::m_instance.MUTEX);
	}
	return m_privateIp;
}

std::string Global::getPublicIp(){
	if (m_isPrivateInit == false){
		WaitForSingleObject(Global::m_instance.MUTEX, INFINITE);
		if (m_isPrivateInit == false){
			initLocalIp();
		}
		ReleaseMutex(Global::m_instance.MUTEX);
	}
	return m_publicIp;
}

void Global::init(){
	// Create running event
	SERVICE_RUNNING_EVENT = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void Global::initLocalIp(){
	if (m_isPrivateInit){
		return;
	}

	std::string ip = Network::getPublicOrPrivate();

	m_publicIp = ip;

	ip = Network::getLocalIpPrivate();

	m_privateIp = ip;

	m_isPrivateInit = true;
}

void Global::initInternetIp(){
	if (m_isInternetInit){
		return;
	}
	if (!m_isPrivateInit){
		initLocalIp();
	}

#ifdef ATTACK_LOCAL
	Global::m_internetIp = (char *)malloc(100);
	strcpy_s(Global::get().m_internetIp, 100, Global::m_publicIp);
#else
	m_internetIp = Network::getInternetIp();
#endif


	if (m_internetIp.empty()){
		MYPRINTF("no internet ip, wait a little\n");
		Sleep(5000);
		m_internetIp = Network::getInternetIp();
		if (m_internetIp.empty()){
			Sleep(5000);
			m_internetIp = Network::getInternetIp();
			if (m_internetIp.empty()){
				Sleep(3600000);
				m_internetIp = Network::getInternetIp();
				if (m_internetIp.empty()){
					Sleep(3600000);
					m_internetIp = Network::getInternetIp();
					if (m_internetIp.empty()){
						m_internetIp = Global::get().getPublicIp();
					}
				}
			}
		}		
	}
	m_isInternetInit = true;
}

Global &Global::get(){
	if (!Global::m_instance.m_isInit){
		WaitForSingleObject(Global::m_instance.MUTEX, INFINITE);
		if (!Global::m_instance.m_isInit){
			Global::m_instance.init();
			Global::m_instance.m_isInit = true;
		}
		ReleaseMutex(Global::m_instance.MUTEX);
	}
	return Global::m_instance;
}

bool Global::isRunning(){
	DWORD res = WaitForSingleObject(SERVICE_RUNNING_EVENT, 0);
	return res == WAIT_TIMEOUT;
};

void Global::stopRunning(){
	if (SetEvent(SERVICE_RUNNING_EVENT) == 0){
		MYPRINTF( "SetEvent failed with error: %d\n", GetLastError());
	}
};



