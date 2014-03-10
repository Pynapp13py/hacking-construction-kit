/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "cprocess.h"

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#include "extern/ntdll.h"
#include "extern/utils.h"
#include "extern/MemoryModule.h"

#include "macro.h"

#pragma warning( disable : 4800 ) // stupid warning about bool

BOOL Process::InjectProcess(DWORD dwPid, PVOID pvImageBase, DWORD dwImageSize, DWORD entryPoint){
	BOOL bRet = FALSE;
	BOOLEAN bEnable;
	PVOID pMap;

	RtlAdjustPrivilege(SE_DEBUG_PRIVILEGE, TRUE, FALSE, &bEnable);

	if (dwPid) {
		HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD|PROCESS_QUERY_INFORMATION|PROCESS_VM_OPERATION|PROCESS_VM_WRITE|PROCESS_VM_READ, FALSE, dwPid);
		if (hProcess) {
			if (CopyImageToProcess(hProcess, pvImageBase, dwImageSize, &pMap))	{
				LPTHREAD_START_ROUTINE StartRoutine = (LPTHREAD_START_ROUTINE)(entryPoint + (DWORD)pMap);
				HANDLE hThread;

				hThread = CreateRemoteThread(hProcess, NULL, 0, StartRoutine, (PVOID)pMap, 0, NULL);
				if (hThread) {
					bRet = TRUE;
				}else{
					MYPRINTF(__FUNCTION__"(): CreateRemoteThread failed last error %x\n", GetLastError());
				}

				if (bRet){
					DWORD dwExitCode;

					if (WaitForSingleObject(hThread, 1*60*1000) == WAIT_OBJECT_0){
						if (GetExitCodeThread(hThread, &dwExitCode))	{
							bRet = dwExitCode == STATUS_SUCCESS;
						}
					}

					CloseHandle(hThread);
				}
			}

			CloseHandle(hProcess);
		}else{
			MYPRINTF(__FUNCTION__"(): OpenProcess failed last error %x\n", GetLastError());
		}
	}else{
		MYPRINTF(__FUNCTION__"(): Process not found\n");
	}

	return bRet;
}

HMEMORYMODULE Process::memoryLoadLibrary(const void *memory){
	return MemoryLoadLibrary(memory);
}

void Process::memoryFreeLibrary(HMEMORYMODULE library){
	MemoryFreeLibrary(library);
}

bool Process::injectDll(const char *path, DWORD pid){
	HANDLE dll = LoadLibrary(path);
	if (dll){			
		return Process::injectDll(dll, pid);
	}
	MYPRINTF("Unable to found dll\n");
	return false;
}

bool Process::injectDll(HANDLE dll, DWORD pid){

	MEMORY_BASIC_INFORMATION pMemInfo;

	VirtualQuery(dll, &pMemInfo, sizeof(pMemInfo));
	PVOID pvImageBase = pMemInfo.AllocationBase;
	PIMAGE_NT_HEADERS headers = RtlImageNtHeader(pMemInfo.AllocationBase);
	if (!headers){
		MYPRINTF("Unable to retrieve headers from dll\n");
		return false;
	}
	DWORD dwImageSize = headers->OptionalHeader.SizeOfImage;
				
	return Process::InjectProcess(pid, pvImageBase, dwImageSize, headers->OptionalHeader.AddressOfEntryPoint);
}

typedef struct {
    PIMAGE_NT_HEADERS headers;
    unsigned char *codeBase;
    HCUSTOMMODULE *modules;
    int numModules;
    int initialized;
    CustomLoadLibraryFunc loadLibrary;
    CustomGetProcAddressFunc getProcAddress;
    CustomFreeLibraryFunc freeLibrary;
    void *userdata;
} MEMORYMODULE, *PMEMORYMODULE;

bool Process::injectDllFromMemory(HMEMORYMODULE handle, DWORD pid){
	MEMORY_BASIC_INFORMATION pMemInfo;
	PMEMORYMODULE module = (PMEMORYMODULE)handle;

	VirtualQuery(module, &pMemInfo, sizeof(pMemInfo));
	PVOID pvImageBase = pMemInfo.AllocationBase;
	PIMAGE_NT_HEADERS headers = module->headers;
	if (!headers){
		MYPRINTF("Unable to retrieve headers from dll\n");
		return false;
	}
	DWORD dwImageSize = headers->OptionalHeader.SizeOfImage;
				
	return Process::InjectProcess(pid, pvImageBase, dwImageSize, headers->OptionalHeader.AddressOfEntryPoint);
}

BOOL Process::ProcessRelocs(PVOID pvImageBase, DWORD dwDelta){
	DWORD dwRelocsSize;
	PIMAGE_BASE_RELOCATION pReloc;

	if (dwDelta){
		pReloc = (PIMAGE_BASE_RELOCATION)RtlImageDirectoryEntryToData(pvImageBase, TRUE, IMAGE_DIRECTORY_ENTRY_BASERELOC, &dwRelocsSize);
		if (pReloc && dwRelocsSize)	{
			PIMAGE_BASE_RELOCATION pEndReloc = (PIMAGE_BASE_RELOCATION)(pReloc + dwRelocsSize);

			while (pReloc->SizeOfBlock && pReloc < pEndReloc){
				pReloc = LdrProcessRelocationBlock(MAKE_PTR(pvImageBase, pReloc->VirtualAddress, ULONG_PTR), (pReloc->SizeOfBlock - sizeof(*pReloc))/sizeof(USHORT), (PUSHORT)(pReloc + 1), dwDelta);
				if (!pReloc) return FALSE;
			}
		}
	}

	return TRUE;
}


BOOL Process::CopyImageToProcess(HANDLE hProcess, PVOID pvImageBase, DWORD dwImageSize, PVOID *ppRemoteMap){
	DWORD dwSize = dwImageSize;
	BOOL bResult = FALSE;
	NTSTATUS St;
	HANDLE hMapping;
	PVOID pCurrentMap = 0, pRemoteMap = 0;

	hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_EXECUTE_READWRITE|SEC_COMMIT, 0, dwImageSize, 0);
	if (hMapping != INVALID_HANDLE_VALUE)	{
		if (pCurrentMap = MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 0))		{
			CopyMemory(pCurrentMap, pvImageBase, dwImageSize);

			St = NtMapViewOfSection(hMapping, hProcess, &pRemoteMap, 0, 0, NULL, &dwSize, ViewUnmap, 0, PAGE_EXECUTE_READWRITE);
			if (NT_SUCCESS(St))	{
				if (bResult = Process::ProcessRelocs(pCurrentMap, (DWORD)pRemoteMap - (DWORD)pvImageBase))	{
					*ppRemoteMap = pRemoteMap;
				}else{
					MYPRINTF(__FUNCTION__"(): ProcessRelocs failed\n", St);
				}

				if (!bResult) NtUnmapViewOfSection(hProcess, pRemoteMap);
			}else{
				MYPRINTF(__FUNCTION__"(): NtMapViewOfSection failed with status %x\n", St);
			}
			

			UnmapViewOfFile(pCurrentMap);
		}

		CloseHandle(hMapping);
	}

	if (!bResult){
		if (pCurrentMap = VirtualAlloc(NULL, dwImageSize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE))	{
			CopyMemory(pCurrentMap, pvImageBase, dwImageSize);

			if (pRemoteMap = VirtualAllocEx(hProcess, NULL, dwImageSize, MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE)){
				if (Process::ProcessRelocs(pCurrentMap, (DWORD)pRemoteMap - (DWORD)pvImageBase))	{
					if (bResult = WriteProcessMemory(hProcess, pRemoteMap, pCurrentMap, dwImageSize, &dwSize))	{
						*ppRemoteMap = pRemoteMap;
					}else{
						MYPRINTF(__FUNCTION__"(): WriteProcessMemory failed with error %x\n", GetLastError());
					}
				}else{
					MYPRINTF(__FUNCTION__"(): ProcessRelocs failed\n", St);
				}

				if (!bResult) VirtualFreeEx(hProcess, pRemoteMap, 0, MEM_RELEASE);
			}else{
				MYPRINTF(__FUNCTION__"(): VirtualAllocEx failed with error %x\n", GetLastError());
			}

			VirtualFree(pCurrentMap, 0, MEM_RELEASE);
		}
	}


	return bResult;
}

bool Process::injectDllWithLoadLibrary(const char *path, DWORD pid){
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if(process == NULL) {
		MYPRINTF("Error: the specified process couldn't be found.\n");
	}

	LPVOID addr = (LPVOID)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryA");
	if(addr == NULL) {
		MYPRINTF("Error: the LoadLibraryA function was not found inside kernel32.dll library.\n");
	}

	LPVOID arg = (LPVOID)VirtualAllocEx(process, NULL, strlen(path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(arg == NULL) {
		MYPRINTF("Error: the memory could not be allocated inside the chosen process.\n");
	}

	int n = WriteProcessMemory(process, arg, path, strlen(path), NULL);
	if(n == 0) {
		MYPRINTF("Error: there was no bytes written to the process's address space.\n");
	}

	HANDLE threadID = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)addr, arg, NULL, NULL);
	if(threadID == NULL) {
		MYPRINTF("Error: the remote thread could not be created.\n");
	}
	else {
		MYPRINTF("Success: the remote thread was successfully created.\n");
	}

	CloseHandle(process);

	return true;
}  

bool Process::killProcess(DWORD pid)
{
	if (!pid){
		return false;
	}
	DWORD ExitCode;
	HANDLE hp;
	BOOL ret = TRUE;

	if(pid) {
		hp = OpenProcess(PROCESS_ALL_ACCESS, true, pid);
		if (hp) {
			GetExitCodeProcess(hp, &ExitCode);
			ret = TerminateProcess(hp, ExitCode);
		} else {
			return false;
		}
	} else {
		return false;
	}

	CloseHandle(hp);
	return ret == TRUE;
}

DWORD Process::getPid(const char *name){
	DWORD ids[1024] = {0};
	DWORD dwReturned = 0;
	EnumProcesses(ids, sizeof(ids), &dwReturned);

	for(DWORD i = 0; i < (dwReturned / sizeof(DWORD)); i++){
		HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ids[i]);
		if(hp) {
			TCHAR fname[MAX_PATH] = TEXT("");
			GetProcessImageFileName(hp, fname, sizeof(fname)/sizeof(WCHAR) - 1);
			if (strstr(fname, name) != 0){
				CloseHandle(hp);
				return ids[i];
			}			
		}
	}
	return 0;
}
