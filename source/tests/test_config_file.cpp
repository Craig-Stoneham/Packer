// See LICENSE for full copyright and licensing information.

#include "test_config_file.h"

PACKER_NAMESPACE_BEGIN

template <class T>
struct Entry {
    String name;
    T value;
};

TestResult TestConfigFile::test() {
    Entry<bool> bool_entry = { "bool_entry", true };
    Entry<int> int_entry = { "int_entry", 42 };
    Entry<String> string_entry = { "string_entry", "Hello, ConfigFile!" };
    Entry<StringVector> string_vector_entry = { "string_vector_entry", { "One", "Two", "Three" } };

    ConfigFile config_file;

    config_file.set_value(bool_entry.name, bool_entry.value);
    config_file.set_value(int_entry.name, int_entry.value);
    config_file.set_value(string_entry.name, string_entry.value);
    config_file.set_value(string_vector_entry.name, string_vector_entry.value);

    if (config_file.get_value(bool_entry.name) == bool_entry.value &&
        config_file.get_value(int_entry.name) == int_entry.value &&
        config_file.get_value(string_entry.name) == string_entry.value &&
        config_file.get_value(string_vector_entry.name) == string_vector_entry.value) {
    } else {
        return TEST_FAILED("Values not stored correctly.");
    }

    String test_file_name = "test_config.cfg";

    config_file.save(test_file_name);

    ConfigFile loaded_config_file;

    loaded_config_file.load(test_file_name);

    FileAccess::remove(test_file_name);

    if (loaded_config_file.get_value(bool_entry.name) == bool_entry.value &&
        loaded_config_file.get_value(int_entry.name) == int_entry.value &&
        loaded_config_file.get_value(string_entry.name) == string_entry.value &&
        loaded_config_file.get_value(string_vector_entry.name) == string_vector_entry.value) {
    } else {
        return TEST_FAILED("Loaded values do not match the original entries.");
    }

    String encrypted_file_name = "encrypted_config.cfg";

    CryptoKey crypto_key("encryption_key");

    config_file.save_encrypted(encrypted_file_name, crypto_key);

    ConfigFile encrypted_config_file;

    encrypted_config_file.load_encrypted(encrypted_file_name, crypto_key);

    FileAccess::remove(encrypted_file_name);

    if (encrypted_config_file.get_value(bool_entry.name) == bool_entry.value &&
        encrypted_config_file.get_value(int_entry.name) == int_entry.value &&
        encrypted_config_file.get_value(string_entry.name) == string_entry.value &&
        encrypted_config_file.get_value(string_vector_entry.name) == string_vector_entry.value) {
    } else {
        return TEST_FAILED("Loaded encrypted values do not match the original entries.");
    }
    return TEST_PASSED();
}

TestConfigFile::TestConfigFile() {
    ADD_TEST("ConfigFile", [this]() { return test(); });
}

PACKER_NAMESPACE_END
