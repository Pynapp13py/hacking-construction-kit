#pragma once

#include "plugin.h"

class ThePlugin: public Plugin {
public:
	ThePlugin();          // no members to set default variables
	~ThePlugin();         // no members to free

	void initialize();  // nothing to initialize
	void destroy();      // nothign to destroy
	void start(); // print a formatted string 
	void stop();

private:
	bool m_stopped;

};