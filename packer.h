/*************************************************************************/
/*  packer.h                                                             */
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

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#if !_HAS_CXX17
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#define _FS std::experimental::filesystem
#else //_HAS_CXX17
#include <filesystem>
#define _FS std::filesystem
#endif //_HAS_CXX17

/*
**  If a file with this name exists in a folder then the folder (and subfolders) will be ignored 
*/
#define PACKER_IGNORE_STRING ".pkignore" 

/*
**  Enable this to write the results to a log file
*/
#define PACKER_LOG_ENABLED

/*
**  The path of the log file
*/
#define PACKER_LOG_PATH      "packer_log.txt"

/*
**  Define this to transform the extension to lower case
*/
#define PACKER_LOWER_CASE_EXTENSION


/*
**  Used to copy files from one location to another
*/
class Packer {

    __forceinline static bool exists(const std::string& p_path);
    __forceinline static void create_directory_nocheck(const std::string& p_path);
    __forceinline static void create_directory(const std::string& p_path);
    __forceinline static std::string get_extension_casesens(const std::string& p_path);
    __forceinline static std::string get_extension(const std::string& p_path);

    template <const int BASE = std::ios::binary>
    __forceinline void copy_file_nocheck(const std::string& p_read_path, const std::string& p_write_path);
    
    __forceinline bool has_ignore_file(const std::string& p_dir_path) const;
    __forceinline bool has_pack_extension(const std::string& p_path) const;
    __forceinline bool can_pack(const std::string& p_path) const;

    void pack_directories(const std::string& p_read_path, const std::string& p_write_path);

public:

    __forceinline void set_ignore_file(const std::string& p_ignore_file);
    __forceinline const std::string& get_ignore_file() const;
    __forceinline const std::vector<std::string>& get_pack_extensions() const;
    __forceinline void add_pack_extension(const std::string& extension);
    __forceinline void remove_pack_extension(const std::string& extension);
    void pack_directories(const std::string& p_read_path, const std::string& p_write_path, bool p_overwrite);

    Packer();

private:
    std::vector<std::string> pack_extensions;
    std::string ignore_file;
    bool pack_overwrite;
#ifdef PACKER_LOG_ENABLED
    std::ofstream log_stream;
#endif //PACKER_LOG_ENABLED

};

bool Packer::exists(const std::string& p_path) {

    return _FS::exists(p_path);
}

void Packer::create_directory_nocheck(const std::string& p_path) {

    _FS::create_directory(p_path);
}

void Packer::create_directory(const std::string& p_path) {

    if (_FS::exists(p_path) == false) {

        std::vector<std::string> dir_tree;
        std::string new_dir = p_path;

        for (;;) {

            new_dir.resize(new_dir.find_last_of('\\'));
            if (_FS::exists(new_dir)) {

                break;
            }
            else {

                dir_tree.push_back(new_dir);
            }
        }

        if (!dir_tree.empty()) {

            for (auto iter = dir_tree.rbegin(); iter != dir_tree.rend(); ++iter) {

                _FS::create_directory(*iter);
            }
        }
        create_directory_nocheck(p_path);
    }
}

std::string Packer::get_extension_casesens(const std::string& p_path) {

    return p_path.substr(p_path.find_last_of('.') + 1);
}

std::string Packer::get_extension(const std::string& p_path) {

    std::string extension = p_path.substr(p_path.find_last_of('.') + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    return extension;
}

template <const int BASE>
void Packer::copy_file_nocheck(const std::string& p_read_path, const std::string& p_write_path) {

    std::ifstream read_file(p_read_path, BASE);
    std::ofstream write_file(p_write_path, BASE);
    write_file << read_file.rdbuf();
#ifdef PACKER_LOG_ENABLED
    log_stream << "copying " << p_write_path << '\n';
#endif //PACKER_LOG_ENABLED
}

bool Packer::has_ignore_file(const std::string& p_dir_path) const {

    std::string extension;

    for (auto& path : _FS::directory_iterator(p_dir_path)) {

        if (_FS::is_directory(path) == false) {

            extension = path.path().string();
            if (extension.substr(extension.find_last_of('\\') + 1) == ignore_file) {

                return true;
            }
        }
    }
    return false;
}

bool Packer::has_pack_extension(const std::string& p_path) const {

    std::string extension = get_extension(p_path);

    for (size_t index = 0; index < pack_extensions.size(); ++index) {

        if (extension == pack_extensions[index]) {

            return true;
        }
    }
    return false;
}

bool Packer::can_pack(const std::string& p_path) const {

    return ((pack_overwrite == false) && (_FS::exists(p_path))) ? false : true;
}

void Packer::pack_directories(const std::string& p_read_path, const std::string& p_write_path) {

    if (has_ignore_file(p_read_path) == false) {

        std::string read_path;
        std::string write_path;
        bool directory_exists = _FS::exists(p_write_path);

        for (auto& path : _FS::directory_iterator(p_read_path)) {

            read_path = path.path().string();
            write_path = p_write_path;
            write_path.append(read_path.substr(read_path.find_last_of('\\')));

            if (_FS::is_directory(path)) {

                pack_directories(read_path, write_path);
            }
            else {

                if (has_pack_extension(read_path) && can_pack(write_path)) {

                    if (directory_exists == false) {

                        create_directory(p_write_path);
                        directory_exists = true;
                    }
#ifdef PACKER_LOWER_CASE_EXTENSION
                    for (size_t index = write_path.find_last_of('.') + 1; index < write_path.size(); ++index) {
                        write_path[index] = ::tolower(write_path[index]);
                    }
#endif // PACKER_LOWER_CASE_EXTENSION

                    copy_file_nocheck(read_path, write_path);
                }
            }
        }
    }
}

void Packer::set_ignore_file(const std::string& p_ignore_file) {

    ignore_file = p_ignore_file;
}

const std::string& Packer::get_ignore_file() const {

    return ignore_file;
}

const std::vector<std::string>& Packer::get_pack_extensions() const {

    return pack_extensions;
}

void Packer::add_pack_extension(const std::string& extension) {


    for (std::vector<std::string>::iterator iter = pack_extensions.begin(); iter != pack_extensions.end(); ++iter) {

        if (extension == (*iter)) {

            return;
        }
    }
    if (extension[0] == '.') {

        pack_extensions.push_back(extension.substr(1));
    }
    else {

        pack_extensions.push_back(extension);
    }
}

void Packer::remove_pack_extension(const std::string& extension) {

    for (std::vector<std::string>::iterator iter = pack_extensions.begin(); iter != pack_extensions.end(); ++iter) {

        if (extension == (*iter)) {

            pack_extensions.erase(iter);
            return;
        }
    }
}

void Packer::pack_directories(const std::string& p_read_path, const std::string& p_write_path, bool p_overwrite) {

#ifdef PACKER_LOG_ENABLED
    log_stream.open(PACKER_LOG_PATH, std::ios::binary);
    log_stream << "Looking for " << pack_extensions[0];
    for (size_t i = 1; i < pack_extensions.size(); ++i) {

        log_stream << ", " << pack_extensions[i];
    }
    log_stream << '\n';
#endif //PACKER_LOG_ENABLED

    pack_overwrite = p_overwrite;
    pack_directories(p_read_path, p_write_path);

#ifdef PACKER_LOG_ENABLED
    log_stream.close();
#endif //PACKER_LOG_ENABLED
}

Packer::Packer() {

    ignore_file = PACKER_IGNORE_STRING;
    pack_overwrite = false;
}
