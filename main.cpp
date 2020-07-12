/*************************************************************************/
/*  main.cpp                                                             */
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
**  The config file path
*/
#define PACKER_CONFIG_PATH "config.pk" 

/*
**  Run the packer using the terminal
*/
int main()
{
    Packer packer;
    PackerConfig packer_config;
    String console_input;
    bool reverse_mode = false;

    std::cout << "Packer version " << PACKER_VERSION << ".\n";
    std::cout << "Type run or help for a list of options\n";

    if (packer_config.load(PACKER_CONFIG_PATH) == false) {
        packer_config.save(PACKER_CONFIG_PATH);
    }

    for (;;) {

        console_input.clear();
        std::cin >> console_input;

        if (console_input == "add") {
            bool ext_added = false;
            std::cout << "Type the extension to add:\n";
            console_input.clear();
            std::cin >> console_input;

            for (size_t index = 0; index < packer_config.extensions.size(); ++index) {
                if (console_input == packer_config.extensions[index]) {
                    std::cout << console_input << " extension already added\n";
                    ext_added = true;
                    break;
                }
            }
            if (ext_added == false) {
                packer_config.extensions.push_back(console_input);
                std::cout << console_input << " extension added\n";
            } 
        }
        else if (console_input == "remove") {
            bool ext_removed = false;
            std::cout << "Type the extension to remove:\n";
            console_input.clear();
            std::cin >> console_input;
            for (size_t index = 0; index < packer_config.extensions.size(); ++index) {
                if (console_input == packer_config.extensions[index]) {
                    packer_config.extensions.erase(packer_config.extensions.begin() + index);
                    std::cout << console_input << " extension removed\n";
                    ext_removed = true;
                    break;
                }
            }
            if (ext_removed == false) {
                std::cout << console_input << " extension not found in database\n";
            }
        }
        else if (console_input == "overwrite") {
            packer_config.overwrite = !packer_config.overwrite;
            std::cout << "Overwrite files " << (packer_config.overwrite ? "enabled" : "disabled") << '\n';
        }
        else if (console_input == "delete") {
            packer_config.delete_old = !packer_config.delete_old;
            std::cout << "Delete moved files " << (packer_config.delete_old ? "enabled" : "disabled") << '\n';
        }
        else if (console_input == "read") {
            std::cout << "Type the read path(or current to use the current directory):\n";
            console_input.clear();
            std::cin >> console_input;
            if (console_input == "current") {
                packer_config.read_path = _FS::current_path().string();
                std::cout << "Read path changed to " << packer_config.read_path << "\n";
            }
            else {
                if (_FS::exists(console_input)) {
                    packer_config.read_path = console_input;
                    std::cout << "Read path changed to " << packer_config.read_path << "\n";
                }
                else {
                    std::cout << "Ivalide read path " << packer_config.read_path << "\n";
                }
            }
        }
        else if (console_input == "write") {
            std::cout << "Type the write path:\n";
            console_input.clear();
            std::cin >> console_input;
            packer_config.write_path = console_input;
            std::cout << "Write path changed to " << packer_config.write_path << "\n";
            if (_FS::exists(console_input) == false) {
                std::cout << "Warning: The write path " << packer_config.write_path << " does not currently exist\n";
            }
        }
        else if (console_input == "suffix") {

            if (packer_config.suffix.length()) {
                std::cout << "The suffix is currently set to " << packer_config.suffix << ". Type the suffix, or remove to disable it:\n";
            }
            else {
                std::cout << "Type the suffix:\n";
            }
            console_input.clear();
            std::cin >> console_input;
            packer_config.suffix = console_input;
            if (console_input == "remove") {
                packer_config.suffix.clear();
                std::cout << "Suffix removed\n";
                continue;
            }
            else {
                packer_config.suffix = console_input;
                std::cout << "Suffix changed to " << packer_config.suffix << "\n";
            }
        }
        else if (console_input == "ignore") {
            std::cout << "Type the ignore file:\n";
            console_input.clear();
            std::cin >> console_input;
            packer_config.ignore_file_name = console_input;
            std::cout << "Ignore file changed to " << packer_config.ignore_file_name << "\n";
        }
        else if (console_input == "reverse") {
            reverse_mode = !reverse_mode;
            std::cout << "Reverse mode " << (packer_config.delete_old ? "enabled" : "disabled") << '\n';
        }
        else if (console_input == "reset") {
            packer_config.reset_data();
            std::cout << "Config file reset\n";
        }
        else if (console_input == "load") {
            if (packer_config.load(PACKER_CONFIG_PATH)) {
                std::cout << "Config file loaded\n";
            }           
        }
        else if (console_input == "save") {
            if (packer_config.save(PACKER_CONFIG_PATH)) {
                std::cout << "Config file saved\n";
            }
        }
        else if (console_input == "config") {
            packer_config.save(std::cout);
            std::cout << "reverse=" << (reverse_mode ? "true" : "false") << PACKER_CONFIG_NEW_LINE;
        }
        else if (console_input == "run") {
            if (packer_config.extensions.size() == 0) {
                std::cout << "No extensions are added\n";
            }
            if (_FS::exists(packer_config.read_path) == false) {

                std::cout << "Invalid read path\n";
            }
            else if (packer_config.write_path.length() == 0) {

                std::cout << "Invalid write path\n";
            }
            else {
                if (reverse_mode && packer_config.delete_old) {
                    std::cout << "Warning. Reverse and delete are both enabled. Files in your source directory will be deleted.\nType ok keep overwrite enabled:";
                    console_input.clear();
                    std::cin >> console_input;
                    if (console_input != "ok") {
                        packer_config.delete_old = false;
                        std::cout << "Overwrite disabled\n";
                    }
                }
                std::cout << "Running packer\n";
                break;
            }            
        }
        else if (console_input == "help") {

            std::cout << "add - add an extension\n";
            std::cout << "remove - remove an extension\n";
            std::cout << "overwrite - overwrite old files\n";
            std::cout << "delete - delete old files\n";
            std::cout << "read - change the read path\n";
            std::cout << "write - change the write path\n";
            std::cout << "suffix - change the suffix to remove\n";
            std::cout << "ignore - change the ignore file name\n";
            std::cout << "reverse - reverse the operation\n";
            std::cout << "reset - reset configuartion\n";
            std::cout << "load - load a configuartion\n";
            std::cout << "save - save the current configuration\n";
            std::cout << "config - print the current configuration\n";
            std::cout << "run - run the packer\n";

        }
        else if ((console_input == "exit") || (console_input == "quit")) {
            return -1;
        }
        else {
            std::cout << "Unrcognised command\n";
        }
    }
    std::cout << "\n";

    packer_config.save(packer);

    if (reverse_mode) {
        packer.pack_files(packer_config.write_path, packer_config.read_path, packer_config.overwrite, packer_config.delete_old, packer_config.suffix.length() ? true : false);
    }
    else {
        packer.pack_files(packer_config.read_path, packer_config.write_path, packer_config.overwrite, packer_config.delete_old, packer_config.suffix.length() ? true : false);
    }

#ifdef PACKER_LOG_ENABLED
    std::ifstream log_stream;
    log_stream.clear();
    log_stream.open(PACKER_LOG_PATH, std::ios::binary);
    std::cout << log_stream.rdbuf();
    log_stream.close();
#endif //PACKER_LOG_ENABLED

    system("pause");
}
