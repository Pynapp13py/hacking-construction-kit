#include <intrin.h>
#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>
#include <psapi.h>
#include <imagehlp.h>
#include <tlhelp32.h>

#include "utils.h"


#ifdef _DEBUG

VOID __cdecl DbgMsg(PCHAR PrintFormat, ...)
{
	va_list VaList;
	CHAR FormatBuffer[1024];
	CHAR ModulePath[MAX_PATH];
	CHAR DebugMessage[1024*4];

	GetModuleFileName(NULL, ModulePath, RTL_NUMBER_OF(ModulePath)-1);
	_snprintf(FormatBuffer, RTL_NUMBER_OF(FormatBuffer)-1, "[%s] %s", PathFindFileName(ModulePath), PrintFormat);

	va_start(VaList, PrintFormat);
	_vsnprintf(DebugMessage, RTL_NUMBER_OF(DebugMessage)-1, FormatBuffer, VaList);
	va_end(VaList);

	OutputDebugString(DebugMessage);
}

#endif

PVOID __cdecl malloc(size_t Size)
{
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, Size);
}

VOID __cdecl free(PVOID Pointer)
{
	HeapFree(GetProcessHeap(), 0, Pointer);
}

PVOID __cdecl realloc(PVOID Pointer, size_t NewSize)
{
	return HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, Pointer, NewSize);
}

PVOID __cdecl operator new(size_t Size)
{
	return malloc(Size);
}

VOID __cdecl operator delete(PVOID Pointer)
{
	free(Pointer);
}

int __cdecl _purecall()
{
	return 0;
}

VOID ThreadCreate(PVOID pvFunc, PVOID pvParam, PHANDLE phHandle)
{
	HANDLE hThread;
	
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pvFunc, pvParam, 0, NULL);
	if (hThread) 
	{
		if (phHandle) *phHandle = hThread;

		CloseHandle(hThread);
	}
	else
	{
		DbgMsg(__FUNCTION__"(): CreateThread: 0x%x\n", GetLastError());
	}
}

VOID GetParrentProcessName(PCHAR FileName, DWORD Size)
{
	PROCESS_BASIC_INFORMATION ProcessInfo;
	HANDLE ProcessHandle;
	DWORD Length;

	ZeroMemory(FileName, Size);
	if (NT_SUCCESS(NtQueryInformationProcess(NtCurrentProcess(), ProcessBasicInformation, &ProcessInfo, sizeof(ProcessInfo), &Length)))
	{
		ProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, ProcessInfo.InheritedFromUniqueProcessId);
		if (ProcessHandle != INVALID_HANDLE_VALUE)
		{
			GetModuleFileNameEx(ProcessHandle, NULL, FileName, Size);

			CloseHandle(ProcessHandle);
		}
	}
}

VOID DeleteFileReboot(PCHAR FilePath)
{
	CHAR TempPath[MAX_PATH];
	CHAR TempName[MAX_PATH];

	GetTempPath(RTL_NUMBER_OF(TempPath), TempPath);
	GetTempFileName(TempPath, NULL, 0, TempName);

	if (!MoveFileEx(FilePath, TempName, MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH))
	{
		DbgMsg(__FUNCTION__"(): MoveFileEx: 0x%x\n", GetLastError());
	}

	if (!MoveFileEx(TempName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT|MOVEFILE_WRITE_THROUGH))
	{
		DbgMsg(__FUNCTION__"(): MoveFileEx: 0x%x\n", GetLastError());
	}
}

PVOID MapBinary(PCHAR lpPath, DWORD dwFileAccess, DWORD dwFileFlags, DWORD dwPageAccess, DWORD dwMapAccess, PDWORD pdwSize)
{
	PVOID pMap = NULL;
	HANDLE hMapping;
	HANDLE hFile;

	hFile = CreateFile(lpPath, dwFileAccess, FILE_SHARE_READ, NULL, OPEN_EXISTING, dwFileFlags, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		hMapping = CreateFileMappingA(hFile, NULL, dwPageAccess, 0, 0, NULL);
		if (hMapping != INVALID_HANDLE_VALUE)
		{
			pMap = MapViewOfFile(hMapping, dwMapAccess, 0, 0, 0);
			if (!pMap)
			{
				DbgMsg(__FUNCTION__"(): MapViewOfFile: 0x%x\n", GetLastError());
			}
			else 
			{
				if (pdwSize) *pdwSize = GetFileSize(hFile, NULL);
			}

			CloseHandle(hMapping);
		}
		else
		{
			DbgMsg(__FUNCTION__"(): CreateFileMapping: 0x%x\n", GetLastError());
		}

		CloseHandle(hFile);
	}
	else
	{
		DbgMsg(__FUNCTION__"(): CreateFile: 0x%x\n", GetLastError());
	}

	return pMap;
}

BOOLEAN SetFileDllFlag(PCHAR FileName)
{
	BOOL bRet = FALSE;
	PVOID pMap;
	PIMAGE_NT_HEADERS pNtHdr;
	DWORD dwFileSize;

	if (pMap = MapBinary(FileName, FILE_ALL_ACCESS, FILE_FLAG_WRITE_THROUGH, PAGE_READWRITE, FILE_MAP_ALL_ACCESS, &dwFileSize))
	{
		if (pNtHdr = RtlImageNtHeader(pMap))
		{
			PIMAGE_FILE_HEADER pFileHdr = &pNtHdr->FileHeader;
			DWORD HeaderSum, CheckSum;

			pFileHdr->Characteristics |= IMAGE_FILE_DLL;

			if (CheckSumMappedFile(pMap, dwFileSize, &HeaderSum, &CheckSum))
			{
				pNtHdr->OptionalHeader.CheckSum = CheckSum;

				bRet = TRUE;
			}
		}

		FlushViewOfFile(pMap, dwFileSize);
		UnmapViewOfFile(pMap);
	}

	return bRet;
}

DWORD GetProcessIdByName(PCHAR ProcessName)
{
	DWORD Result = NULL;
	PROCESSENTRY32 ProcessEntry = {0};
	HANDLE hSnap;

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		ProcessEntry.dwSize = sizeof(ProcessEntry);
		if (Process32First(hSnap, &ProcessEntry))
		{
			do 
			{
				if (!lstrcmp(ProcessName, ProcessEntry.szExeFile))
				{
						Result = ProcessEntry.th32ProcessID;

					break;
				}
			}
			while (Process32Next(hSnap, &ProcessEntry));
		}

		CloseHandle(hSnap);
	}

	return Result;
}

BOOL CheckUAC()
{
	BOOL fIsElevated = FALSE;
	HANDLE hToken = NULL;

	if (OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hToken))
	{
		TOKEN_ELEVATION elevation;
		DWORD dwSize;

		if (GetTokenInformation(hToken,TokenElevation,&elevation,sizeof(elevation),&dwSize))
		{
			fIsElevated = !elevation.TokenIsElevated;
		}

		CloseHandle(hToken);
	}

	return fIsElevated;
}


PVOID GetSystemInformation(SYSTEM_INFORMATION_CLASS dwInfoClass)
{
	NTSTATUS St;
	DWORD dwSize = 1000;
	PVOID pvInfo = NULL;

	do
	{
		pvInfo = malloc(dwSize);
		if (!pvInfo) return NULL;

		St = NtQuerySystemInformation(dwInfoClass, pvInfo, dwSize, NULL); 
		if (!NT_SUCCESS(St))
		{
			free(pvInfo);

			dwSize += 1000;
			pvInfo = NULL;
		}
	} 
	while (St == STATUS_INFO_LENGTH_MISMATCH);

	return pvInfo;
}


LPWSTR GetObjectInfo(HANDLE hObject, OBJECT_INFORMATION_CLASS objInfoClass)
{
	LPWSTR lpwsReturn = NULL;
	DWORD dwSize = sizeof(OBJECT_NAME_INFORMATION);
	POBJECT_NAME_INFORMATION pObjectInfo = (POBJECT_NAME_INFORMATION)malloc(dwSize);

	NTSTATUS ntReturn = NtQueryObject(hObject, objInfoClass, pObjectInfo, dwSize, &dwSize);
	if((ntReturn == STATUS_BUFFER_OVERFLOW) || (ntReturn == STATUS_INFO_LENGTH_MISMATCH))
	{
		free(pObjectInfo);
		pObjectInfo = (POBJECT_NAME_INFORMATION)malloc(dwSize);
		ntReturn = NtQueryObject(hObject, objInfoClass, pObjectInfo, dwSize, &dwSize);
	}
	if((ntReturn >= STATUS_SUCCESS) && (pObjectInfo->Name.Buffer != NULL))
	{
		lpwsReturn = (LPWSTR)malloc(pObjectInfo->Name.Length + sizeof(WCHAR));
		ZeroMemory(lpwsReturn, pObjectInfo->Name.Length + sizeof(WCHAR));
		CopyMemory(lpwsReturn, pObjectInfo->Name.Buffer, pObjectInfo->Name.Length);

		free(pObjectInfo);
	}

	return lpwsReturn;
}