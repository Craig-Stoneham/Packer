// Copyright notice: See COPYRIGHT.md for full copyright and licensing information.

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
