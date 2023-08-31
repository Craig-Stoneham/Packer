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


#include "TestConfigFile.h"

template <class T>
struct Entry {
    String name;
    T value;
};

void TestConfigFile::test() {
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
        test_failed("Values not stored properly.");
        return;
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
        test_failed("Loaded values do not match the original entries.");
        return;
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
        test_failed("Loaded encrypted values do not match the original entries.");
    }
}

TestConfigFile::TestConfigFile() {
   add_test("ConfigFile", [this]() { test(); });
}
