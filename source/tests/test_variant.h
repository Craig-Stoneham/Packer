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

/**
 * @file test_variant.h
 * @brief Defines test cases for the Variant class.
 */

#pragma once

#include "test_suite.h"

#include <variant.h>

PACKER_NAMESPACE_BEGIN

 /**
  * @class TestVariant
  * @brief Test cases for the Variant class.
  *
  * This class defines test cases for the Variant class. It tests various types of
  * values to ensure that Variant correctly stores and serializes them.
  */
class TestVariant : public TestSuite {
    /**
     * @brief Test a specific type using the Variant class.
     *
     * This template function tests a specific type by creating a Variant instance with
     * the provided value, ensuring that the Variant correctly sets its type, and
     * serializes and deserializes the value correctly.
     *
     * @tparam T The type to test with Variant.
     * @param value The value of type T to test.
     *
     * @return The result of the test, indicating success or failure.
     */
    template <class T>
    TestResult test(const T& value);

public:
    /**
     * @brief Constructor for the TestVariant class.
     *
     * Initializes and adds test cases for different types of values to be tested
     * with the Variant class.
     */
    TestVariant();
};

PACKER_NAMESPACE_END
