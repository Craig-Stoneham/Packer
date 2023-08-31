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

#include "Typedefs.h"

class Application : public Console {

    using Function = void (Application::*)();

    struct Command {
        Function function;
        String name;
        String description;
        String prompt;
        bool has_prompt;
        bool hidden;
    };

    Packer packer;
    String input;
    Vector <Command> commands;
    bool process_commands;

#ifndef CONSOLE_FEATURES_DISABLED
    Color command_text_color;
#endif // CONSOLE_FEATURES_DISABLED

#ifndef LOG_DISABLED
    String log_file_name;
#endif // LOG_DISABLED

    void _add_command(Function p_function, const String& p_name, const String& p_description, bool p_hidden = false);
    void _add_command(Function p_function, const String& p_name, const String& p_description, const String& p_prompt, bool p_hidden = false);

    // Commands

    void _set_read_path();
    void _set_write_path();
    void _add_extension();
    void _remove_extension();
    void _clear_extensions();
    void _set_pack_everything();
    void _set_overwrite_files();
    void _set_move_files();
    void _set_suffix_string();
    void _set_suffix_enabled();
    void _set_extension_insensitive();
    void _set_extension_adjust();
#ifndef IGNORE_FILE_DISABLED
    void _set_ignore_file_name();
    void _set_ignore_file_enabled();
#endif // IGNORE_FILE_DISABLED
#ifndef LOG_DISABLED
    void _set_log_file_name();
    void _set_log_enabled();
#endif // LOG_DISABLED
    void _revert_state();
    void _load_config();
    void _save_config();
    void _print_info();
    void _print_help();
    void _run_packer();
    void _quit();

    Error _save(const String& p_path);
    Error _load(const String& p_path);

public:
    void read_input(bool p_lower_case = false);

    void to_config_file(ConfigFile& p_file) const;
    void from_config_file(const ConfigFile& p_file);

    Error save(const String& p_path) const;
    Error load(const String& p_path);

    Error save_encrypted(const String& p_path, const CryptoKey& p_key) const;
    Error load_encrypted(const String& p_path, const CryptoKey& p_key);

    int run();

    Application();
};
