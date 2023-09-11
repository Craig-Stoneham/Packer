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
 * @file TestConfigFile.cpp
 * @brief Contains the implementation of the TestConfigFile class.
 */

#pragma once

#include "TestSuite.h"

/**
 * @class TestConfigFile
 * @brief A test suite for the ConfigFile class.
 *
 * This test suite evaluates the functionality of the ConfigFile class by testing
 * various aspects such as setting values, saving and loading configuration files,
 * and encryption and decryption of configuration files.
 */
class TestConfigFile : public TestSuite {
    /**
     * @brief Test the functionality of the ConfigFile class.
     *
     * This test function sets various values of different types in a ConfigFile
     * instance, saves the configuration to a file, loads the configuration from
     * the file, and verifies that the loaded values match the original values.
     *
     * Additionally, it tests the encryption and decryption of configuration files
     * using a CryptoKey.
     *
     * @return The result of the test, indicating success or failure.
     */
    TestResult test();

public:
    /**
     * @brief Construct a new TestConfigFile object.
     *
     * This constructor adds the "ConfigFile" test case to the test suite.
     */
    TestConfigFile();
};
