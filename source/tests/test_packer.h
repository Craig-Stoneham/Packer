/**************************************************************************/
/* Copyright (c) 2021-present Craig Stoneham.                             */
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