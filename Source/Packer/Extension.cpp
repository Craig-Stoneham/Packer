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

#include "Extension.h"

static const char* extension_case[] = {
    "default",
    "lower",
    "upper"
};

String get_extension_case_string(ExtensionCase p_case) {
    if (static_cast<size_t>(p_case) < static_cast<size_t>(ExtensionCase::Max)) {
        return extension_case[static_cast<size_t>(p_case)];
    } else {
        return "unknown";
    }
}

ExtensionCase find_extension_case(const String& p_case) {
    for (size_t i = 0; i < static_cast<size_t>(ExtensionCase::Max); ++i) {
        if (p_case == extension_case[i]) {
            return static_cast<ExtensionCase>(i);
        }
    }
    return static_cast<ExtensionCase>(-1);
}
