/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "random.h"

#include <stdio.h>

#include <windows.h>
#include <time.h>

#include "global.h"
#include "macro.h"

#include "memory_debug.h"

const char *Random::internetMask = "%d.%d.%d.%d";
const char *Random::privateMask = "192.168.%d.%d";

Random random;

Random::Random(){
	srand((unsigned int)time(NULL));
}

int Random::createRandomIp(char *ip, int size, bool useOnlyPrivateAddress){
	if (useOnlyPrivateAddress){
		return createRandomIpPrivate(ip, size);
	}
	return createRandomIpPublic(ip, size);
}

int Random::createRandomIpPublic(char *ip, int size){
	std::string localIp;
	int ip1 = rand()%255;
	if (ip1 != 0 && ip1 != 127 && ip1 < 224 && ip1 != 10) {
		int ip2 = rand()%255;
		if (!(ip1 == 192 && ip2 == 168) && !(ip1 == 172 && ip2 > 16 && ip2 < 32)){
			int ip3 = rand()%255;
			int ip4 = rand()%253 + 1;
			sprintf_s(ip, size, Random::internetMask, ip1, ip2, ip3, ip4);
			localIp = Global::get().getInternetIp();
			if (strcmp(localIp.c_str(), ip) == 0){
				Sleep(1000);
				MYPRINTF("Try to attack its own ip\n");
				return createRandomIpPublic(ip, size);
			}
		} else {
			return createRandomIpPublic(ip, size);
		}
	} else {
		return createRandomIpPublic(ip, size);
	}
	return 0;
}

int Random::createRandomIpPrivate(char *ip, int size){
	std::string localIp;
	int ip1 = rand()%255;
	if (ip1 != 0) {
		int ip2 = rand()%253 + 1;
		sprintf_s(ip, size, Random::privateMask, ip1, ip2);
		localIp = Global::get().getPrivateIp();
		if (strcmp(localIp.c_str(), ip) == 0){
			Sleep(1000);
			MYPRINTF("Try to attack its own ip\n");
			return createRandomIpPrivate(ip, size);
		}
	} else {
		return createRandomIpPrivate(ip, size);
	}
	return 0;
}

std::string Random::createRandomName(int size){
	std::string res = "";
	static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < size; ++i) {
        res += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
	return res;
}

std::string Random::createRandomFileName(int size, std::string extension){
	return Random::createRandomName(size).append(extension);
}

void Random::createRandomName(int size, char *name){
	static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < size; ++i) {
        name[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    name[size] = 0;
}

void Random::createRandomName(int size, wchar_t *name){
	static const wchar_t alphanum[] =
        L"0123456789"
        L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        L"abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < size; ++i) {
        name[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    name[size] = 0;
}