// See LICENSE for full copyright and licensing information.

#pragma once

#include <type_traits>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

#if _MSC_VER 
#if !_HAS_CXX17
#define EXPERIMENTAL_FILESYSTEM
#endif // _HAS_CXX17
#else // _MSC_VER
#if __cplusplus < 201703L
#define EXPERIMENTAL_FILESYSTEM
#endif // __cplusplus < 201703L
#endif // _MSC_VER

#ifndef EXPERIMENTAL_FILESYSTEM
#include <filesystem>
#else // EXPERIMENTAL_FILESYSTEM
#include <experimental/filesystem>
#endif //EXPERIMENTAL_FILESYSTEM

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
 * @namespace FileAccess
 * @brief Namespace containing filesystem-related types and functions.
 */
namespace FileAccess {
#ifndef EXPERIMENTAL_FILESYSTEM
    using namespace std::filesystem; // Use C++17 filesystem API.
#else // EXPERIMENTAL_FILESYSTEM
    using namespace std::experimental::filesystem; // Use experimental filesystem API.
#endif // EXPERIMENTAL_FILESYSTEM
};

/**
 * @def PACKER_NAMESPACE_BEGIN
 * @brief Defines the beginning of the optional Packer namespace.
 *
 * This macro defines the beginning of an optional namespace, allowing the code within it to be encapsulated within the
 * "Packer" namespace. The namespace encapsulation is controlled by the presence or absence of the "PACKER_NAMESPACE_ENABLED"
 * preprocessor macro.
 */
#ifdef PACKER_NAMESPACE_ENABLED
#define PACKER_NAMESPACE_BEGIN namespace Packer {
#else // PACKER_NAMESPACE_ENABLED
#define PACKER_NAMESPACE_BEGIN
#endif // PACKER_NAMESPACE_ENABLED

/**
 * @def PACKER_NAMESPACE_END
 * @brief Defines the end of the optional Packer namespace.
 *
 * This macro defines the end of an optional namespace, closing the "Packer" namespace encapsulation. The namespace
 * encapsulation is controlled by the presence or absence of the "PACKER_NAMESPACE_ENABLED" preprocessor macro.
 */
#ifdef PACKER_NAMESPACE_ENABLED
#define PACKER_NAMESPACE_END };
#else // PACKER_NAMESPACE_ENABLED
#define PACKER_NAMESPACE_END
#endif // PACKER_NAMESPACE_ENABLED

/**
 * @def USING_NAMESPACE_PACKER
 * @brief Provides the option to use the "Packer" namespace.
 *
 * This macro allows the option to include the entire "Packer" namespace in the current scope, making all classes, functions,
 * and variables within the "Packer" namespace accessible without qualifying them with "Packer::". The usage of this macro is
 * determined by the presence or absence of the "USING_NAMESPACE_PACKER" preprocessor macro.
 */
#ifdef PACKER_NAMESPACE_ENABLED
#define USING_NAMESPACE_PACKER using namespace Packer;
#else // PACKER_NAMESPACE_ENABLED
#define USING_NAMESPACE_PACKER
#endif // PACKER_NAMESPACE_ENABLED

/**
 * @brief Normalizes path separators in a string.
 *
 * This function replaces all backslashes ('\') in the given string with forward slashes ('/').
 *
 * @param p_path The string to normalize path separators in.
 */
inline static void normalize_path_separators(String& p_path) {
    for (size_t i = 0; i < p_path.size(); ++i) {
        if (p_path[i] == '\\') {
            p_path[i] = '/';
        }
    }
}

/**
* @brief Removes a specified suffix from a path string.
*
* This function searches for the specified suffix in the path string, starting from the first forward slash ('/').
* If the suffix is found, it is removed from the path string, and the function returns true. If the suffix is not
* found, the function returns false.
*
* @param p_path The path string from which to remove the suffix.
* @param p_suffix The suffix to remove from the path string.
* @return True if the suffix was found and removed, false otherwise.
*/
inline static bool remove_path_suffix(String& p_path, const String& p_suffix) {
    if (p_suffix.empty()) {
        return false;
    }
    size_t suffix_pos = p_path.find(p_suffix, p_path.find('/'));
    if (suffix_pos != String::npos) {
        p_path.erase(suffix_pos, p_suffix.length());
        return true;
    } else {
        return false;
    }
}