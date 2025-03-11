// See LICENSE for full copyright and licensing information.

#pragma once

#include "test_suite.h"

#include <config_file.h>

PACKER_NAMESPACE_BEGIN

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

PACKER_NAMESPACE_END
