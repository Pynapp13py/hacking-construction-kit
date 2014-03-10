/*
 * Author: thirdstormofcythraul@outlook.com
 */
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <tchar.h>

#include <plugin_loader.h>
#pragma comment(lib, "hacking construction kit.lib")

#include <memory_debug.h>



int _tmain(int argc, _TCHAR* argv[]){

#if defined (WIN32) && defined (_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
#endif

	PluginLoader& plugin = PluginLoader::getInstance();
	plugin.loadAllPlugins();

	plugin.load("test_plug.dll");

	Sleep(5000);
	plugin.stop();
}

