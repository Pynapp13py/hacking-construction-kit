/* ****************************************************************************
 * 
 * 				KeyPair.cpp
 * 
 * Author: Nedim Srndic
 * Release date: 22th of July 2008
 * 
 * This file contains the implementation for the KeyPair class. 
 * 
 * ****************************************************************************
 */
#include "KeyPair.h"

#include "..\memory_debug.h"

std::ostream &operator <<(std::ostream &cout, const KeyPair &k)
{
	return std::cout 
	<< "Private key:" << std::endl << k.GetPrivateKey() << std::endl
	<< "Public key:" << std::endl << k.GetPublicKey();
}
