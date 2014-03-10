/*
 * Hacking construction kit -- A GPL Windows security library.
 * While this library can potentially be used for nefarious purposes, 
 * it was written for educational and recreational
 * purposes only and the author does not endorse illegal use.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: thirdstormofcythraul@outlook.com
 */
#pragma once 

#define WIN32_LEAN_AND_MEAN
#include "windows.h"

// Must be defined
// hDll must be a handle to current module
extern HINSTANCE hDll;

// Start startService function is user enter correct password
void WINAPI ExecuteService(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow);

// Install this service
int WINAPI InstallAnotherService(const char *serviceName, const char *dllpath, char *description);

// Install current dll as service
int WINAPI InstallService(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow);

// Remove current service
DWORD WINAPI RemoveService(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow);

// Remove service and remove dll at next reboot
DWORD WINAPI KillService(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow);

// Handler for service
DWORD WINAPI HandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext);

// Main for service
VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
