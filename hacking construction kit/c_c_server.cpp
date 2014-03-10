/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "c_c_server.h"

#include "memory_debug.h"

CCServer const CCServer::m_instance = CCServer();

CCServer::CCServer(){
	m_servers = new std::string[MAX_SERVERS];
}

CCServer::~CCServer(){
	delete m_servers;
}

const CCServer &CCServer::get(){
	return m_instance;
}

void CCServer::add(const std::string &server){
	m_servers[m_index] = server;
	m_index++;
}

int CCServer::getCount() const {
	return m_index;
}

std::string* CCServer::getServers() const {
	return m_servers;
}

