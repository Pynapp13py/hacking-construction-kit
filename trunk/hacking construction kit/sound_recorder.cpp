/*
 * Author: thirdstormofcythraul@outlook.com
 */
#include "sound_recorder.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

#include <Windows.h>
#include <MMSystem.h>

#include "macro.h"

#include "memory_debug.h"

#pragma comment(lib, "WinMM.Lib")

SoundRecorder::SoundRecorder(int sampleRate, int bitsPerSample, int nbChannels):
	m_sampleRate(sampleRate),
	m_bitsPerSample(bitsPerSample),
	m_nbChannels(nbChannels)
{

}

bool SoundRecorder::record(int lengthTime, const char *filename){
	unsigned char *response = 0;
	int responseSize = 0;
	bool res = record(lengthTime, &response, responseSize);
	if (!res){
		return false;
	}

	std::fstream myFile(filename, std::fstream::out | std::fstream::binary);
	myFile.write(reinterpret_cast<char*>(response), responseSize);	// data
    myFile.close();

	delete response;

	return res;
}

bool SoundRecorder::record(int lengthTime, unsigned char **buffer, int& size){
	HWAVEIN microHandle;
    WAVEHDR waveHeader;

    const int numpts = m_sampleRate * lengthTime;
    short int *waveIn = new short int[numpts];   // 'short int' is a 16-bit type; I request 16-bit samples below
												// for 8-bit capture, you'd use 'unsigned char' or 'BYTE' 8-bit types
    MMRESULT result = 0;

    WAVEFORMATEX format;
    format.wFormatTag = WAVE_FORMAT_PCM;      // simple, uncompressed format
    format.wBitsPerSample = m_bitsPerSample;                //  16 for high quality, 8 for telephone-grade
    format.nChannels = m_nbChannels;                     //  1=mono, 2=stereo
    format.nSamplesPerSec = m_sampleRate;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nChannels * format.wBitsPerSample / 8;
                                            // = nSamplesPerSec * n.Channels * wBitsPerSample/8
    format.nBlockAlign = format.nChannels * format.wBitsPerSample / 8;                    
                                            // = n.Channels * wBitsPerSample/8
    format.cbSize = 0;// Size, in bytes, of extra format information

    result = waveInOpen(&microHandle, WAVE_MAPPER, &format, 0L, 0L, WAVE_FORMAT_DIRECT);
    if (result) {
        MYPRINTF("waveInOpen failed\n");
        return false; 
    }

    // Set up and prepare header for input
    waveHeader.lpData = (LPSTR)waveIn;
    waveHeader.dwBufferLength = numpts * 2;
    waveHeader.dwBytesRecorded = 0;
    waveHeader.dwUser = 0L;
    waveHeader.dwFlags = 0L;
    waveHeader.dwLoops = 0L;
    waveInPrepareHeader(microHandle, &waveHeader, sizeof(WAVEHDR));

    // Insert a wave input buffer
    result = waveInAddBuffer(microHandle, &waveHeader, sizeof(WAVEHDR));
    if (result) {
        MYPRINTF("waveInAddBuffer failed\n");
        return false; 
    }

    result = waveInStart(microHandle);
    if (result) {
        MYPRINTF("waveInStart failed\n");
        return false; 
    }

     // Wait until finished recording
     do {} while (waveInUnprepareHeader(microHandle, &waveHeader, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
	 
	int chunksize, pcmsize, numSamples, subchunk1size;
	int audioFormat = WAVE_FORMAT_PCM;
		
	numSamples = ((long) (numpts / m_sampleRate) * 1000);
	pcmsize = sizeof(PCMWAVEFORMAT);
		
	subchunk1size = 16;
	int byteRate = m_sampleRate * m_nbChannels * m_bitsPerSample / 8;
	int blockAlign = m_nbChannels * m_bitsPerSample / 8;
	int subchunk2size = waveHeader.dwBufferLength * m_nbChannels;
	chunksize = (36 + subchunk2size);

	*buffer = new unsigned char[44 + waveHeader.dwBufferLength];
	size = 44 + waveHeader.dwBufferLength;

	(*buffer)[0] = 'R';
	(*buffer)[1] = 'I';
	(*buffer)[2] = 'F';
	(*buffer)[3] = 'F';

	memcpy(&(*buffer)[4], &chunksize, 4);

	(*buffer)[8] = 'W';
	(*buffer)[9] = 'A';
	(*buffer)[10] = 'V';
	(*buffer)[11] = 'E';

	(*buffer)[12] = 'f';
	(*buffer)[13] = 'm';
	(*buffer)[14] = 't';
	(*buffer)[15] = ' ';

	memcpy(&(*buffer)[16], &subchunk1size, 4);

	memcpy(&(*buffer)[20], &audioFormat, 2);

	memcpy(&(*buffer)[22], &m_nbChannels, 2);

	memcpy(&(*buffer)[24], &m_sampleRate, 4);

	memcpy(&(*buffer)[28], &byteRate, 4);

	memcpy(&(*buffer)[32], &blockAlign, 2);

	memcpy(&(*buffer)[34], &m_bitsPerSample, 2);

	(*buffer)[36] = 'd';
	(*buffer)[37] = 'a';
	(*buffer)[38] = 't';
	(*buffer)[39] = 'a';

	memcpy(&(*buffer)[40], &subchunk2size, 4);

	memcpy(&(*buffer)[44], waveHeader.lpData, waveHeader.dwBufferLength);

	Print::printBufferAsHexa(*buffer, 44);

     waveInClose(microHandle);

	 delete waveIn;

	 return true;
}