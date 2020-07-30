/*************************************************************************/
/*  packer_config.cpp                                                    */
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

#include "packer_config.h"

/*
**  Default values
*/
#define PACKER_DEFAULT_OVERWRITE false
#define PACKER_DEFAULT_DELETE_OLD false

/*
**  Remove whitespace from the head and tail
*/
static void remove_outer_whitespace(String& p_string) {

    size_t start = 0;
    size_t string_length = p_string.length();
    size_t new_length = string_length;
    int index = 0;

    for (index = 0; index < string_length; ++index) {
        if ((p_string[index] == '\0') || (p_string[index] == ' ')) {
            ++start;
        }
        else {
            break;
        }
    }
    for (index = (int)string_length - 1; index > 0; --index) {
        if ((p_string[index] == '\0') || (p_string[index] == ' ')) {
            --new_length;
        }
        else {
            break;
        }
    }

    if (start) {
        p_string = p_string.substr(start, new_length);
    }
    else {
        p_string.resize(new_length);
    }
}

bool PackerConfig::load(const String& p_path) {

    std::ifstream file_stream(p_path, PACKER_OPEN_MODE);

    if (!file_stream.is_open()) {
        return false;
    }
    load(file_stream);
    file_stream.close();

    return true;
}

void PackerConfig::load(std::istream& p_stream) {

    set_to_default();

    String file_data((std::istreambuf_iterator<char>(p_stream)), (std::istreambuf_iterator<char>()));
    String line;
    String name;
    String value;
    size_t equal_pos = 0;
    size_t file_data_length = file_data.length() + 1;
    size_t line_length_accum = 0;
    size_t line_length = 0;
    size_t index = 0;

    for (index = 0; index <= file_data_length; ++index) {

        if (file_data[index] == PACKER_CONFIG_NEW_LINE) {

            line = file_data.substr(line_length_accum, line_length);
            line_length_accum += (line_length + 1);
            line_length = 0;

            equal_pos = line.find_first_of('=');

            if (equal_pos == String::npos) {
                continue;
            }
            for (size_t ii = 0; ii <= line.length(); ++ii) {
                if (line[ii] == '\r') {
                    line[ii] = 0;
                }
            }

            name = line.substr(0, equal_pos);
            value = line.substr(equal_pos + 1);

            remove_outer_whitespace(name);
            remove_outer_whitespace(value);

            if (name == "read") {
                read_path = value;
            }
            else if (name == "write") {
                write_path = value;
            }
            else if (name == "ext") {
                extensions.push_back(value);
            }
            else if (name == "overwrite") {
                overwrite = (value == "true") ? true : false;
            }
            else if (name == "delete") {
                delete_old = (value == "true") ? true : false;
            }
            else if (name == "suffix") {
                suffix = value;
            }
#ifdef PACKER_IGNORE_FILE
            else if (name == "ignore") {
                ignore_file_name = value;
            }
#endif //PACKER_IGNORE_FILE
        }
        else {
            ++line_length;
        }
    }
}

void PackerConfig::load(const Packer& p_packer) {

    extensions = p_packer.extensions;
    suffix = p_packer.suffix;

#ifdef PACKER_IGNORE_FILE
    ignore_file_name = p_packer.ignore_file_name;
#endif //PACKER_IGNORE_FILE
}

bool PackerConfig::save(const String& p_path) {

    std::ofstream file_stream(p_path, PACKER_OPEN_MODE);

    if (!file_stream.is_open()) {
        return false;
    }

    save(file_stream);
    file_stream.close();
    return true;
}

void PackerConfig::save(std::ostream& p_stream) {

    validate();

    p_stream << "read=" << read_path << PACKER_CONFIG_NEW_LINE;
    p_stream << "write=" << write_path << PACKER_CONFIG_NEW_LINE;
    if (extensions.size()) {
        for (uint32_t index = 0; index < extensions.size(); ++index) {
            p_stream << "ext=" << extensions[index] << PACKER_CONFIG_NEW_LINE;
        }
    }
    p_stream << "overwrite=" << (overwrite ? "true" : "false") << PACKER_CONFIG_NEW_LINE;
    p_stream << "delete=" << (delete_old ? "true" : "false") << PACKER_CONFIG_NEW_LINE;
    p_stream << "suffix=" << suffix << PACKER_CONFIG_NEW_LINE;
#ifdef PACKER_IGNORE_FILE
    p_stream << "ignore=" << ignore_file_name << PACKER_CONFIG_NEW_LINE;
#endif //PACKER_IGNORE_FILE
}

void PackerConfig::save(Packer& p_packer) {

    validate();

    p_packer.extensions = extensions;
    p_packer.suffix = suffix;

#ifdef PACKER_IGNORE_FILE
    p_packer.ignore_file_name = ignore_file_name;
#endif //PACKER_IGNORE_FILE
}

void PackerConfig::validate() {

    remove_outer_whitespace(read_path);
    remove_outer_whitespace(write_path);
    for (size_t index = 0; index < extensions.size(); ++index) {
        remove_outer_whitespace(extensions.at(index));
    }
    remove_outer_whitespace(suffix);
#ifdef PACKER_IGNORE_FILE
    remove_outer_whitespace(ignore_file_name);
#endif //PACKER_IGNORE_FILE
}

void PackerConfig::set_to_default() {

    extensions.clear();
    read_path = _FS::current_path().string();
    write_path.clear();
    overwrite = PACKER_DEFAULT_OVERWRITE;
    delete_old = PACKER_DEFAULT_DELETE_OLD;
    suffix.clear();
#ifdef PACKER_IGNORE_FILE
    ignore_file_name = PACKER_IGNORE_FILE_NAME;
#endif //PACKER_IGNORE_FILE
}

PackerConfig::PackerConfig() {

    overwrite = PACKER_DEFAULT_OVERWRITE;
    delete_old = PACKER_DEFAULT_DELETE_OLD;
#ifdef PACKER_IGNORE_FILE
    ignore_file_name = PACKER_IGNORE_FILE_NAME;
#endif //PACKER_IGNORE_FILE
}
