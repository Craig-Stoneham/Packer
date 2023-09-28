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
 * @file error.h
 * @brief Defines error codes and associated functions.
 */

#pragma once

#include "typedefs.h"

PACKER_NAMESPACE_BEGIN

/**
 * @enum Error
 * @brief Enumerates error codes.
 */
enum class Error {
    Unknown = -1,        ///< Unknown error.
    OK,                  ///< No error; operation successful.
    Failed,              ///< General operation failure.
    Unconfigured,        ///< Configuration not set.
    RangeError,          ///< Value out of valid range.
    FileNotFound,        ///< File not found.
    FileBadPath,         ///< Invalid file path.
    FileAlreadyInUse,    ///< File is already in use.
    FileCantOpen,        ///< Unable to open file.
    InvalidData,         ///< Invalid data format.
    DoesNotExist,        ///< The requested item does not exist.
    Max                  ///< Maximum value for error codes.
};

/**
 * @brief Get a string representation of an error code.
 * @param p_error The error code.
 * @return A string describing the error.
 */
String get_error_name(Error p_error);

/**
 * @brief Find an error code by its string representation.
 * @param p_error The string representation of the error.
 * @return The corresponding error code, or Error::Unknown if not found.
 */
Error find_error(const String& p_error);

PACKER_NAMESPACE_END