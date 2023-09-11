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

#include "TestVariant.h"

template <class T>
Variant::Type get_variant_type();

template <>
Variant::Type get_variant_type<bool>() { return Variant::Type::Bool; }

template <>
Variant::Type get_variant_type<int>() { return Variant::Type::Int; }

template <>
Variant::Type get_variant_type<String>() { return Variant::Type::String; }

template <>
Variant::Type get_variant_type<StringVector>() { return Variant::Type::StringVector; }

template <class T>
TestResult TestVariant::test(const T& value) {

    Variant variant = value;

    if (variant.get_type() != get_variant_type<T>()) {
        return TEST_FAILED("Type has not been set correctly.");
    }

    String serialized_string = variant.serialize();
    Variant parsed_variant;
    parsed_variant.parse(serialized_string);

    if (variant != parsed_variant) {
        return TEST_FAILED("Value does not match after serialize/parse.");
    }
    return TEST_PASSED();
}

TestVariant::TestVariant() {
    ADD_TEST("Variant<bool>", [this]() { return test(true); });
    ADD_TEST("Variant<int>", [this]() { return test(42); });
    ADD_TEST("Variant<String>", [this]() { return test(String("Hello World!")); });
    ADD_TEST("Variant<StringVector>", [this]() { return test(StringVector({ "One", "Two", "Three" })); });
}
