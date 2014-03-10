/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "smb.h"

#include <stdio.h>

#include <ws2tcpip.h>

#include "macro.h"

#include "memory_debug.h"

#define BUFFER_SIZE 256

bool Smb::connect(const char *target){
	NETRESOURCE	lpNetResource;
	char	RemoteName[BUFFER_SIZE] = {0};
	char	Username[BUFFER_SIZE] = {0};
	char	Password[BUFFER_SIZE] = {0};
	int res;

	sprintf_s(RemoteName, BUFFER_SIZE, "\\\\%s\\IPC$", target);

	lpNetResource.dwScope = RESOURCE_CONNECTED;
	lpNetResource.dwType = RESOURCETYPE_DISK;
	lpNetResource.dwDisplayType = RESOURCEDISPLAYTYPE_SHARE;
	lpNetResource.dwUsage = RESOURCEUSAGE_CONNECTABLE;
	lpNetResource.lpLocalName = NULL;
	lpNetResource.lpRemoteName = RemoteName;
	lpNetResource.lpComment = NULL;
	lpNetResource.lpProvider = NULL;

	res = WNetAddConnection2(&lpNetResource, Username, Password, CONNECT_UPDATE_PROFILE);
	if(res != 0) {
		MYPRINTF( "\t\tMake SMB Connection error:%d for %s\n", GetLastError(), RemoteName);
	}
	return res == 0;
}

bool Smb::closeConnection(const char *target){
	char	RemoteName[BUFFER_SIZE] = {0};
	DWORD status;

	if (srvsvc__MIDL_AutoBindHandle != 0){
		RpcBindingFree(&srvsvc__MIDL_AutoBindHandle);
		srvsvc__MIDL_AutoBindHandle = 0;
	}

	sprintf_s(RemoteName, BUFFER_SIZE, "\\\\%s\\IPC$", target);
	status = WNetCancelConnection2(
		RemoteName,
		0,
		TRUE
		);
	if (status != NO_ERROR){
		MYPRINTF( "\t\tWNetCancelConnection2 error:%d\n", status);
		return false;
	}
	return true;
}



bool Smb::bindToSpoolssPipe(const unsigned char* target){
	return Smb::bindToPipe(target, (unsigned char *)"\\PIPE\\SPOOLSS", spoolss__MIDL_AutoBindHandle);
}

bool Smb::bindToBrowserPipe(const unsigned char* target){
	return Smb::bindToPipe(target, (unsigned char *)"\\PIPE\\BROWSER", srvsvc__MIDL_AutoBindHandle);
}

bool Smb::bindToPipe(const unsigned char* target, unsigned char *pipe, RPC_BINDING_HANDLE handle){
	unsigned char * pszStringBinding = NULL;
	unsigned char * pszUuid = NULL;
	unsigned char * pszProtocolSequence = (unsigned char *)"ncacn_np";
	const unsigned char * pszNetworkAddress = target;
	unsigned char *	pszEndpoint = pipe;
	unsigned char * pszOptions = NULL;

	if (RpcStringBindingCompose(pszUuid, pszProtocolSequence, (RPC_CSTR)pszNetworkAddress, pszEndpoint, pszOptions, &pszStringBinding)){
		return false;
	}
	if(RpcBindingFromStringBinding(pszStringBinding, &handle) != RPC_S_OK)
	{
		RpcStringFree( &pszStringBinding );
		return false;
	}
	RpcStringFree( &pszStringBinding );
	return true;
}