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
 * @file file_access.h
 * @brief Provides file access utilities for reading, writing, and managing files.
 */

#pragma once

#include "Typedefs.h"

PACKER_NAMESPACE_BEGIN

/**
 * @namespace FileAccess
 * @brief A namespace containing utilities for file access operations.
 *
 * The `FileAccess` namespace provides functions and a callback mechanism for performing various
 * file access operations such as reading, writing, copying, moving, and removal of files.
 */
namespace FileAccess {
#ifndef EXPERIMENTAL_FILESYSTEM
    using namespace std::filesystem; // Use C++17 filesystem API.
#else // EXPERIMENTAL_FILESYSTEM
    using namespace std::experimental::filesystem; // Use experimental filesystem API.
#endif // EXPERIMENTAL_FILESYSTEM

    /**
     * @brief A callback function signature for file access operations.
     *
     * This callback function is used to notify external code about the pack file operations 
     * It receives the source path, destination path, and whether the operation is a move.
     */
    using Callback = void (*)(const String&, const String&, bool);

    /**
     * @brief Set a callback function for file access operations.
     *
     * This function allows you to set a callback that will be called when the file packing
     * operation is performed. The callback receives source and destination paths, and a flag
     * indicating whether the operation is a move (true) or not (false).
     *
     * @param p_callback The callback function to set.
     */
    void set_callback(Callback p_callback);

    /**
     * @brief Get the currently registered file access callback function.
     *
     * @return The currently registered callback function or nullptr if no callback is set.
     */
    Callback get_callback();

    /**
     * @brief Pack a file from the source path to the destination path.
     *
     * This function copies a file from the source path to the destination path. It also provides an
     * option to move the file (delete the source after copying).
     *
     * @param p_read_path The source path of the file to pack.
     * @param p_write_path The destination path where the file will be written.
     * @param p_move If true, the function will move the file (delete the source); otherwise, it will
     *               copy the file.
     *
     * @return true if the operation was successful, false otherwise.
     */
    bool pack_file(const String& p_read_path, const String& p_write_path, bool p_move = false);

    /**
     * @brief Normalizes path separators in a string.
     *
     * This function replaces all backslashes ('\') in the given string with forward slashes ('/').
     *
     * @param p_path The string to normalize path separators in.
     */
    void normalize_separators(String& p_path);

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
    bool remove_suffix(String& p_path, const String& p_suffix);
};

PACKER_NAMESPACE_END
