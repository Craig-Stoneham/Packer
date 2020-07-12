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

#ifndef INCLUDE_PACKER
#define INCLUDE_PACKER

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
**  Define the current version
*/
#define PACKER_VERSION "1.1"

/*
**  Define this check for an ignore file when scanning folders
*/
#define PACKER_IGNORE_FILE

/*
**  If a file with this name exists in a folder then the folder (and subfolders) will be ignored
*/
#define PACKER_IGNORE_FILE_NAME ".pkignore" 

/*
**  Define this to write the results to a log file
*/
#define PACKER_LOG_ENABLED

/*
**  The path of the log file
*/
#define PACKER_LOG_PATH      "packer_log.txt"

/*
**  Define this to transform the extension to lower case when writing
*/
#define PACKER_WRITE_LOWER_CASE_EXTENSIONS

/*
**  Define a Vector
*/
template <class T> using Vector = std::vector<T>;

/*
**  Define a String
*/
using String = std::string;

/*
**  Moves files from one location to another
*/
struct Packer {

    Vector<String> extensions;
    String suffix;

#ifdef PACKER_IGNORE_FILE
    String ignore_file_name;
#endif //PACKER_IGNORE_FILE

#ifdef PACKER_LOG_ENABLED
    String log_file_path;
    std::ofstream log_stream;
#endif //PACKER_LOG_ENABLED

    void pack_files(const String& p_read_path, const String& p_write_path, bool p_overwrite, bool p_delete_old, bool p_remove_suffix);

    Packer();

private:

    template<bool OVERWRITE, bool DELETE_OLD, bool REMOVE_SUFFIX>
    void _pack_files(const String& p_read_path, const String& p_write_path);

    template <bool OVERWRITE>
    __forceinline bool _can_write(const String& p_path) const;

    template <bool REMOVE_SUFFIX>
    __forceinline String _get_write_path(const String& p_read_path, const String& p_write_path) const;

    template <bool DELETE_OLD>
    __forceinline void _pack_file(const String& p_read_path, const String& p_write_path);

};

template <>
__forceinline bool Packer::_can_write<false>(const String& p_path) const {

    return _FS::exists(p_path) ? false : true;
}

template <>
__forceinline bool Packer::_can_write<true>(const String& p_path) const {

    return true;
}

template <>
__forceinline String Packer::_get_write_path<false>(const String& p_read_path, const String& p_write_path) const {

    String write_path = p_write_path;
    write_path.append(p_read_path.substr(p_read_path.find_last_of('\\')));
    return write_path;
}

template <>
__forceinline String Packer::_get_write_path<true>(const String& p_read_path, const String& p_write_path) const {

    String write_path = p_write_path;
    write_path.append(p_read_path.substr(p_read_path.find_last_of('\\')));

    size_t suffix_pos = write_path.find(suffix);
    if (suffix_pos != String::npos)
    {
        //std::cout << "Checking MATCH\n";
        write_path.erase(suffix_pos, suffix.length());
    }
    return write_path;
}
template <>
__forceinline void Packer::_pack_file<false>(const String& p_read_path, const String& p_write_path) {

    std::ifstream read_file(p_read_path, std::ios::binary);
    std::ofstream write_file(p_write_path, std::ios::binary);

    write_file << read_file.rdbuf();

    read_file.close();
    write_file.close();

#ifdef PACKER_LOG_ENABLED
    log_stream << "copying " << p_write_path << '\n';
#endif //PACKER_LOG_ENABLED
}

template <>
__forceinline void Packer::_pack_file<true>(const String& p_read_path, const String& p_write_path) {

    _pack_file<false>(p_read_path, p_write_path);
    std::remove(p_read_path.c_str());
}

template <bool OVERWRITE, bool DELETE_OLD, bool REMOVE_SUFFIX>
void Packer::_pack_files(const String& p_read_path, const String& p_write_path) {

    String extension;

#ifdef PACKER_IGNORE_FILE
    for (auto& path : _FS::directory_iterator(p_read_path)) {

        if (_FS::is_directory(path) == false) {
            extension = path.path().string();
            if (extension.substr(extension.find_last_of('\\') + 1) == ignore_file_name) {

                return;
            }
        }
    }
#endif //PACKER_IGNORE_FILE

    String read_path;
    String write_path;
    bool directory_exists = false;

    for (auto& path : _FS::directory_iterator(p_read_path)) {

        read_path = path.path().string();

        if (_FS::is_directory(path)) {

            write_path = _get_write_path<false>(read_path, p_write_path);
            _pack_files<OVERWRITE, DELETE_OLD, REMOVE_SUFFIX>(read_path, write_path);
        }
        else {

            extension = read_path.substr(read_path.find_last_of('.') + 1);

            for (size_t index = 0; index < extensions.size(); ++index) {

                if (extensions[index].find(extension) != (-1)) {

                    write_path = _get_write_path<REMOVE_SUFFIX>(read_path, p_write_path);

                    if (_can_write<OVERWRITE>(write_path)) {

                        if (directory_exists == false) {

                            if (_FS::exists(p_write_path) == false) {

                                Vector<String> dir_tree;
                                String new_dir = p_write_path;

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
                                    for (size_t index = 0; index < dir_tree.size(); ++index) {
                                        _FS::create_directory(dir_tree[index]);
                                    }
                                }
                                _FS::create_directory(p_write_path);
                                directory_exists = true;
                            }
                            if (_FS::exists(p_write_path) == false) {

                                std::vector<std::string> dir_tree;
                                std::string new_dir = p_write_path;

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
                                _FS::create_directory(p_write_path);
                            }
                            directory_exists = true;
                        }
#ifdef PACKER_WRITE_LOWER_CASE_EXTENSIONS
                        for (size_t index = write_path.find_last_of('.') + 1; index < write_path.size(); ++index) {
                            write_path[index] = ::tolower(write_path[index]);
                        }
#endif // PACKER_WRITE_LOWER_CASE_EXTENSIONS
                        _pack_file<DELETE_OLD>(read_path, write_path);
                        break;
                    }
                }
            }
        }
    }
}

#endif //INCLUDE_PACKER
