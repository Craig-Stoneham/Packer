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
 * @file Version.h
 * @brief Defines the version information for the software.
 */

#pragma once

/**
 * @def VERSION_MAJOR
 * @brief The major version number of the software.
 */
#define VERSION_MAJOR 2

/**
 * @def VERSION_MINOR
 * @brief The minor version number of the software.
 */
#define VERSION_MINOR 5

/**
 * @def VERSION_PATCH
 * @brief The patch version number of the software.
 */
#define VERSION_PATCH 0

/**
 * @def VERSION_STRING
 * @brief A string representation of the software version in the format "MAJOR.MINOR.PATCH".
 */
#define VERSION_STRING MAKE_STRING(VERSION_MAJOR) "." MAKE_STRING(VERSION_MINOR) "." MAKE_STRING(VERSION_PATCH)
