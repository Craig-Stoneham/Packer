// See LICENSE for full copyright and licensing information.

#include "test_crypto.h"

PACKER_NAMESPACE_BEGIN

TestResult TestCrypto::test(uint32_t p_initial, size_t p_num_tests) {

    std::srand(p_initial);

    for (size_t i = 0; i < p_num_tests; ++i) {
        int value = std::rand();
        CryptoKey key(value);
        String test_key = "test_key " + std::to_string(value);
        String encrypted_key = Crypto::encrypt_decrypt(test_key, key);
        String decrypted_key = Crypto::encrypt_decrypt(encrypted_key, key);
        if (decrypted_key != test_key) {
            return TEST_FAILED("The data does not match after undergoing encryption and decryption with the key '" + std::to_string(value) + "'.");
        }
    }
    return TEST_PASSED();
}

TestCrypto::TestCrypto() {
    ADD_TEST("Crypto", [this]() { return test(); });
}

PACKER_NAMESPACE_END