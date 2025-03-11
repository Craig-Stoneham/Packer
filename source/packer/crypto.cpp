// See LICENSE for full copyright and licensing information.

#include "crypto.h"

PACKER_NAMESPACE_BEGIN

void CryptoKey::set_key(const size_t p_key) {
	key = p_key;
}

void CryptoKey::set_key(const String& p_key) {
	key = std::hash<String>{}(p_key);
}

CryptoKey::CryptoKey(size_t p_key) :
	key(p_key) {
}

CryptoKey::CryptoKey(const String& p_key) :
	key(p_key.size() ? std::hash<String>{}(p_key) : 0) {
}

static size_t random_lcg(size_t p_seed) {
	const size_t a = 6364136223846793005ULL;
	const size_t c = 1ULL;
	const size_t m = (1ULL << 63) - 1;

	p_seed = (a * p_seed + c) & m;

	return p_seed;
}

void Crypto::encrypt_decrypt(const String& p_data, String& p_result, const CryptoKey& p_key) {
	p_result.resize(p_data.length());

	size_t key = p_key.key;

	for (size_t i = 0; i < p_data.length(); ++i) {
		p_result[i] = p_data[i] ^ (key & 0xFF);
		key = random_lcg(key);
	}
}

String Crypto::encrypt_decrypt(const String& p_data, const CryptoKey& p_key) {
	String data;
	encrypt_decrypt(p_data, data, p_key);
	return data;
}

PACKER_NAMESPACE_END
