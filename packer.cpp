/*************************************************************************/
/*  packer.cpp                                                           */
/*************************************************************************/
/*                       This file is part of:                           */
/*                       TUFF GAMES UTILITIES                            */
/*************************************************************************/
/* Copyright (c) 2020 Craig Stoneham.                                    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "packer.h"

void Packer::pack_files(const String& p_read_path, const String& p_write_path, bool p_overwrite, bool p_delete_old, bool p_remove_suffix) {

#ifdef PACKER_LOG_ENABLED
    log_stream.open(PACKER_LOG_PATH, PACKER_OPEN_MODE);
    log_stream << "Reading from " << p_read_path << '\n';
    log_stream << "Writing to " << p_write_path << '\n';
    log_stream << "Overwrite " << (p_overwrite ? "enabled" : "disabled") << '\n';
    log_stream << "Delete old " << (p_delete_old ? "enabled" : "disabled") << '\n';
#ifdef PACKER_READ_LOWER_CASE_EXTENSIONS
    //log_stream << "COnverting all extensions to lowercase when reading\n";
#endif // PACKER_READ_LOWER_CASE_EXTENSIONS
#ifdef PACKER_WRITE_LOWER_CASE_EXTENSIONS
    //log_stream << "COnverting all extensions to lowercase when writing\n";
#endif // PACKER_WRITE_LOWER_CASE_EXTENSIONS
    if (p_remove_suffix) {
        log_stream << "Removing suffix " << suffix << "\n";
    }
    else {
        log_stream << "Suffix removal disabled\n";
    }
    if (extensions.size()) {
        if (extensions.size() == 1) {
            log_stream << "Added extension " << extensions[0] << '\n';
        }
        else {

            log_stream << "Added extensions " << extensions[0];
            for (size_t index = 1; index < extensions.size(); ++index) {
                log_stream << " ," << extensions[index];
            }
            log_stream << "\n";
        }
    }
#endif //PACKER_LOG_ENABLED

    if (extensions.size() == 0) {
#ifdef PACKER_LOG_ENABLED
        log_stream << "No extensions are added\n";
        log_stream.close();
#endif //PACKER_LOG_ENABLED 
        return;
    }

    if (_FS::exists(p_read_path) == false) {
#ifdef PACKER_LOG_ENABLED
        log_stream << "Read path does not exist\n";
        log_stream.close();
#endif //PACKER_LOG_ENABLED       
        return;
    }

    if (_FS::exists(p_write_path) == false) {
        _FS::create_directory(p_write_path);
    }

    if (p_overwrite) {
        if (p_delete_old) {
            if (p_remove_suffix) {
                _pack_files<true, true, true>(p_read_path, p_write_path);
            }
            else {
                _pack_files<true, true, false>(p_read_path, p_write_path);
            }
        }
        else {
            if (p_remove_suffix) {
                _pack_files<true, false, true>(p_read_path, p_write_path);
            }
            else {
                _pack_files<true, false, false>(p_read_path, p_write_path);
            }
        }
    }
    else {
        if (p_delete_old) {
            if (p_remove_suffix) {
                _pack_files<false, true, true>(p_read_path, p_write_path);
            }
            else {
                _pack_files<false, true, false>(p_read_path, p_write_path);
            }
        }
        else {
            if (p_remove_suffix) {
                _pack_files<false, false, true>(p_read_path, p_write_path);
            }
            else {
                _pack_files<false, false, false>(p_read_path, p_write_path);
            }
        }
    }

#ifdef PACKER_LOG_ENABLED
    log_stream.close();
#endif //PACKER_LOG_ENABLED

}

Packer::Packer() {
#ifdef PACKER_IGNORE_FILE
    ignore_file_name = PACKER_IGNORE_FILE_NAME;
#endif //PACKER_IGNORE_FILE

#ifdef PACKER_LOG_ENABLED
    log_file_path = PACKER_LOG_PATH;
#endif //PACKER_LOG_ENABLED
}
