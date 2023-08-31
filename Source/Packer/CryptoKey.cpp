/**************************************************************************/
/* Copyright (c) 2023 Craig Stoneham.                                     */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "CryptoKey.h"

static size_t random_lcg(size_t p_seed) {
	const size_t a = 6364136223846793005ULL;
	const size_t c = 1ULL;
	const size_t m = (1ULL << 63) - 1;

	p_seed = (a * p_seed + c) & m;

	return p_seed;
}

void CryptoKey::encrypt_decrypt(const String& p_data, String& p_result, size_t p_key) {
	p_result.resize(p_data.length());

	size_t key = p_key;

	for (size_t i = 0; i < p_data.length(); ++i) {
		p_result[i] = p_data[i] ^ (key & 0xFF);
		key = random_lcg(key);
	}
}

void CryptoKey::set_key(const size_t p_key) {
	key = p_key;
}

void CryptoKey::set_key(const String p_key) {
	key = std::hash<String>{}(p_key);
}

String CryptoKey::encrypt(const String& p_data) const {
	String encrypted_data;
	encrypt_decrypt(p_data, encrypted_data, key);
	return encrypted_data;
}

String CryptoKey::decrypt(const String& p_data) const {
	String decrypted_data;
	encrypt_decrypt(p_data, decrypted_data, key);
	return decrypted_data;
}

CryptoKey::CryptoKey(size_t p_key) :
	key(p_key) {
}

CryptoKey::CryptoKey(const String p_key) :
	key(std::hash<String>{}(p_key)) {
}
