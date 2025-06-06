// See LICENSE for full copyright and licensing information.

#pragma once

#include <typedefs.h>

#include <functional>

PACKER_NAMESPACE_BEGIN

/**
 * @brief Typedef for function objects.
 *
 * This template typedef defines a standard alias for function objects,
 * allowing for the use of `std::function` with custom types in the test suite.
 *
 * @tparam T The type of the function object.
 */
template <class T>
using Function = std::function<T>;

/**
 * @struct TestResult
 * @brief Represents the result of a test case.
 */
struct TestResult {
    enum Error {
        OK, ///< The test passed.
        Failed, ///< The test failed.
    };

    Error error; ///< The error status of the test.
    String message; ///< A message describing the test result.

    /**
     * @brief Checks if the test result has the specified error status.
     * @param p_error The error status to compare with.
     * @return True if the test result has the specified error status, false otherwise.
     */
    bool operator==(Error p_error) const;

    /**
     * @brief Checks if the test result does not have the specified error status.
     * @param p_error The error status to compare with.
     * @return True if the test result does not have the specified error status, false otherwise.
     */
    bool operator!=(Error p_error) const;

    /// Default constructor for a passing test.
    TestResult();

    /**
     * @brief Constructor to set a custom error status.
     * @param p_error The custom error status for the test result.
     */
    TestResult(Error p_error);

    /**
     * @brief Constructor for a failed test with a custom message.
     * @param p_message The custom message describing the test result.
     */
    TestResult(const char* p_message);

    /**
     * @brief Constructor for a failed test with a custom message.
     * @param p_message The custom message describing the test result.
     */
    TestResult(const String& p_message);
};


/// Defines a function type for test cases that returns a TestResult.
using TestFunction = Function <TestResult()>;

/**
 * @class TestSuite
 * @brief A class to manage and run a suite of test cases.
 */
class TestSuite {
    struct TestCase {
        String name; ///< The name of the test case.
        TestFunction function; ///< The function that defines the test case.
        TestResult result; ///< The result of the test case.
    };

    static Vector<TestCase> test_cases; ///< A list of test cases to run.

public:
    /**
     * @brief Adds a test case to the suite.
     * @param p_name The name of the test case.
     * @param p_function The function that defines the test case.
     */
    static void add_test(const String& p_name, TestFunction p_function);

    /**
     * @brief Runs all test cases in the suite.
     * @param p_pause Pause the system before returning.
     * @return The number of test failures (0 for success).
     */
    static int run_tests(bool p_pause = false);
};

/**
 * @def TEST_PASSED
 * @brief A macro to indicate a passing test case.
 * @details Use this macro within your test case functions to indicate success.
 */
#define TEST_PASSED() TestResult(TestResult::OK)

/**
 * @def TEST_FAILED
 * @brief A macro to indicate a failing test case with a custom message.
 * @param p_message The custom message describing the failure.
 * @details Use this macro within your test case functions to indicate failure with a custom message.
 */
#define TEST_FAILED(p_message) TestResult(p_message)

/**
 * @def ADD_TEST
 * @brief A macro to add a test case to the test suite.
 * @param p_name The name of the test case.
 * @param p_function The function that defines the test case.
 * @details Use this macro in your test suite's initialization code to add test cases.
 */
#define ADD_TEST(p_name, p_function) TestSuite::add_test(p_name, p_function)

PACKER_NAMESPACE_END
