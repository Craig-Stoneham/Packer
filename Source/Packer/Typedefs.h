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
 * @file Typedefs.h
 * @brief Defines common types and utility macros for the application.
 */

#pragma once

#include <type_traits>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

#if __cplusplus >= 201703L
#include <filesystem>
#else // __cplusplus >= 201703L
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#endif // __cplusplus >= 201703L

/**
 * @def STRINGIFY(p_x)
 * @brief Converts a token into a string literal.
 * @param p_x The token to convert.
 */
#define STRINGIFY(p_x) #p_x

/**
 * @def MAKE_STRING(m_x)
 * @brief Macro to convert a token into a string literal.
 * @param m_x The token to convert.
 */
#define MAKE_STRING(m_x) STRINGIFY(m_x)

/**
 * @typedef String
 * @brief Alias for std::string, representing a sequence of characters.
 */
using String = std::string;

/**
 * @typedef Vector
 * @brief Alias for std::vector, representing a dynamic array.
 * @tparam T The type of elements stored in the vector.
 * @tparam ALLOC The allocator type for memory management (optional).
 */
template <class T, class ALLOC = std::allocator<T>>
using Vector = std::vector<T, ALLOC>;

/**
 * @typedef Map
 * @brief Alias for std::map, representing an associative container.
 * @tparam K The key type.
 * @tparam T The mapped type.
 * @tparam COMPARE The comparison function for keys (optional).
 * @tparam ALLOC The allocator type for memory management (optional).
 */
template <class K, class T, class COMPARE = std::less<K>, class ALLOC = std::allocator<std::pair<const K, T>>>
using Map = std::map<K, T, COMPARE, ALLOC>;

/**
 * @typedef StringVector
 * @brief Alias for a vector of String, representing a collection of strings.
 */
using StringVector = Vector<String>;

/**
 * @namespace FileAccess
 * @brief Namespace containing filesystem-related types and functions.
 */
namespace FileAccess {
#if __cplusplus >= 201703L
    using namespace std::filesystem;
#else // __cplusplus >= 201703L
    using namespace std::experimental::filesystem;
#endif // __cplusplus >= 201703L
};

/**
* @typedef StringStream
* @brief Alias for std::stringstream, representing a stream for string operations.
*/
using StringStream = std::stringstream;

/**
 * @typedef StringStreamI
 * @brief Alias for std::istringstream, representing an input string stream.
 */
using StringStreamI = std::istringstream;

/**
 * @typedef StringStreamO
 * @brief Alias for std::ostringstream, representing an output string stream.
 */
using StringStreamO = std::ostringstream;

/**
 * @typedef FileStreamI
 * @brief Alias for std::ifstream, representing an input file stream.
 */
using FileStreamI = std::ifstream;

/**
 * @typedef FileStreamO
 * @brief Alias for std::ofstream, representing an output file stream.
 */
using FileStreamO = std::ofstream;

/**
 * @var BinaryIOS
 * @brief Constant for specifying binary file I/O mode.
 * @details Use this constant as an argument to set binary mode when opening files.
 */
static constexpr int BinaryIOS = std::ios::binary;
