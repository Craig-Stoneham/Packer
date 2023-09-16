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

/**
 * @file Variant.h
 * @brief Defines the Variant class for handling various data types.
 */

#pragma once

#include "Error.h"

PACKER_NAMESPACE_BEGIN

/**
 * @class Variant
 * @brief Represents a variant that can hold various data types.
 */
class Variant {
public:
    /**
     * @enum Type
     * @brief Enumeration of supported data types by Variant.
     */
    enum class Type {
        Unknown = -1,  ///< Unknown data type.
        Null,          ///< Null value.
        Bool,          ///< Boolean value.
        Int,           ///< Integer value.
        String,        ///< String value.
        StringVector,  ///< Vector of strings.
        Max            ///< Maximum value for Type enumeration.
    };

private:
    Type type;      ///< The current data type.
    union {
        bool _bool;         ///< Boolean value.
        int _int;           ///< Integer value.
        void* _data;        ///< Generic data pointer.
    };

    /**
     * @brief Helper function to construct Variant.
     */
    void _construct();

    /**
     * @brief Helper function to destruct Variant.
     */
    void _destruct();

    /**
     * @brief Helper function to copy Variant from another Variant.
     * @param p_value The Variant to copy from.
     */
    void _copy(const Variant& p_value);

public:
    /**
     * @brief Get the string representation of a Type enumeration value.
     * @param p_type The Type enumeration value.
     * @return A string representing the Type.
     */
    static String get_type_name(Type p_type);

    /**
     * @brief Find the Type enumeration value from a string representation.
     * @param p_type The string representation of the Type.
     * @return The corresponding Type enumeration value.
     */
    static Type find_type(const String& p_type);

    /**
     * @brief Set the data type of the Variant.
     * @param p_type The Type to set.
     */
    void set_type(Type p_type);

    /**
     * @brief Get the current data type of the Variant.
     * @return The current Type.
     */
    Type get_type() const;

    /**
     * @brief Assigns the value of another Variant to this Variant.
     * @param p_value The Variant to copy from.
     * @return Reference to this Variant.
     */
    Variant& operator=(const Variant& p_value);

    /**
     * @brief Assigns a boolean value to this Variant.
     * @param p_value The boolean value to set.
     * @return Reference to this Variant.
     */
    Variant& operator=(bool p_value);

    /**
     * @brief Assigns an integer value to this Variant.
     * @param p_value The integer value to set.
     * @return Reference to this Variant.
     */
    Variant& operator=(int p_value);

    /**
     * @brief Assigns a string value to this Variant.
     * @param p_value The string value to set.
     * @return Reference to this Variant.
     */
    Variant& operator=(const String& p_value);

    /**
     * @brief Assigns a C-string value to this Variant.
     * @param p_value The C-string value to set.
     * @return Reference to this Variant.
     */
    Variant& operator=(const char* p_value);

    /**
     * @brief Assigns a vector of strings to this Variant.
     * @param p_value The vector of strings to set.
     * @return Reference to this Variant.
     */
    Variant& operator=(const StringVector& p_value);

    /**
     * @brief Converts the Variant to a constant boolean value.
     * @return The boolean value of the Variant.
     */
    operator const bool() const;

    /**
     * @brief Converts the Variant to a boolean value.
     * @return The boolean value of the Variant.
     */
    operator bool();

    /**
     * @brief Converts the Variant to a constant integer value.
     * @return The integer value of the Variant.
     */
    operator const int() const;

    /**
     * @brief Converts the Variant to an integer value.
     * @return The integer value of the Variant.
     */
    operator int();

    /**
     * @brief Converts the Variant to a constant string reference.
     * @return The string value of the Variant.
     */
    operator const String& () const;

    /**
     * @brief Converts the Variant to a string reference.
     * @return The string value of the Variant.
     */
    operator String& ();

    /**
     * @brief Converts the Variant to a constant vector of strings reference.
     * @return The vector of strings value of the Variant.
     */
    operator const StringVector& () const;

    /**
     * @brief Converts the Variant to a vector of strings reference.
     * @return The vector of strings value of the Variant.
     */
    operator StringVector& ();

    /**
     * @brief Compares the Variant to another Variant for equality.
     * @param p_value The Variant to compare with.
     * @return True if both Variants are equal, false otherwise.
     */
    bool operator == (const Variant& p_value);

    /**
     * @brief Compares the Variant to a boolean value for equality.
     * @param p_value The boolean value to compare with.
     * @return True if the Variant and the boolean value are equal, false otherwise.
     */
    bool operator == (const bool& p_value);

    /**
     * @brief Compares the Variant to an integer value for equality.
     * @param p_value The integer value to compare with.
     * @return True if the Variant and the integer value are equal, false otherwise.
     */
    bool operator == (const int& p_value);

    /**
     * @brief Compares the Variant to a string value for equality.
     * @param p_value The string value to compare with.
     * @return True if the Variant and the string value are equal, false otherwise.
     */
    bool operator == (const String& p_value);

    /**
     * @brief Compares the Variant to a vector of strings for equality.
     * @param p_value The vector of strings to compare with.
     * @return True if the Variant and the vector of strings are equal, false otherwise.
     */
    bool operator == (const StringVector& p_value);

    /**
     * @brief Compares the Variant to another Variant for inequality.
     * @param p_value The Variant to compare with.
     * @return True if both Variants are not equal, false otherwise.
     */
    bool operator != (const Variant& p_value);

    /**
     * @brief Compares the Variant to a boolean value for inequality.
     * @param p_value The boolean value to compare with.
     * @return True if the Variant and the boolean value are not equal, false otherwise.
     */
    bool operator != (const bool& p_value);

    /**
     * @brief Compares the Variant to an integer value for inequality.
     * @param p_value The integer value to compare with.
     * @return True if the Variant and the integer value are not equal, false otherwise.
     */
    bool operator != (const int& p_value);

    /**
     * @brief Compares the Variant to a string value for inequality.
     * @param p_value The string value to compare with.
     * @return True if the Variant and the string value are not equal, false otherwise.
     */
    bool operator != (const String& p_value);

    /**
     * @brief Compares the Variant to a vector of strings for inequality.
     * @param p_value The vector of strings to compare with.
     * @return True if the Variant and the vector of strings are not equal, false otherwise.
     */
    bool operator != (const StringVector& p_value);

    /**
     * @brief Serialize the Variant to a string.
     * @return A string representation of the Variant.
     */
    String serialize() const;

    /**
     * @brief Parse a string into the Variant.
     * @param p_value The string to parse.
     * @return An Error code indicating success or failure.
     */
    Error parse(const String& p_value);

    /**
     * @brief Default constructor, initializes the Variant as null.
     */
    Variant();

    /**
     * @brief Constructor to set the data type of the Variant.
     * @param p_type The Type to set.
     */
    Variant(Type p_type);

    /**
     * @brief Copy constructor to create a Variant from another Variant.
     * @param p_value The Variant to copy from.
     */
    Variant(const Variant& p_value);

    /**
     * @brief Constructor to initialize the Variant with a boolean value.
     * @param p_value The boolean value to set.
     */
    Variant(bool p_value);

    /**
     * @brief Constructor to initialize the Variant with an integer value.
     * @param p_value The integer value to set.
     */
    Variant(int p_value);

    /**
     * @brief Constructor to initialize the Variant with a string value.
     * @param p_value The string value to set.
     */
    Variant(const String& p_value);

    /**
     * @brief Constructor to initialize the Variant with a character array (C-string) value.
     * @param p_value The character array value to set.
     */
    Variant(const char* p_value);

    /**
     * @brief Constructor to initialize the Variant with a vector of strings value.
     * @param p_value The vector of strings value to set.
     */
    Variant(const StringVector& p_value);

    /**
     * @brief Destructor to clean up the Variant.
     */
    ~Variant();
};

PACKER_NAMESPACE_END