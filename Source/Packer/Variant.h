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
    static String get_type_string(Type p_type);

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
     * @name Assignment Operators
     * @brief Assign values to the Variant.
     * @{
     */
    Variant& operator=(const Variant& p_value);
    Variant& operator=(bool p_value);
    Variant& operator=(int p_value);
    Variant& operator=(const String& p_value);
    Variant& operator=(const char* p_value);
    Variant& operator=(const StringVector& p_value);
    /** @} */

    /**
     * @name Conversion Operators
     * @brief Convert Variant to other data types.
     * @{
     */
    operator const bool() const;
    operator bool();
    operator const int() const;
    operator int();
    operator const String& () const;
    operator String& ();
    operator const StringVector& () const;
    operator StringVector& ();
    /** @} */

    /**
     * @name Equality Operators
     * @brief Compare the Variant to other values.
     * @{
     */
    bool operator == (const Variant& p_value);
    bool operator == (const bool& p_value);
    bool operator == (const int& p_value);
    bool operator == (const String& p_value);
    bool operator == (const StringVector& p_value);
    /** @} */

    /**
     * @name Inequality Operators
     * @brief Compare the Variant to other values.
     * @{
     */
    bool operator != (const Variant& p_value);
    bool operator != (const bool& p_value);
    bool operator != (const int& p_value);
    bool operator != (const String& p_value);
    bool operator != (const StringVector& p_value);
    /** @} */

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
