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

// Record sound from mic
class SoundRecorder {
public:

	/**
	* Create a new SoundRecorder
	* sampleRate must be 22050 or 44100
	* bitsPerSample 8 or 16
	* nbChannels is 1 for mono, 2 for stereo
	*/
	SoundRecorder(int sampleRate = 22050, int bitsPerSample = 8, int nbChannels = 1);

	// Record sound
	// length time in seconds
	// buffer need to be deleted after call
	bool record(int lengthTime, unsigned char **buffer, int& size);
	// Record sound and save as filename
	// length time in seconds
	// TODO: calculate correctly lengthtime
	bool record(int lengthTime, const char *filename);

private:
	int m_sampleRate;
	int m_bitsPerSample;
	int m_nbChannels;

};