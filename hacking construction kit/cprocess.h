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
#include <Windows.h>

#include "extern/MemoryModule.h"

// manage processes
class Process {

public:
	// get process pid from his name
	static DWORD getPid(const char *name);

	// kill this process
	static bool killProcess(DWORD pid);

	// inject this dll into this process
	// use a good old CreateRemoteThread with LoadLibrary
	// you have to use the full path to the dll
	static bool injectDllWithLoadLibrary(const char *fullpath, DWORD pid);

	// load in current process a dll from memory location (without using LoadLibrary)
	// this is just a wrapper to MemoryModule
	static HMEMORYMODULE memoryLoadLibrary(const void *memory); 

	// free previously loaded library
	static void memoryFreeLibrary(HMEMORYMODULE library); 


	// inject this dll into this process
	// this function doesn't use LoadLibrary in the remote process.
	// It'll load the dll and copy it to remote process, after performing relocation of adresses.
	// The dll must not use CRT library. It must also not use a library which is not present in remote process.
	// In fact, you'll probably have to use only kernel32.lib
	// The function called in the dll is the entrypoint (DllMain)
	static bool injectDll(const char *path, DWORD pid);

	// Inject dll from a handle
	static bool injectDll(HANDLE handle, DWORD pid);

	// Inject dll from a HMEMORYMODULE
	static bool injectDllFromMemory(HMEMORYMODULE handle, DWORD pid);

	static BOOL InjectProcess(DWORD dwPid, PVOID pvImageBase, DWORD dwImageSize, DWORD entryPoint);

	static BOOL ProcessRelocs(PVOID pvImageBase, DWORD dwDelta);

	static BOOL CopyImageToProcess(HANDLE hProcess, PVOID pvImageBase, DWORD dwImageSize, PVOID *ppRemoteMap);

};