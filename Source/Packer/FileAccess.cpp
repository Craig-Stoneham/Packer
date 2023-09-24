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

#include "FileAccess.h"

#include "Defaults.h"

PACKER_NAMESPACE_BEGIN


static FileAccess::Callback callback = nullptr;

void FileAccess::set_callback(Callback p_callback) {
    callback = p_callback;
}

FileAccess::Callback FileAccess::get_callback() {
    return callback;
}

bool FileAccess::pack_file(const String& p_read_path, const String& p_write_path, bool p_move) {
    if (copy_file(p_read_path, p_write_path, FileAccess::copy_options::update_existing) == false) {
        return false;
    }

    if (p_move) {
        FileAccess::remove(p_read_path);
    }

    if (callback) {
        callback(p_write_path, p_write_path, p_move);
    }

    return true;
}

void FileAccess::normalize_separators(String& p_path) {
    for (size_t i = 0; i < p_path.size(); ++i) {
        if (p_path[i] == '\\') {
            p_path[i] = '/';
        }
    }
}

bool FileAccess::remove_suffix(String& p_path, const String& p_suffix) {
    if (p_suffix.empty()) {
        return true;
    }
    size_t suffix_pos = p_path.find(p_suffix, p_path.find('/'));
    if (suffix_pos != String::npos) {
        p_path.erase(suffix_pos, p_suffix.length());
        return true;
    } else {
        return false;
    }
}

PACKER_NAMESPACE_END
