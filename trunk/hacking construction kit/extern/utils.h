#ifndef _UTILSU_H_
#define _UTILSU_H_

#include "ntdll.h"

#ifdef _DEBUG
VOID  __cdecl DbgMsg(PCHAR PrintFormat, ...);
#else
#define DbgMsg(...)
#endif

PVOID __cdecl malloc(size_t Size);
VOID __cdecl free(PVOID Pointer);
PVOID __cdecl realloc(PVOID Pointer, size_t NewSize);

VOID ThreadCreate(PVOID pvFunc, PVOID pvParam, PHANDLE phHandle);
VOID DeleteFileReboot(PCHAR FilePath);
VOID GetParrentProcessName(PCHAR FileName, DWORD Size);
BOOLEAN SetFileDllFlag(PCHAR FileName);
DWORD GetProcessIdByName(PCHAR ProcessName);
BOOL CheckUAC();
PVOID MapBinary(PCHAR lpPath, DWORD dwFileAccess, DWORD dwFileFlags, DWORD dwPageAccess, DWORD dwMapAccess, PDWORD pdwSize);
PVOID GetSystemInformation(SYSTEM_INFORMATION_CLASS dwInfoClass);
LPWSTR GetObjectInfo(HANDLE hObject, OBJECT_INFORMATION_CLASS objInfoClass);


#define ALIGN_DOWN(x, align)(x &~ (align - 1))
#define ALIGN_UP(x, align)((x & (align - 1))?ALIGN_DOWN(x, align) + align:x)
#define RtlOffsetToPointer(B,O) ((PCHAR)(((PCHAR)(B)) + ((ULONG_PTR)(O))))
#define RtlPointerToOffset(B,P) ((ULONG)(((PCHAR)(P)) - ((PCHAR)(B))))
#define FlagOn(x, f) ((x) & (f))
#define MAKE_PTR(B, O, T) ((T)RtlOffsetToPointer(B, O))


#define RVA_TO_VA(B,O) ((PCHAR)(((PCHAR)(B)) + ((ULONG_PTR)(O))))
#define VA_TO_RVA(B,P) ((ULONG)(((PCHAR)(P)) - ((PCHAR)(B))))



#endif