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
 * @file Defaults.h
 * @brief Defines default configuration values for the application.
 */

#pragma once

/**
 * @def DEFAULT_READ_PATH
 * @brief The default path for reading files.
 */
#define DEFAULT_READ_PATH ""

/**
 * @def DEFAULT_WRITE_PATH
 * @brief The default path for writing files.
 */
#define DEFAULT_WRITE_PATH ""

/**
 * @def DEFAULT_EXTENTIONS
 * @brief The default file extensions to include.
 */
#define DEFAULT_EXTENTIONS StringVector()

/**
 * @def DEFAULT_PACK_MODE
 * @brief The default packing mode.
 */
#define DEFAULT_PACK_MODE PackMode::Include

/**
 * @def DEFAULT_OVERWRITE_FILES
 * @brief The default option to overwrite existing files.
 */
#define DEFAULT_OVERWRITE_FILES false

/**
 * @def DEFAULT_MOVE_FILES
 * @brief The default option to move files instead of copying.
 */
#define DEFAULT_MOVE_FILES false

/**
 * @def DEFAULT_SUFFIX_STRING
 * @brief The default suffix to add to packed files.
 */
#define DEFAULT_SUFFIX_STRING ""

/**
 * @def DEFAULT_SUFFIX_ENABLED
 * @brief The default option to enable suffixes for packed files.
 */
#define DEFAULT_SUFFIX_ENABLED false

/**
 * @def DEFAULT_EXTENSION_INSENSITIVE
 * @brief The default option for case-insensitive file extensions.
 */
#define DEFAULT_EXTENSION_INSENSITIVE false

/**
 * @def DEFAULT_EXTENSION_ADJUST
 * @brief The default extension adjustment mode.
 */
#define DEFAULT_EXTENSION_ADJUST ExtensionAdjust::Default

#ifdef IGNORE_FILE_ENABLED
/**
 * @def DEFAULT_IGNORE_FILE_NAME
 * @brief The default name of the ignore file.
 */
#define DEFAULT_IGNORE_FILE_NAME ".pkignore"

/**
 * @def DEFAULT_IGNORE_FILE_ENABLED
 * @brief The default option to enable ignore file processing.
 */
#define DEFAULT_IGNORE_FILE_ENABLED true

#endif // IGNORE_FILE_ENABLED

/**
 * @def DEFAULT_LOG_ENABLED
 * @brief The default option to enable logging.
 */
#define DEFAULT_LOG_ENABLED true
