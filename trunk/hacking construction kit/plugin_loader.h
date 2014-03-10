/*
 * Hacking construction kit -- A GPL Windows security library.
 * While this library can potentially be used for nefarious purposes, 
 * it was written for educational and recreational
 * purposes only and the author does not endorse illegal use.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: thirdstormofcythraul@outlook.com
 */
#pragma once

#include <Windows.h>

#include <vector>

// A plugin
struct Plugin;

// PluginLoader for command manager
// This class will search in registry for dlls to be loaded
// path in registry: SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Plugs
// singleton
class PluginLoader {
public:
	~PluginLoader();

	// Load this dll in a different thread
	void load(const char *dllPath);

	// Load all dlls found in registry
	void loadAllPlugins();

	// Stop all threads
	void stop();

	// Thread used to start dll
	static DWORD WINAPI startThread(LPVOID params);

	// Get PluginLoader instance
	static PluginLoader& getInstance();

	// Vector of plugins
	static std::vector<Plugin*> plugs;

protected:
	PluginLoader();

private:
	static PluginLoader instance;

	std::vector<HANDLE> m_handles;
	

};