/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "info.h"

#include <sstream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinBase.h>
#include <Sddl.h>
#include <winsock2.h>
#include <Winnls.h>
#include <ws2tcpip.h>


#include <strsafe.h>
#include <assert.h>
#include <lm.h>
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "Ws2_32.lib")

#include "macro.h"
#include "decoder.h"

#include "memory_debug.h"

std::string Info::getSid(){
	DWORD sidSize = 0;
	TCHAR domain[1024] = {0};
	DWORD domainSize = 1024;
	SID_NAME_USE accountType;
	LPTSTR cStringSid;

	LPTSTR accountName = "";
	// First we need the size of SID buffer
	LookupAccountName(0, accountName, 0, &sidSize, domain, &domainSize, &accountType);
	int *sid = new int[sidSize];
	bool res = LookupAccountName(0, accountName, sid, &sidSize, domain, &domainSize, &accountType) != 0;
	if (!res || !ConvertSidToStringSid(sid, &cStringSid)){
		return "";
	}
	return cStringSid;
}

std::string Info::getNameFromSid(const std::string sid){
	char lpName[100];
    char lpDomain[100];
    PSID pSID = NULL;
	DWORD dwSize = 100, dwResult = 0;
	SID_NAME_USE SidType;
	if (ConvertStringSidToSid(sid.c_str(), &pSID) == 0){
		MYPRINTF("Error in ConvertStringSidToSid: %d", GetLastError());
	}
	if (LookupAccountSid(0, pSID, lpName, &dwSize, lpDomain, &dwSize, &SidType) == 0){
		int error = GetLastError();
		if (error == ERROR_NONE_MAPPED){
			MYPRINTF("Error in LookupAccountSid: not mapped");
		} else {
			MYPRINTF("Error in LookupAccountSid: %d", GetLastError());
		}
	}
	FreeSid(pSID);
	
	return lpName;
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

std::string Info::getModulePath(){
	CHAR   dllPath[512] = {0};

	GetModuleFileName((HINSTANCE)&__ImageBase, dllPath, _countof(dllPath));
	return dllPath;
}

std::string Info::getModuleName(){
	char fileName[MAX_PATH];
	char fileNamePath [MAX_PATH];

	GetModuleFileName((HINSTANCE)&__ImageBase, fileNamePath, MAX_PATH);
	char *pch = strrchr(fileNamePath,'\\');

	size_t length = strlen(pch);
	for(size_t i = 0; i < length; i++)
	{
		*pch++;
		fileName[i] = *pch;
	}

	std::string res(fileName);
	return res;
}


bool Info::getUniqueId(DWORD& lpVolumeSerialNumber){
	LPCTSTR lpRootPathName = 0;
	LPTSTR lpVolumeNameBuffer = 0;
	DWORD nVolumeNameSize = 0;
	DWORD lpMaximumComponentLength = 1;
	DWORD lpFileSystemFlags = 1;
	LPTSTR lpFileSystemNameBuffer = 0;
	DWORD nFileSystemNameSize = 1;

	BOOL res = GetVolumeInformation(lpRootPathName,
		lpVolumeNameBuffer,
		nVolumeNameSize,
		&lpVolumeSerialNumber,
		&lpMaximumComponentLength,
		&lpFileSystemFlags,
		lpFileSystemNameBuffer,
		nFileSystemNameSize
		);

	if(!res) {
		MYPRINTF("unable to retrieve volume information\n");
		return false;
	}

	//MYPRINTF("lpVolumeSerialNumber: %d\n", lpVolumeSerialNumber);
	return true;
}


std::string Info::getAllIps(){

	DWORD dwRetval;

	ADDRINFOT *ptr = NULL;
	ADDRINFOT hints;
	ADDRINFOT *result = NULL;

	sockaddr_in  *sockaddr_ipv4;

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;

	TCHAR buffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
	if (GetComputerName(buffer, &size) == 0){
		MYPRINTF("GetComputerName error: %d", WSAGetLastError());
		return "";
	}
	dwRetval = GetAddrInfo(buffer, 0, &hints, &result);
	if ( dwRetval != 0 ) {
		MYPRINTF("GetAddrInfo error: %d", WSAGetLastError());
		return "";
	}

	std::string ips;
	std::string newIp;
	// Retrieve each address and print out the hex bytes
	for(ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		switch (ptr->ai_family) {
		case AF_INET:
			sockaddr_ipv4 = (sockaddr_in *) ptr->ai_addr;
			newIp =  inet_ntoa(sockaddr_ipv4->sin_addr);
			ips = ips.append(newIp).append(" ");
			break;
		default:
			break;
		}
	}

	FreeAddrInfo(result);
	if (!ips.empty()) {
		ips.pop_back();
	}
	return ips;
}

std::tstring Info::getWindowsLanguage(){
	TCHAR szLang[256];
	if (GetLocaleInfo(LANG_SYSTEM_DEFAULT, LOCALE_SABBREVLANGNAME, szLang, 256) == 0){
		MYPRINTF("GetLocaleInfo failed: %d\n", GetLastError());
	}
	return szLang;
}


std::tstring Info::getAllUsers(){
	LPUSER_INFO_0 pBuf = NULL;
	LPUSER_INFO_0 pTmpBuf;
	DWORD dwLevel = 0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;

	std::wstring res;

	//
	// Call the NetUserEnum function, specifying level 0;
	//   enumerate global user account types only.
	//
	do {
		nStatus = NetUserEnum((LPCWSTR) 0,
			dwLevel,
			FILTER_NORMAL_ACCOUNT     , // global users
			(LPBYTE*)&pBuf,
			dwPrefMaxLen,
			&dwEntriesRead,
			&dwTotalEntries,
			&dwResumeHandle);
		//
		// If the call succeeds,
		//
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) {
			if ((pTmpBuf = pBuf) != NULL) {
				//
				// Loop through the entries.
				//
				for (i = 0; (i < dwEntriesRead); i++){
					assert(pTmpBuf != NULL);

					if (pTmpBuf == NULL) {
						MYPRINTF("An access violation has occurred\n");
						break;
					}

					std::wstring temp = pTmpBuf->usri0_name;
					res = res.append(temp).append(L" ");

					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}
		//
		// Otherwise, print the system error.
		//
		else {
			MYPRINTF("A system error has occurred: %d\n", nStatus);
		}
		//
		// Free the allocated buffer.
		//
		if (pBuf != NULL) {
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	}
	// Continue to call NetUserEnum while
	//  there are more entries.
	//
	while (nStatus == ERROR_MORE_DATA); // end do
	//
	// Check again for allocated memory.
	//
	if (pBuf != NULL) {
		NetApiBufferFree(pBuf);
	}
	if (!res.empty()) {
		res.pop_back();
	}
	std::string str(res.begin(), res.end());
	return str;
}

DWORD Info::getUptime(){
	return GetTickCount();
}

bool Info::getComputerName(TCHAR *buffer, DWORD &size){
	if (GetComputerName(buffer, &size) == 0){
		MYPRINTF("GetComputerName error: %d", WSAGetLastError());
		return false;
	}
	return true;
}

#define BUFSIZE 256

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

bool Info::getOSDisplayString( LPTSTR pszOS){
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	PGPI pGPI;
	BOOL bOsVersionInfoEx;
	DWORD dwType;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);

	if(bOsVersionInfoEx == NULL ) return false;

	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

	pGNSI = (PGNSI) GetProcAddress(
		GetModuleHandle(TEXT("kernel32.dll")),
		"GetNativeSystemInfo");
	if(NULL != pGNSI)
		pGNSI(&si);
	else GetSystemInfo(&si);

	if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId &&
		osvi.dwMajorVersion > 4 ) {
		StringCchCopy(pszOS, BUFSIZE, TEXT(""));

		// Test for the specific product.

		if ( osvi.dwMajorVersion == 6 )	{
			if( osvi.dwMinorVersion == 0 )	{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					StringCchCat(pszOS, BUFSIZE, TEXT("W Vista "));
				else StringCchCat(pszOS, BUFSIZE, TEXT("W Server 2008 " ));
			}

			if ( osvi.dwMinorVersion == 1 )	{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					StringCchCat(pszOS, BUFSIZE, TEXT("W 7 "));
				else StringCchCat(pszOS, BUFSIZE, TEXT("W Server 2008 R2 " ));
			}

			if ( osvi.dwMinorVersion == 2 )	{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					StringCchCat(pszOS, BUFSIZE, TEXT("W 8 "));
				else StringCchCat(pszOS, BUFSIZE, TEXT("W Server 2012 " ));
			}

			pGPI = (PGPI) GetProcAddress(
				GetModuleHandle(TEXT("kernel32.dll")),
				"GetProductInfo");

			pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

			switch( dwType ){
			case PRODUCT_ULTIMATE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Ultimate" ));
				break;
			case PRODUCT_PROFESSIONAL:
				StringCchCat(pszOS, BUFSIZE, TEXT("Pro" ));
				break;
			case PRODUCT_HOME_PREMIUM:
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Premium" ));
				break;
			case PRODUCT_HOME_BASIC:
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Basic" ));
				break;
			case PRODUCT_ENTERPRISE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise" ));
				break;
			case PRODUCT_BUSINESS:
				StringCchCat(pszOS, BUFSIZE, TEXT("Business" ));
				break;
			case PRODUCT_STARTER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Starter" ));
				break;
			case PRODUCT_CLUSTER_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Cluster Server" ));
				break;
			case PRODUCT_DATACENTER_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter" ));
				break;
			case PRODUCT_DATACENTER_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter (core installation)" ));
				break;
			case PRODUCT_ENTERPRISE_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise" ));
				break;
			case PRODUCT_ENTERPRISE_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise (core installation)" ));
				break;
			case PRODUCT_ENTERPRISE_SERVER_IA64:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise for Itanium-based Systems" ));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server" ));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
				StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server Premium" ));
				break;
			case PRODUCT_STANDARD_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Standard" ));
				break;
			case PRODUCT_STANDARD_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Standard (core installation)" ));
				break;
			case PRODUCT_WEB_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Web Server" ));
				break;
			}
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )	{
			if( GetSystemMetrics(SM_SERVERR2) )
				StringCchCat(pszOS, BUFSIZE, TEXT( "W Server 2003 R2, "));
			else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
				StringCchCat(pszOS, BUFSIZE, TEXT( "W Storage Server 2003"));
			else if ( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
				StringCchCat(pszOS, BUFSIZE, TEXT( "W Home Server"));
			else if( osvi.wProductType == VER_NT_WORKSTATION &&
				si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64) {
				StringCchCat(pszOS, BUFSIZE, TEXT( "W XP Pro x64"));
			} else StringCchCat(pszOS, BUFSIZE, TEXT("W Server 2003, "));

			// Test for the server type.
			if ( osvi.wProductType != VER_NT_WORKSTATION )	{
				if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 ) {
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter for Itanium-based Systems" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise for Itanium-based Systems" ));
				}

				else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )	{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter x64" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise x64" ));
					else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard x64" ));
				}

				else {
					if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Compute Cluster" ));
					else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise" ));
					else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Web" ));
					else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard" ));
				}
			}
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )	{
			StringCchCat(pszOS, BUFSIZE, TEXT("W XP "));
			if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Home" ));
			else StringCchCat(pszOS, BUFSIZE, TEXT( "Pro" ));
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )	{
			StringCchCat(pszOS, BUFSIZE, TEXT("W 2000 "));

			if ( osvi.wProductType == VER_NT_WORKSTATION )	{
				StringCchCat(pszOS, BUFSIZE, TEXT( "Pro" ));
			}
			else {
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Server" ));
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Advanced Server" ));
				else StringCchCat(pszOS, BUFSIZE, TEXT( "Server" ));
			}
		}

		// Include service pack (if any) and build number.

		if( strlen(osvi.szCSDVersion) > 0 )	{
			StringCchCat(pszOS, BUFSIZE, TEXT(" ") );
			StringCchCat(pszOS, BUFSIZE, osvi.szCSDVersion);
		}

		TCHAR buf[80];

		StringCchPrintf( buf, 80, TEXT(" (build %d)"), osvi.dwBuildNumber);
		StringCchCat(pszOS, BUFSIZE, buf);

		if ( osvi.dwMajorVersion >= 6 )	{
			if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				StringCchCat(pszOS, BUFSIZE, TEXT( ", 64-bit" ));
			else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
				StringCchCat(pszOS, BUFSIZE, TEXT(", 32-bit"));
		}

		return true;
	}

	else {
		MYPRINTF( "This sample does not support this version of Windows.\n");
		return false;
	}
}

std::tstring Info::getVersion(){

	OSVERSIONINFO osvi;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	TCHAR buffer[256];
	sprintf_s(buffer, 256, TEXT("%u.%u.%u.%s"), osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber, osvi.szCSDVersion);

	std::tstring s = buffer;

	return s;
}


std::tstring Info::getProcessor(){
	SYSTEM_INFO systemInfo;
	ZeroMemory(&systemInfo, sizeof(SYSTEM_INFO));
	PGNSI pGNSI;
	pGNSI = (PGNSI) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
	if(NULL != pGNSI) {
		pGNSI(&systemInfo);
	} else {
		GetSystemInfo(&systemInfo);
	}
	TCHAR buffer[256];
	sprintf_s(buffer, 256, TEXT("%s %u"), systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ? "64" : "32", systemInfo.dwNumberOfProcessors);

	std::tstring s = buffer;

	return s;
}

std::string Info::getAllInfos(){
    TCHAR buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

	std::string res;

    if (getComputerName(buffer, size)){
		res.append("c=").append(Decoder::simpleEncode(buffer));
    } else {
        MYPRINTF("Unable to retrieve computer name\n");
    }

	res.append("&o=").append(Decoder::simpleEncode(getVersion()));

    TCHAR os[256];
    if (getOSDisplayString(os)){
		res.append("&s=").append(Decoder::simpleEncode(os));
    }

	res.append("&p=").append(Decoder::simpleEncode(getProcessor()));

	res.append("&u=").append(Decoder::simpleEncode(getAllUsers()));

	std::ostringstream oss;
	oss << getUptime() / (1000 * 60);
	res.append("&t=").append(oss.str());

	res.append("&a=").append(Decoder::simpleEncode(getAllIps()));

	res.append("&l=").append(Decoder::simpleEncode(getWindowsLanguage()));

	return res;
}

