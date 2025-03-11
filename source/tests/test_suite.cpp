// See LICENSE for full copyright and licensing information.

#include "test_suite.h"

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

    return EXIT_SUCCESS;
}

PACKER_NAMESPACE_END
