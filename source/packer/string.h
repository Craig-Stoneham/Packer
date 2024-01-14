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

/**
 * @file string.h
 * @brief Defines a string class.
 */

#pragma once

#include "typedefs.h"

//PACKER_NAMESPACE_BEGIN


// Make a wrapper class that wraps all of the functionallity of std::string.
/*class String {
	std::string string;

public:

	String() = default;
	String(const std::string& str) : string(str) {}
	String(const char* str) : string(str) {}

	// Operators

	String& operator=(const std::string& str) { string = str; return *this; }
	String& operator=(const char* str) { string = str; return *this; }

	String& operator+=(const std::string& str) { string += str; return *this; }
	String& operator+=(const char* str) { string += str; return *this; }

	String operator+(const std::string& str) const { return String(string + str); }
	String operator+(const char* str) const { return String(string + str); }

	bool operator==(const std::string& str) const { return string == str; }
	bool operator==(const char* str) const { return string == str; }

	bool operator!=(const std::string& str) const { return string != str; }
	bool operator!=(const char* str) const { return string != str; }

	bool operator<(const std::string& str) const { return string < str; }
	bool operator<(const char* str) const { return string < str; }

	bool operator>(const std::string& str) const { return string > str; }
	bool operator>(const char* str) const { return string > str; }

	bool operator<=(const std::string& str) const { return string <= str; }
	bool operator<=(const char* str) const { return string <= str; }

	bool operator>=(const std::string& str) const { return string >= str; }
	bool operator>=(const char* str) const { return string >= str; }

	char& operator[](size_t index) { return string[index]; }
	const char& operator[](size_t index) const { return string[index]; }

	operator std::string() const { return string; }


	// Functions

// Returns the length of the string.
size_t length() const { return string.length(); }

// Returns the size of the string.
size_t size() const { return string.size(); }

// Returns the capacity of the string.
size_t capacity() const { return string.capacity(); }

// Returns the maximum size of the string.
size_t max_size() const { return string.max_size(); }

// Returns the number of bytes in the string.
size_t byte_size() const { return string.size() * sizeof(char); }

// Returns the number of bytes the string can hold.
size_t byte_capacity() const { return string.capacity() * sizeof(char); }

// Returns the number of bytes the string can hold.
size_t byte_max_size() const { return string.max_size() * sizeof(char); }

// Returns the number of bytes the string can hold.
size_t byte_length() const { return string.length() * sizeof(char); }






};*/


//PACKER_NAMESPACE_END