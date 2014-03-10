/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "finger_print.h"

#include "smb.h"
#include "generated\spoolss.h"
#include "macro.h"

#include "memory_debug.h"

const char *FingerPrint::searchInBuffer(const char *haystack, size_t haystacklen, const char *needle, size_t needlelen)
{
	int searchlen = haystacklen - needlelen + 1;
	for ( ; searchlen-- > 0; haystack++)
		if (!memcmp(haystack, needle, needlelen))
			return haystack;
	return NULL;
}

int FingerPrint::fingerTest(const unsigned char *buffer, int count, wchar_t* search){
	return searchInBuffer((char *)buffer, count, (char *)search, wcslen(search)) != NULL;
}

FingerPrint::TARGETS FingerPrint::parseBuffer(const unsigned char *buffer, int count){
	wchar_t wSwedish[] = {0x46,0x6a,0xe4,0x72,0x72,0x73,0x6b,0x72,0x69,0x76,0x61,0x72,0x65,0x00};
	wchar_t wCzech[] = {0x56,0x7a,0x64,0xe1,0x6c,0x65,0x6e,0xe9,0x20,0x74,0x69,0x73,0x6b,0xe1,0x72,0x6e,0x79,0x00};
	wchar_t wFinnish[] = {0x45,0x74,0xe4,0x74,0x75,0x6c,0x6f,0x73,0x74,0x69,0x6d,0x65,0x74,0x00};
	wchar_t wHungarian[] = {0x54,0xe1,0x76,0x6f,0x6c,0x69,0x20,0x6e,0x79,0x6f,0x6d,0x74,0x61,0x74,0xf3,0x6b,0x00};


	char cTurkish[] = {0x59,0x00,0x61,0x00,0x7a,0x00,0x31,0x01,0x63,0x00,0x31,0x01,0x6c,0x00,0x61,0x00,0x72,0x00,0x00,0x00};
	wchar_t *wTurkish = (wchar_t *)cTurkish;
	unsigned char cJapanese[] = {0xea,0x30,0xe2,0x30,0xfc,0x30,0xc8,0x30,0x20,0x00,0xd7,0x30,0xea,0x30,0xf3,0x30,0xbf,0x30,0x00,0x00};
	wchar_t *wJapanese = (wchar_t *)cJapanese;
	unsigned char cTraditional[] = {0xdc,0x8f,0x0b,0x7a,0x53,0x62,0x70,0x53,0x3a,0x67,0x00,0x00};
	wchar_t *wTraditional = (wchar_t *)cTraditional;
	unsigned char cTaiwan[] = {0x60,0x90,0xef,0x7a,0x70,0x53,0x68,0x88,0x5f,0x6a,0x00,0x00};
	wchar_t *wTaiwan = (wchar_t *)cTaiwan;
	unsigned char cKorean[] = {0xd0,0xc6,0xa9,0xac,0x20,0x00,0x04,0xd5,0xb0,0xb9,0x30,0xd1,0x00,0x00};
	wchar_t *wKorean = (wchar_t *)cKorean;
	char cRussian[] = {0x1f,0x04,0x40,0x04,0x38,0x04,0x3d,0x04,0x42,0x04,0x35,0x04,0x40,0x04,0x4b,0x04,0x20,0x00,0x43,0x04,0x34,0x04,0x30,0x04,0x3b,0x04,0x35,0x04,0x3d,0x04,0x3d,0x04,0x3e,0x04,0x33,0x04,0x3e,0x04,0x20,0x00,0x34,0x04,0x3e,0x04,0x41,0x04,0x42,0x04,0x43,0x04,0x3f,0x04,0x30,0x04,0x00,0x00};
	wchar_t *wRussian = (wchar_t *)cRussian;

	if (fingerTest(buffer, count, L"Windows NT Remote Printers") ||
		fingerTest(buffer, count, L"LanMan Print Services")) {
			MYPRINTF( "\t\tEnglish\n");
			return English;
	}

	if (fingerTest(buffer, count, L"Impresoras remotas Windows NT") ||
		fingerTest(buffer, count, L"Impresoras remotas de Windows NT")) {
			MYPRINTF( "\t\tSpanish\n");
			return Spanish;
	}

	if (fingerTest(buffer, count, L"Stampanti remote di Windows NT")||
		fingerTest(buffer, count, L"Servizi di stampa LanMan")){
			MYPRINTF( "\t\tItalian\n");
			return Italian;
	}

	if (fingerTest(buffer, count, L"Imprimantes distantes NT")||
		fingerTest(buffer, count, L"Imprimantes distantes pour Windows NT")||
		fingerTest(buffer, count, L"Services d'impression LanMan")){
			MYPRINTF( "\t\tFrench\n");
			return French;
	}

	if (fingerTest(buffer, count, L"Remotedrucker")){
		MYPRINTF( "\t\tGerman\n");
		return German;
	}

	if (fingerTest(buffer, count, L"Impr. remotas Windows NT")||
		fingerTest(buffer, count, L"Impressoras remotas do Windows NT")){
			MYPRINTF( "\t\tPortuguese - Brazilian\n");
			return Brazilian;
	}

	if (fingerTest(buffer, count, L"Imp. remotas do Windows NT")){
		MYPRINTF( "\t\tPortuguese\n");
		return Portuguese;
	}

	if (fingerTest(buffer, count, wHungarian)){
		MYPRINTF( "\t\tHungarian\n");
		return Hungarian;
	}

	if (fingerTest(buffer, count, wFinnish)){
		MYPRINTF( "\t\tFinnish\n");
		return Finnish;
	}

	if (fingerTest(buffer, count, L"Externe printers voor NT")){
		MYPRINTF( "\t\tDutch\n");
		return Dutch;
	}

	if (fingerTest(buffer, count, L"Fjernprintere")){
		MYPRINTF( "\t\tDanish\n");
		return Danish;
	}

	if (fingerTest(buffer, count, wSwedish)){
		MYPRINTF( "\t\tSwedish\n");
		return Swedish;
	}

	if (fingerTest(buffer, count, L"Zdalne drukarki")){
		MYPRINTF( "\t\tPolish\n");
		return Polish;
	}

	if (fingerTest(buffer, count, wCzech)){
		MYPRINTF( "\t\tCzech\n");
		return Czech;
	}

	if (fingerTest(buffer, count, wTurkish)){
		MYPRINTF( "\t\tTurkish\n");
		return Turkish;
	}

	if (fingerTest(buffer, count, wJapanese)){
		MYPRINTF( "\t\tJapanese\n");
		return Japanese;
	}

	if (fingerTest(buffer, count, wTraditional)){
		MYPRINTF( "\t\tChinese - Traditional\n");
		return Chinese;
	}

	if (fingerTest(buffer, count, wTaiwan)){
		MYPRINTF( "\t\tChinese - Traditional / Taiwan\n");
		return Taiwan;
	}

	if (fingerTest(buffer, count, wKorean)){
		MYPRINTF( "\t\tKorean\n");
		return Korean;
	}

	if (fingerTest(buffer, count, wRussian)){
		MYPRINTF( "\t\tRussian\n");
		return Russian;
	}

	MYPRINTF( "\t\tno finger\n");

	return Nofinger;
}

FingerPrint::TARGETS FingerPrint::getLanguage(const char *target){
	char * spoolBuffer = NULL;
	long countBytesNeeded = 0;
	long printersFound  = 0;
	TARGETS targetOs;

	if(!Smb::bindToSpoolssPipe((unsigned char *)target)) {
		MYPRINTF( "Unable to bind to SPOOLSS\n");
		spoolss__MIDL_AutoBindHandle = NULL;
		return Dontknow;
	}

	RpcTryExcept
	{
		spoolss_EnumPrinters(spoolss__MIDL_AutoBindHandle, 8, NULL, 1, NULL, 0, &countBytesNeeded, &printersFound );
	}
	RpcExcept ( 1 )
	{

		MYPRINTF( "\t\tRpcExceptionCode() for spoolss_EnumPrinters(1) = %u\n", RpcExceptionCode());

		RpcBindingFree( &spoolss__MIDL_AutoBindHandle );
		spoolss__MIDL_AutoBindHandle = NULL;
		return Dontknow;
	}
	RpcEndExcept

		spoolBuffer = new char[(countBytesNeeded)];
	memset(spoolBuffer, 0, sizeof(spoolBuffer));

	RpcTryExcept
	{
		spoolss_EnumPrinters(spoolss__MIDL_AutoBindHandle, 8, NULL, 1, (byte *)spoolBuffer, countBytesNeeded, &countBytesNeeded, &printersFound );
	}
	RpcExcept ( 1 )
	{

		MYPRINTF( "\t\tRpcExceptionCode() for spoolss_EnumPrinters(2) = %u\n", RpcExceptionCode());

		RpcBindingFree( &spoolss__MIDL_AutoBindHandle );
		spoolss__MIDL_AutoBindHandle = NULL;
		delete[] spoolBuffer;
		return Dontknow;
	}
	RpcEndExcept

		RpcBindingFree( &spoolss__MIDL_AutoBindHandle );
	spoolss__MIDL_AutoBindHandle = NULL;

	//Print::printBuffer(spoolBuffer, countBytesNeeded);

	targetOs = parseBuffer((byte *)spoolBuffer, countBytesNeeded);
	delete[] spoolBuffer;
	return targetOs;
}

FingerPrint::TARGETS FingerPrint::connectAndGetLanguage(const char *target){
	if (!Smb::connect(target)){
		MYPRINTF("Unable to connect to %s\n", target);
		return FingerPrint::Dontknow;
	}
	MYPRINTF( "\t\tSMB Connect OK!\n");

	FingerPrint::TARGETS language = FingerPrint::getLanguage(target);

	if (!Smb::closeConnection(target)){
		MYPRINTF("Unable to disconnect from %s\n", target);
	}
	return language;
}