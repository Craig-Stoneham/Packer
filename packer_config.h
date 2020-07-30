/*************************************************************************/
/*  packer_config.h                                                      */
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

#ifndef INCLUDE_PACKER_CONFIG
#define INCLUDE_PACKER_CONFIG

#include "packer.h"

/*
**  Define the character for a new line
*/
#define PACKER_CONFIG_NEW_LINE '\n'  

/*
**  Utility class to load and save settings from a file
*/
struct PackerConfig {

    String read_path;
    String write_path;
    Vector<String> extensions;
    bool overwrite;
    bool delete_old;
    String suffix;

#ifdef PACKER_IGNORE_FILE
    String ignore_file_name;
#endif //PACKER_IGNORE_FILE

    bool load(const String& p_path);
    void load(std::istream& p_stream);
    void load(const Packer& p_packer);

    bool save(const String& p_path);
    void save(std::ostream& p_stream);
    void save(Packer& p_packer);

    void validate();
    void set_to_default();

    PackerConfig();
};

#endif //INCLUDE_PACKER_CONFIG
