/**************************************************************************/
/* Copyright (c) 2021-present Craig Stoneham.                             */
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

#pragma once

#include "variant.h"
#include "crypto.h"

PACKER_NAMESPACE_BEGIN

/**
 * @class ConfigFile
 * @brief Provides functionality for managing key-value configuration data in a file.
 * 
 * The `ConfigFile` class allows you to store and retrieve configuration data as key-value pairs
 * in a file. It supports both plain text and encrypted storage using the `Crypto` class.
 */
class ConfigFile {
    Map<String, Variant> values; ///< A map to store key-value pairs.

public:
    /**
     * @brief Set a key-value pair in the configuration.
     * @param p_key The key for the configuration value.
     * @param p_value The value to associate with the key.
     */
    void set_value(const String& p_key, const Variant& p_value);

    /**
     * @brief Get the value associated with a given key.
     * @param p_key The key to look up.
     * @param p_default The default value to return if the key is not found.
     * @return The value associated with the key, or the default value if the key is not found.
     */
    Variant get_value(const String& p_key, Variant p_default = Variant()) const;

    /**
     * @brief Check if a key exists in the configuration.
     * @param p_key The key to check.
     * @return `true` if the key exists, `false` otherwise.
     */
    bool has_value(const String& p_key) const;

    /**
     * @brief Erase a key-value pair from the configuration.
     * @param p_key The key to erase.
     * @return `true` if the key was found and erased, `false` otherwise.
     */
    bool erase_value(const String& p_key);

    /**
     * @brief Clear all key-value pairs in the configuration.
     */
    void clear();

    /**
     * @brief Get a reference to the internal map of key-value pairs.
     * @return A reference to the map of key-value pairs.
     */
    const Map<String, Variant>& get_values() const;

    /**
     * @brief Set the entire configuration data from a map of key-value pairs.
     * @param p_values A map of key-value pairs to set as the configuration.
     */
    void set_values(const Map<String, Variant>& p_values);

    /**
     * @brief Get a list of all keys in the configuration.
     * @param p_keys An output vector to store the keys.
     */
    void get_keys(Vector<String>& p_keys) const;

    /**
     * @brief Save the configuration data to a file.
     * @param p_path The path to the file where the configuration will be saved.
     * @return An `Error` code indicating the success or failure of the operation.
     */
    Error save(const String& p_path) const;

    /**
     * @brief Load configuration data from a file.
     * @param p_path The path to the file from which configuration data will be loaded.
     * @return An `Error` code indicating the success or failure of the operation.
     */
    Error load(const String& p_path);

    /**
     * @brief Save the configuration data to an encrypted file.
     * @param p_path The path to the file where the encrypted configuration will be saved.
     * @param p_key The encryption key used to protect the configuration.
     * @return An `Error` code indicating the success or failure of the operation.
     */
    Error save_encrypted(const String& p_path, const CryptoKey& p_key) const;

    /**
     * @brief Load encrypted configuration data from a file.
     * @param p_path The path to the file from which encrypted configuration data will be loaded.
     * @param p_key The encryption key used to decrypt the configuration.
     * @return An `Error` code indicating the success or failure of the operation.
     */
    Error load_encrypted(const String& p_path, const CryptoKey& p_key);

    /**
     * @brief Constructor for the ConfigFile class.
     */
    ConfigFile();
};

PACKER_NAMESPACE_END
