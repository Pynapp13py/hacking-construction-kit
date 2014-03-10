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
/**
* Base64: encode and decode base64
*/
class Base64 {
public:
	Base64();

	~Base64();

	/**
	* return encoded data without trailing \0. You need to free the data.
	*/
	char *encode(unsigned char *data, size_t input_length, size_t *output_length);

	/**
	* return decoded data without trailing \0. You need to free the data.
	*/
	unsigned char *decode(const char *data,	size_t input_length, size_t *output_length);


private:
	void build_decoding_table();

	void base64_cleanup();

	static const char encoding_table[];
	static unsigned int mod_table[];
	char *decoding_table;
};

