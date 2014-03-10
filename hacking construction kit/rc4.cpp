/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "rc4.h"

#include <Windows.h>

#include "memory_debug.h"

RC4::RC4(char *key):
m_key(key) {
	
}

char* RC4::encode(char* lpBuf, DWORD dwBufLen){

	int a, b = 0, s[256];
	BYTE swap;
	DWORD dwCount;
	DWORD dwKeyLen = strlen(m_key);

	for(a = 0; a < 256; a++){
		s[a] = a;
	}

	for(a = 0; a < 256; a++){
		b = (b + s[a] + m_key[a % dwKeyLen]) % 256;
		swap = s[a];
		s[a] = s[b];
		s[b] = swap;
		b = 0;
	}

	a = 0;
	b = 0;

	for(dwCount = 0; dwCount < dwBufLen; dwCount++)	{

		a = (a + 1) % 256;
		b = (b + s[a]) % 256;
		swap = s[a];
		s[a] = s[b];
		s[b] = swap;
		lpBuf[dwCount] ^= s[(s[a] + s[b]) % 256];
	}

	return lpBuf;

}