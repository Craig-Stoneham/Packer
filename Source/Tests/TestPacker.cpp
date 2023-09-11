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

#include "TestPacker.h"

TestResult TestPacker::test() {





    // Create a testsuite for Packer
    String current_path = FileAccess::current_path().string();

    std::cout << (current_path + "\n");

    String cross_platform_path;
    for (char c : current_path) {
        if (c == '\\') {
            cross_platform_path += '/';
        } else {
            cross_platform_path += c;
        }
    }

    std::cout << (cross_platform_path + "\n");

	if (FileAccess::exists(cross_platform_path)) {
		std::cout << cross_platform_path << " exists\n";
	} else {
		std::cout << cross_platform_path << " does not exist\n";
	}

    // @TODO: Add tests
    return TEST_FAILED("Failed Packer");

    return TEST_PASSED();
}

TestPacker::TestPacker() {
    ADD_TEST("Packer", [this]() { return test(); });
}
