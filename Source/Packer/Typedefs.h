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

// Convert into a string literal.
#define STRINGIFY(p_x) #p_x
#define MAKE_STRING(m_x) STRINGIFY(m_x)

// Common types

using String = std::string;

template <class T, class ALLOC = std::allocator<T>>
using Vector = std::vector<T, ALLOC>;

template <class K, class T, class COMPARE = std::less<K>, class ALLOC = std::allocator<std::pair<const K, T>>>
using Map = std::map<K, T, COMPARE, ALLOC>;

using StringVector = Vector<String>;

#if __cplusplus >= 201703L
namespace FileAccess {
	using namespace std::filesystem;
};
#else // __cplusplus >= 201703L
namespace FileAccess {
	using namespace std::experimental::filesystem;
};
#endif // __cplusplus >= 201703L

using StringStream = std::stringstream;
using StringStreamI = std::istringstream;
using StringStreamO = std::ostringstream;

using FileStreamI = std::ifstream;
using FileStreamO = std::ofstream;

static constexpr int BinaryIOS = std::ios::binary;
