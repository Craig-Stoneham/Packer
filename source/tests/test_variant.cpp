// See LICENSE for full copyright and licensing information.

#include "test_variant.h"

PACKER_NAMESPACE_BEGIN

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

PACKER_NAMESPACE_END
