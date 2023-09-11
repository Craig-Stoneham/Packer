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

/**
 * @file TestCrypto.cpp
 * @brief Contains the implementation of the TestCrypto class, which tests the cryptographic functions.
 */

#pragma once

#include "TestSuite.h"

 /**
  * @class TestCrypto
  * @brief Represents a test suite for cryptographic functionality.
  *
  * This class defines test cases for cryptographic operations, including encryption and decryption
  * using randomly generated keys.
  */
class TestCrypto : public TestSuite {
    uint32_t initial = initial = 0xBEADBEEF; /**< The initial value for randomization. */
    size_t num_tests = 1 << 16; /**< The number of cryptographic tests to perform. */

    /**
     * @brief Performs cryptographic tests.
     *
     * This function generates random data, encrypts and decrypts it with randomly generated keys,
     * and checks if the decrypted data matches the original data.
     */
    void test();

public:
    /**
     * @brief Constructs a new TestCrypto object.
     *
     * Initializes the test suite with cryptographic test cases.
     */
    TestCrypto();
};
