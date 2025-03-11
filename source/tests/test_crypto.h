// Copyright notice: See COPYRIGHT.md for full copyright and licensing information.

#pragma once

#include "test_suite.h"

#include <crypto.h>

PACKER_NAMESPACE_BEGIN

 /**
  * @class TestCrypto
  * @brief Represents a test suite for cryptographic functionality.
  *
  * This class defines test cases for cryptographic operations, including encryption and decryption
  * using randomly generated keys.
  */
class TestCrypto : public TestSuite {
    /**
     * @brief Performs cryptographic tests.
     *
     * This function generates random data, encrypts and decrypts it with randomly generated keys,
     * and checks if the decrypted data matches the original data.
     *
     * @param p_initial The initial value for randomization.
     * @param p_num_tests The number of cryptographic tests to perform.
     * @return The result of the cryptographic tests, indicating success or failure.
     */
    TestResult test(uint32_t p_initial = 0xBEADBEEF, size_t p_num_tests = 1 << 16);

public:
    /**
     * @brief Constructs a new TestCrypto object.
     *
     * Initializes the test suite with cryptographic test cases.
     */
    TestCrypto();
};

PACKER_NAMESPACE_END
