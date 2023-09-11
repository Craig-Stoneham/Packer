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
 * @file TestSuite.h
 * @brief Defines a basic test suite framework for writing and running tests.
 */

#pragma once

#include "Typedefs.h"

/**
 * @class TestSuite
 * @brief A simple test suite framework for writing and running tests.
 *
 * This class provides a basic framework for creating and running test cases.
 * Test cases are added to the suite using the `add_test` method, and then
 * the `run_tests` method is called to execute all the test cases.
 */
class TestSuite {
    struct TestCase {
        String name; ///< The name of the test case.
        Function<void()> function; ///< The function that defines the test case.
    };

    Vector<TestCase> test_cases; ///< A list of test cases to run.
    bool has_error = false; ///< Indicates whether a test case has failed.
    String error_message; ///< The error message associated with a failed test.

protected:
    /**
     * @brief Marks a test case as failed with a given error message.
     * @param p_message The error message for the failed test case.
     */
    void test_failed(const String& p_message);

    /**
     * @brief Adds a test case to the test suite.
     * @param p_name The name of the test case.
     * @param p_function The function that defines the test case.
     */
    void add_test(const String& p_name, Function<void()> p_function);

public:
    /**
     * @brief Runs all the test cases in the suite.
     */
    void run_tests();

    /**
     * @brief Constructor for the TestSuite class.
     */
    TestSuite();
};
