// Copyright notice: See COPYRIGHT.md for full copyright and licensing information.

#pragma once

#include "test_suite.h"

#include <packer.h>

PACKER_NAMESPACE_BEGIN

/**
 * @class TestPacker
 * @brief Test suite for the Packer class.
 *
 * This class contains test cases for the functionality provided by the Packer class.
 */
class TestPacker : public TestSuite {
    Packer packer; ///< The Packer object used for testing.

    String read_path; ///< The path for reading test files.
    String write_path; ///< The path for writing packed files.
    Vector<String> files; ///< A list of test file names.

    /**
     * @brief Test the functionality of the Packer class.
     *
     * This function creates, packs, and tests various scenarios using the Packer class.
     *
     * @return True if all test cases pass, false otherwise.
     */
    bool test_packer();

    /**
     * @brief Run the Packer test cases.
     *
     * This function is responsible for executing the test cases for the Packer class.
     *
     * @return The result of the cryptographic tests, indicating success or failure.
     */
    TestResult test();

public:
    /**
     * @brief Construct a new TestPacker object.
     *
     * Initializes the test suite for the Packer class.
     */
    TestPacker();

    /**
     * @brief Destructor for the TestPacker object.
     *
     * Cleans up any resources used by the test suite.
     */
    ~TestPacker();
};

PACKER_NAMESPACE_END