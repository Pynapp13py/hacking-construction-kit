/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "Decoder.h"

#include "extern\RSA.h"
#include "extern\PrimeGenerator.h"
#include "macro.h"

#include "memory_debug.h"

std::string Decoder::modulus;

std::string Decoder::exponent;

std::string Decoder::rsaDecode(char *encoded){
	if (modulus.empty() || exponent.empty()){
		MYPRINTF("RSA error: modulus or exponent is empty");
		return "";
	}
	BigInt modulus(Decoder::modulus);
	BigInt exponent(Decoder::exponent);

	Key key(modulus, exponent);
	std::string decrypted;

	try {
		decrypted = RSA::Decrypt(encoded, key);
	} catch(char *){
		MYPRINTF("unable to decrypt %s\n", encoded);
		return decrypted;
	}
	return decrypted;
}

char * Decoder::simpleEncode(char *encodedTarget){
	size_t j;
	for(j = 0; j < strlen(encodedTarget); j++){
		char c = encodedTarget[j];
		if ((c >= 65 && c <= 122) || (c >= 48 && c <= 57)){
			encodedTarget[j] = c + 1;			
		}
	}
	return encodedTarget;
}

std::string Decoder::simpleEncode(std::string line){
	for(size_t i = 0; i < line.length(); ++i){
		char c = line[i];
		if ((c >= 65 && c <= 122) || (c >= 48 && c <= 57)){
			line[i] = c + 1;			
		}
	}
	return line;
}