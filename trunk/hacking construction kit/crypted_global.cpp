/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "crypted_global.h"

#include <stdio.h>

#include "base64.h"
#include "rc4.h"
#include "macro.h"

#include "memory_debug.h"

#define BUFFER_STRING_SIZE 100


CryptedGlobal const CryptedGlobal::m_instance = CryptedGlobal();



CryptedGlobal::CryptedGlobal(): m_strings(new std::vector<char*> ){
}

CryptedGlobal::~CryptedGlobal(){
	for (std::vector<char*>::iterator it = m_strings->begin() ; it != m_strings->end(); ++it){
		delete *it;
	}
	delete m_strings;
}

const CryptedGlobal &CryptedGlobal::get(){
	return m_instance;
}

char *CryptedGlobal::decodeNewString(const char *str){
	char buffer[BUFFER_STRING_SIZE];
	char *base64;
	char *key2 = KEY2;
	char *key1 = KEY1;
	size_t length;
	char *newString;
	Base64 base64encoder;

	MYPRINTF("decode string: %s\n", str);

	base64 = (char *)base64encoder.decode(str, strlen(str), &length);

	sprintf_s(buffer, BUFFER_STRING_SIZE, "%s%s", key1, key2);

	RC4 rc4(buffer);
	rc4.encode(base64, length);

	newString = new char[(length + 1)];
	strncpy_s(newString, length + 1, base64, length);
	newString[length] = '\0';

	CryptedGlobal::get().m_strings->push_back(newString);
	delete[] base64;

	MYPRINTF("string decoded: %s\n", newString);

	return newString;

}

