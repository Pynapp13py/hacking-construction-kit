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

#include <string>

#define MAX_SERVERS 10

// Manage C&C servers names/ips
// Singleton
class CCServer {
public:
	// Get instance
	static const CCServer& get();

	~CCServer();

	// Add C&C server to the list
	void add(const std::string &server);

	// count servers
	int getCount() const;

	// Get all C&C servers names
	std::string *getServers() const;

private:
	CCServer();

	int m_index;
	std::string * m_servers;

	static const CCServer m_instance;
};