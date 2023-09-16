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

#include "TestSuite.h"

PACKER_NAMESPACE_BEGIN

bool TestResult::operator==(Error p_error) const {
    return error == p_error;
}

bool TestResult::operator!=(Error p_error) const {
    return error != p_error;
}

TestResult::TestResult() :
    error(OK) {
}

TestResult::TestResult(Error p_error) :
    error(p_error) {
}

TestResult::TestResult(const char* p_message) :
    error(Failed),
    message(p_message) {
}

TestResult::TestResult(const String& p_message) :
    error(Failed),
    message(p_message) {
}

Vector<TestSuite::TestCase> TestSuite::test_cases;

void TestSuite::add_test(const String& p_name, TestFunction p_function) {
    test_cases.push_back({ p_name, p_function });
}

int TestSuite::run_tests(bool p_pause) {
    int num_failures = 0;

    for (auto& test_case : test_cases) {
        std::cout << "Running test: " << test_case.name << "...\n";
        test_case.result = test_case.function();
        if (test_case.result.error == TestResult::OK) {
            std::cout << "Passed\n";
        } else {
            std::cout << "Failed: " << test_case.result.message << "\n";
            ++num_failures;
        }
    }

    if (p_pause) {
        system("PAUSE");
    }

    return num_failures ? EXIT_FAILURE : EXIT_SUCCESS;
}

PACKER_NAMESPACE_END
