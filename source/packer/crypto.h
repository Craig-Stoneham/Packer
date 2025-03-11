// See LICENSE for full copyright and licensing information.

#pragma once

#include "typedefs.h"

PACKER_NAMESPACE_BEGIN

/**
 * @class CryptoKey
 * @brief Represents a cryptographic key for encryption and decryption.
 */
class CryptoKey {
    friend class Crypto;

    static constexpr size_t DefaultKey = 0xDEADBEEF; /**< The default key value. */

    size_t key; /**< The cryptographic key value. */

public:
    /**
     * @brief Sets the cryptographic key to a specific value.
     * @param p_key The key value to set.
     */
    void set_key(const size_t p_key);

    /**
     * @brief Sets the cryptographic key from a string value.
     * @param p_key The string value from which to derive the key.
     */
    void set_key(const String& p_key);

    /**
     * @brief Sets the cryptographic key based on the type name of a class.
     * @tparam T The class type from which to derive the key.
     */
    template <class T>
    void set_key() {
        set_key(String(typeid(T).name()));
    }

    /**
     * @brief Constructs a CryptoKey instance with a default key.
     * @param p_key The initial key value.
     */
    CryptoKey(size_t p_key = DefaultKey);

    /**
     * @brief Constructs a CryptoKey instance from a string value.
     * @param p_key The string value from which to derive the key.
     */
    CryptoKey(const String& p_key);

    /**
     * @brief Constructs a CryptoKey instance based on the type name of a class.
     * @tparam T The class type from which to derive the key.
     * @param p_class An instance of the class used to derive the key.
     */
    template <class T>
    CryptoKey(const T& p_class) :
        key(std::hash<String>{}(String(typeid(T).name()))) {
    }
};

/**
 * @class Crypto
 * @brief Provides encryption and decryption functionalities.
 */
class Crypto {
public:
    /**
     * @brief Encrypts or decrypts data using a cryptographic key.
     * @param p_data The data to be encrypted or decrypted.
     * @param p_result The resulting encrypted or decrypted data.
     * @param p_key The cryptographic key to use.
     */
    static void encrypt_decrypt(const String& p_data, String& p_result, const CryptoKey& p_key);

    /**
     * @brief Encrypts or decrypts data using a cryptographic key and returns the result.
     * @param p_data The data to be encrypted or decrypted.
     * @param p_key The cryptographic key to use.
     * @return The resulting encrypted or decrypted data.
     */
    static String encrypt_decrypt(const String& p_data, const CryptoKey& p_key);
};

PACKER_NAMESPACE_END