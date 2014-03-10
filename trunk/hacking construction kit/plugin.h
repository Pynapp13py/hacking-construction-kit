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

