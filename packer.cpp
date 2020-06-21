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

/*
**  The config file path
*/
#define PACKER_CONFIG_PATH "config.pk" 

/*
**  Run the packer
**  Read from the app location and the write location in PACKER_CONFIG_PATH
*/
int main()
{
    std::string read_path = _FS::current_path().string();    
    std::ifstream config_stream(PACKER_CONFIG_PATH);

    if (config_stream.is_open() == false) {

        std::cout << "Failed to find config file.\n";
        std::cout << "Add a text file named " << PACKER_CONFIG_PATH << " with the location you want to write to.\n";
        std::system("pause");
        return 0;
    }

    std::string console_input;
    Packer packer;
    bool overwrite = false;
    std::string write_path((std::istreambuf_iterator<char>(config_stream)), (std::istreambuf_iterator<char>()));

    std::cout << "Read from " << read_path << '\n';
    std::cout << "Write to " << write_path << "\n\n";

    std::cout << "Would you like to overwrite old files?\n";
    std::cin >> console_input;
    if (console_input[0] == 'y') {

        overwrite = true;
    }

    std::cout << "Add an extension\n";
    for (;;) {

        std::cin >> console_input;

        std::transform(console_input.begin(), console_input.end(), console_input.begin(), ::tolower);
        packer.add_pack_extension(console_input);

        std::cout << "Would you like to add another extension?\n";
        std::cin >> console_input;
        if (console_input[0] != 'y') {

            break;
        }
        std::cout << "Add another extension\n";
    }

    std::cout << "\n\n";

    packer.pack_directories(read_path, write_path, overwrite);

#ifdef PACKER_LOG_ENABLED
    std::ifstream log_stream;
    log_stream.open(PACKER_LOG_PATH, std::ios::binary);
    std::cout << log_stream.rdbuf();
    log_stream.close();
#endif //PACKER_LOG_ENABLED

    std::system("pause");
}
