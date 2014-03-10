/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "register.h"

#include <tchar.h>
#include <cstdio>
#include <locale.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Winreg.h>
#pragma comment(lib, "Advapi32.lib")

#include "print.h"
#include "macro.h"

#include "memory_debug.h"

bool Register::createStringKey(HKEY hKeyParam, const TCHAR *path, const TCHAR *key, const TCHAR *value) {

	HKEY hKey;
	DWORD dwDisp = 0;
	LPDWORD lpdwDisp = &dwDisp;
	LONG iSuccess;


	iSuccess = RegCreateKeyEx(hKeyParam, path, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, lpdwDisp);

	if(iSuccess == ERROR_SUCCESS)
	{
		iSuccess = RegSetValueEx (hKey, key, 0L, REG_SZ,(CONST BYTE*) value, _tcslen(value) * sizeof(TCHAR) + 1);
		if (iSuccess != ERROR_SUCCESS){
			MYPRINTF( "RegSetValueEx failed with error: %d\n", iSuccess);
			return false;
		}
	} else {
		MYPRINTF( "RegOpenKeyEx failed with error: %d\n", iSuccess);
		return false;
	}
	return true;
}

bool Register::createDwordKey(HKEY hKeyParam, const TCHAR *path, const TCHAR *key, int value) {

	HKEY hKey;
	DWORD dwDisp = 0;
	LPDWORD lpdwDisp = &dwDisp;
	LONG iSuccess;


	iSuccess = RegCreateKeyEx(hKeyParam, path, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, lpdwDisp);

	if(iSuccess == ERROR_SUCCESS)
	{
		iSuccess = RegSetValueEx (hKey, key, 0L, REG_DWORD, (const BYTE*)&value, sizeof(value));
		if (iSuccess != ERROR_SUCCESS){
			MYPRINTF( "RegSetValueEx failed with error: %d\n", iSuccess);
			return false;
		}
	} else {
		MYPRINTF( "RegOpenKeyEx failed with error: %d\n", iSuccess);
		return false;
	}
	return true;
}

std::tstring Register::getKey(const TCHAR *path, const TCHAR *mykey) {
	unsigned long type = REG_SZ, size = 1024;
	TCHAR res[1024] = TEXT("");
	HKEY key;


	if (RegOpenKeyEx(HKEY_CURRENT_USER, path, NULL, KEY_READ, &key) == ERROR_SUCCESS) {
		RegQueryValueEx(key, mykey, NULL, &type, (LPBYTE)&res[0], &size);
		RegCloseKey(key);
	}

	//A std:string  using the char* constructor.
	std::tstring ss(res);
	return ss;
}

int Register::getKeyAsInt(const TCHAR *path, const TCHAR *mykey, int defaultValue){
	std::tstring index = this->getKey(path, mykey);
	int value = _ttoi(index.c_str());
	if (value == 0){
		value = defaultValue;
	}
	return value;
}