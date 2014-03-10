#pragma once

#include <string>

// To create a plugin just derive this class
struct  Plugin
{
    virtual ~Plugin() { } // we intend on deriving from this, let’s call the destructor
                                                 // of our children!

    virtual void initialize() = 0;  // we’ll use this to initialize the object
    virtual void destroy() = 0;   // we’ll use this to destroy the object

    virtual void start() = 0; // 

	virtual void stop() = 0;
	
};