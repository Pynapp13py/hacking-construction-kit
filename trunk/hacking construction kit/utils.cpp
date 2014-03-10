/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "utils.h"

#include <stdlib.h>

#include <time.h>

#include "network.h"
#include "macro.h"

#include "memory_debug.h"

bool Utils::isDateObsolete(int year, int month){
#define MYLANG MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT)
#define LOCALE_409 MAKELCID(MYLANG, SORT_DEFAULT)
	char yearstring[3];
	char monthstring[3]; 
	GetDateFormat( LOCALE_409,
		0,
		NULL, /*localtime, not GMT*/
		"yy",
		yearstring,
		sizeof(yearstring));
	GetDateFormat( LOCALE_409,
		0,
		NULL, /*localtime, not GMT*/
		"M",
		monthstring,
		sizeof(monthstring));
	if (atoi(yearstring) >= year && atoi(monthstring) >= month) {
		return true;
	}
	return false;
}

HANDLE Utils::sandboxEvent = 0;

DWORD WINAPI Utils::checkSandbox(LPVOID){
	Sleep(30000);
	SetEvent(Utils::sandboxEvent);
	return 1;
}

bool Utils::isSandbox(){
	// threads
	DWORD   dwThreadIdArray;
	Utils::sandboxEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	DWORD tick = GetTickCount();
	CreateThread( 
		NULL,                   // default security attributes
		0,                      // use default stack size  
		Utils::checkSandbox,       // thread function name
		0,						// argument to thread function 
		0,                      // use default creation flags 
		&dwThreadIdArray);   // returns the thread identifier 

	WaitForSingleObject(Utils::sandboxEvent, INFINITE);
	bool res = false;
	if (tick + 20000 > GetTickCount()){
		res = true;
	}
	CloseHandle(Utils::sandboxEvent);
	return false;
}

int Utils::waitALittle(int count){
	int res = 0;
	while(count--){
		res = Network::isPortOpen("8.8.8.8", 80);
		MYPRINTF( "google respond %d\n", res);
		res = Network::isPortOpen("8.8.4.4", 53);
		MYPRINTF( "google respond %d\n", res);
	}
	return 0;
}