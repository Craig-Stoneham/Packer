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

#ifndef DEFAULT_READ_PATH
#define DEFAULT_READ_PATH ""
#endif // DEFAULT_READ_PATH

#ifndef DEFAULT_WRITE_PATH
#define DEFAULT_WRITE_PATH ""
#endif // DEFAULT_WRITE_PATH

#ifndef DEFAULT_EXTENTIONS
#define DEFAULT_EXTENTIONS StringVector()
#endif // DEFAULT_EXTENTIONS

#ifndef DEFAULT_PACK_MODE
#define DEFAULT_PACK_MODE PackMode::Include
#endif // DEFAULT_PACK_MODE

#ifndef DEFAULT_OVERWRITE_FILES
#define DEFAULT_OVERWRITE_FILES false
#endif // DEFAULT_OVERWRITE_FILES

#ifndef DEFAULT_MOVE_FILES
#define DEFAULT_MOVE_FILES false
#endif // DEFAULT_MOVE_FILES

#ifndef DEFAULT_SUFFIX_STRING
#define DEFAULT_SUFFIX_STRING ""
#endif // DEFAULT_SUFFIX_STRING

#ifndef DEFAULT_SUFFIX_ENABLED
#define DEFAULT_SUFFIX_ENABLED false
#endif // DEFAULT_SUFFIX_ENABLED

#ifndef DEFAULT_EXTENSION_INSENSITIVE
#define DEFAULT_EXTENSION_INSENSITIVE false
#endif // DEFAULT_EXTENSION_INSENSITIVE

#ifndef DEFAULT_EXTENSION_ADJUST
#define DEFAULT_EXTENSION_ADJUST ExtensionAdjust::Default
#endif // DEFAULT_EXTENSION_ADJUST

#ifndef IGNORE_FILE_DISABLED

#ifndef DEFAULT_IGNORE_FILE_NAME
#define DEFAULT_IGNORE_FILE_NAME ".pkignore"
#endif // DEFAULT_IGNORE_FILE_NAME

#ifndef DEFAULT_IGNORE_FILE_ENABLED
#define DEFAULT_IGNORE_FILE_ENABLED true
#endif // DEFAULT_IGNORE_FILE_ENABLED

#endif // IGNORE_FILE_DISABLED

#ifndef DEFAULT_LOG_ENABLED
#define DEFAULT_LOG_ENABLED true
#endif // DEFAULT_LOG_ENABLED
