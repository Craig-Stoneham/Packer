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

#include "Application.h"
#include "Defaults.h"

void Application::_add_command(Function p_function, const String& p_name, const String& p_description, bool p_hidden) {
    Command command;
    command.function = p_function;
    command.name = p_name;
    command.description = p_description;
    command.has_prompt = false;
    command.hidden = p_hidden;
    commands.push_back(command);
}

void Application::_add_command(Function p_function, const String& p_name, const String& p_description, const String& p_prompt, bool p_hidden) {
    Command command;
    command.function = p_function;
    command.name = p_name;
    command.description = p_description;
    command.prompt = p_prompt;
    command.has_prompt = true;
    command.hidden = p_hidden;
    commands.push_back(command);
}

void Application::_set_read_path() {
    String read_path = input != "current" ? input : FileAccess::current_path().string();
    if (read_path == packer.get_read_path()) {
        print_line("Read path is already '" + read_path + "'.");
        return;
    }
    if (!FileAccess::exists(input)) {
        print_line("Path does not exist.");
        return;
    }
    packer.set_read_path(read_path);
    print_line("Read path changed to '" + read_path + "'.");
}

void Application::_set_write_path() {
    if (input == packer.get_write_path()) {
        print_line("Write path is already '" + input + "'.");
        return;
    }
    packer.set_write_path(input);
    print_line("Write path changed to '" + input + "'.");
}

void Application::_add_extension() {
    if (packer.add_extension(input)) {
        print_line("Extension '" + input + "' added.");
    } else {
        print_line("Extension '" + input + "' already exists.");
    }
}

void Application::_remove_extension() {
    if (packer.remove_extension(input)) {
        print_line("Extension '" + input + "' removed.");
    } else {
        print_line("Extension '" + input + "' does not exists.");
    }
}

void Application::_clear_extensions() {
    packer.clear_extensions();
    print_line("Extensions cleared.");
}

void Application::_set_pack_everything() {
    packer.set_pack_everything(!packer.get_pack_everything());
    print_line("Pack everything is " + String(packer.get_pack_everything() ? "enabled" : "disabled") + ".");
}

void Application::_set_overwrite_files() {
    packer.set_overwrite_files(!packer.get_overwrite_files());
    print_line("Overwrite files is " + String(packer.get_overwrite_files() ? "enabled" : "disabled") + ".");
}

void Application::_set_move_files() {
    packer.set_move_files(!packer.get_move_files());
    print_line("Move files is " + String(packer.get_move_files() ? "enabled" : "disabled") + ".");
}

void Application::_set_suffix_string() {
    packer.set_suffix_string(input);
    print_line("Suffix string changed to '" + input + "'.");
}

void Application::_set_suffix_enabled() {
    packer.set_suffix_enabled(!packer.get_suffix_enabled());
    print_line("Suffix is " + String(packer.get_suffix_enabled() ? "enabled" : "disabled") + ".");
}

void Application::_set_extension_insensitive() {
    packer.set_extension_insensitive(!packer.get_extension_insensitive());
    print_line("Extension insensitive is " + String(packer.get_extension_insensitive() ? "enabled" : "disabled") + ".");
}

void Application::_set_extension_adjust() {
    ExtensionCase ext_case = find_extension_case(input);
    if (ext_case == static_cast<ExtensionCase>(-1)) {
        print_line("Extension case '" + input + "' is invalid.");
        return;
    }
    if (ext_case == packer.get_extension_adjust()) {
        print_line("Extension case is already '" + input + "'.");
        return;
    }
    packer.set_extension_adjust(ext_case);
    print_line("Extension case changed to '" + input + "'.");
}

#ifndef IGNORE_FILE_DISABLED
void Application::_set_ignore_file_name() {
    String ignore_file_name = input != "default" ? input : DEFAULT_IGNORE_FILE_NAME;
    if (ignore_file_name == packer.get_ignore_file_name()) {
        print_line("Ignore file name is already '" + ignore_file_name + "'.");
        return;
    }
    packer.set_ignore_file_name(ignore_file_name);
    print_line("Ignore file name changed to '" + packer.get_ignore_file_name() + "'.");
}

void Application::_set_ignore_file_enabled() {
    packer.set_ignore_file_enabled(!packer.get_ignore_file_enabled());
    print_line("Ignore file is " + String(packer.get_ignore_file_enabled() ? "enabled" : "disabled") + ".");
}

#endif // IGNORE_FILE_DISABLED

#ifndef LOG_DISABLED

void Application::_set_log_file_name() {
    if (input == "default") {
        log_file_name = DEFAULT_LOG_FILE_NAME;
    } else {
        log_file_name = input;
    }
}

void Application::_set_log_enabled() {
    packer.set_log_enabled(!packer.get_log_enabled());
    print_line("Log " + String(packer.get_log_enabled() ? "enabled" : "disabled") + ".");
}

#endif // LOG_DISABLED

void Application::_revert_state() {
    auto set_value = [](auto &p_param, auto p_default) {
        p_param = p_default;
        };

#ifndef LOG_DISABLED
    set_value(log_file_name, DEFAULT_LOG_FILE_NAME);
#endif // LOG_DISABLED

    packer.revert_state();

    print_line("State reverted.");
}

void Application::_load_config() {
    String file_name = input != "default" ? input : DEFAULT_CONFIG_FILE_NAME;

    if (_load(file_name) == Error::OK) {
        print_line("Config file '" + file_name + "' loaded.");
    } else {
        print_line("Failed to load config file '" + file_name + "'.");
    }
}

void Application::_save_config() {
    String file_name = input != "default" ? input : DEFAULT_CONFIG_FILE_NAME;

    if (_save(file_name) == Error::OK) {
        print_line("Config file '" + file_name + "' saved.");
    } else {
        print_line("Failed to save config file '" + file_name + "'.");
    }
}

void Application::_print_info() {
    print_line("Read path: " + packer.get_read_path());
    print_line("Write path: " + packer.get_write_path());
    if (packer.get_extension_count()) {
        String extension_string = "Extensions: ";
        for (int i = 0; i < packer.get_extension_count(); ++i) {
            extension_string += packer.get_extension(i);
            if (i < (packer.get_extension_count() - 1)) {
                extension_string += ", ";
            }
        }
        print_line(extension_string);
    } else {
        print_line("No extensions added");
    }
    print_line("Pack everything: " + String(packer.get_pack_everything() ? "enabled" : "disabled"));
    print_line("Overwrite files: " + String(packer.get_overwrite_files() ? "enabled" : "disabled"));
    print_line("Move files: " + String(packer.get_move_files() ? "enabled" : "disabled"));
    print_line("Suffix string: " + packer.get_suffix_string());
    print_line("Suffix: " + String(packer.get_suffix_enabled() ? "enabled" : "disabled"));
    print_line("Extension insensitive: " + String(packer.get_extension_insensitive() ? "enabled" : "disabled"));
    print_line("Extension adjust: " + get_extension_case_string(packer.get_extension_adjust()));
#ifndef IGNORE_FILE_DISABLED
    print_line("Ignore file name: " + packer.get_ignore_file_name());
    print_line("Ignore file: " + String(packer.get_ignore_file_enabled() ? "enabled" : "disabled"));
#endif // IGNORE_FILE_DISABLED
#ifndef LOG_DISABLED
    print_line("Log file name: " + log_file_name);
    print_line("Log: " + String(packer.get_log_enabled() ? "enabled" : "disabled"));
#endif // LOG_DISABLED
}

void Application::_print_help() {
    print_line("Available commands:");

    size_t longest_string = 0;
    for (const auto& command : commands) {
        if (longest_string < command.name.size()) {
            longest_string = command.name.size();
        }
    }

    for (const auto& command : commands) {
        if (command.hidden) {
            continue;
        }
        print_line(command.name + String(longest_string - command.name.size(), ' ') + " - " + command.description + ".");
    }
}

void Application::_run_packer() {
#ifndef LOG_DISABLED
    LogFile log_file;
    if (packer.get_log_enabled() && log_file_name.length()) {
        log_file.open(log_file_name);
    }
#endif // LOG_DISABLED

    LOG_INFO("Running packer version " VERSION_STRING "\n");

    if (packer.get_read_path().empty()) {
        LOG_ERROR("Read path is not configured\n");
        return;
    }

    if (!FileAccess::exists(packer.get_read_path())) {
        LOG_ERROR("Read path '" + packer.get_read_path() + "' does not exist\n");
        return;
    }

    if (!FileAccess::is_directory(packer.get_read_path())) {
        LOG_ERROR("Read path '" + packer.get_read_path() + "' is not a directory\n");
        return;
    }

    if (packer.get_write_path().empty()) {
        LOG_ERROR("Write path is not configured\n");
        return;
    }

    if (packer.get_extension_count() == 0) {
        LOG_ERROR("No extensions are added\n");
        return;
    }

    LOG_INFO("Read path: " + packer.get_read_path() + "\n");
    LOG_INFO("Write path: " + packer.get_write_path() + "\n");
    if (packer.get_extension_count()) {
        String extension_string = "Extensions: ";
        for (int i = 0; i < packer.get_extension_count(); ++i) {
            extension_string += packer.get_extension(i);
            if (i < (packer.get_extension_count() - 1)) {
                extension_string += ", ";
            }
        }
        LOG_INFO(extension_string + "\n");
    }
    LOG_INFO("Pack everything: " + String(packer.get_pack_everything() ? "enabled" : "disabled") + "\n");
    LOG_INFO("Overwrite files: " + String(packer.get_overwrite_files() ? "enabled" : "disabled") + "\n");
    LOG_INFO("Move files: " + String(packer.get_move_files() ? "enabled" : "disabled") + "\n");
    LOG_INFO("Suffix string: " + packer.get_suffix_string() + "\n");
    LOG_INFO("Suffix: " + String(packer.get_suffix_enabled() ? "enabled" : "disabled") + "\n");
    if (packer.get_suffix_enabled() && packer.get_suffix_string().empty()) {
        LOG_WARN("Suffix is enabled but suffix string is invalid\n");
    }
    LOG_INFO("Extension insensitive: " + String(packer.get_extension_insensitive() ? "enabled" : "disabled") + "\n");
    LOG_INFO("Extension adjust: " + get_extension_case_string(packer.get_extension_adjust()) + "\n");
#ifndef IGNORE_FILE_DISABLED
    LOG_INFO("Ignore file name: " + packer.get_ignore_file_name() + "\n");
    LOG_INFO("Ignore file: " + String(packer.get_ignore_file_enabled() ? "enabled" : "disabled") + "\n");
    if (packer.get_ignore_file_enabled() && packer.get_ignore_file_name().empty()) {
        LOG_WARN("Ignore file is enabled but ignore file name is invalid\n");
    }
#endif // IGNORE_FILE_DISABLED
#ifndef LOG_DISABLED
    LOG_INFO("Log file name: " + log_file_name + "\n");
    LOG_INFO("Log: " + String(packer.get_log_enabled() ? "enabled" : "disabled") + "\n");
    if (packer.get_log_enabled() && log_file_name.empty()) {
        LOG_WARN("Log is enabled but log file name is invalid\n");
    }
#endif // LOG_DISABLED

    print_line("Packing files...");

    packer.pack_files();
}

void Application::_quit() {
    process_commands = false;
}

Error Application::_save(const String& p_path) {
#ifdef CONFIG_FILE_ENCRYPTION_ENABLED
    return save_encrypted(p_path, CryptoKey("Application"));
#else // CONFIG_FILE_ENCRYPTION_ENABLED
    return save(p_path);
#endif // CONFIG_FILE_ENCRYPTION_ENABLED
}

Error Application::_load(const String& p_path) {
#ifdef CONFIG_FILE_ENCRYPTION_ENABLED
    return load_encrypted(p_path, CryptoKey("Application"));
#else // CONFIG_FILE_ENCRYPTION_ENABLED
    return load(p_path);
#endif // CONFIG_FILE_ENCRYPTION_ENABLED
}

void Application::read_input(bool p_lower_case) {
#ifndef CONSOLE_FEATURES_DISABLED
    set_text_color(command_text_color);
#endif // CONSOLE_FEATURES_DISABLED

    input.clear();
    std::cin >> input;

    if (p_lower_case) {
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    }

#ifndef CONSOLE_FEATURES_DISABLED
    set_text_color(Color::White);
#endif // CONSOLE_FEATURES_DISABLED
}

void Application::to_config_file(ConfigFile& p_file) const {
    auto set_value = [&p_file](const auto& p_name, const auto& p_param) {
        p_file.set_value(p_name, p_param);
        };

#ifndef LOG_DISABLED
    set_value("log_file_name", log_file_name);
#endif // LOG_DISABLED

    packer.to_config_file(p_file);
}

void Application::from_config_file(const ConfigFile& p_file) {
    auto set_value = [&p_file](const auto& p_name, auto& p_param, auto p_default) {
        if (p_file.has_value(p_name)) {
            p_param = p_file.get_value(p_name);
        } else {
            p_param = p_default;
        }
        };

#ifndef LOG_DISABLED
    set_value("log_file_name", log_file_name, DEFAULT_LOG_FILE_NAME);
#endif // LOG_DISABLED

    packer.from_config_file(p_file);
}

Error Application::save(const String& p_path) const {
    ConfigFile config_file;
    to_config_file(config_file);
    return config_file.save(p_path);
}

Error Application::load(const String& p_path) {
    ConfigFile config_file;
    Error error = config_file.load(p_path);
    if (error != Error::OK) {
        return error;
    }
    from_config_file(config_file);
    return Error::OK;
}

Error Application::save_encrypted(const String& p_path, const CryptoKey& p_key) const {
    ConfigFile config_file;
    to_config_file(config_file);
    return config_file.save_encrypted(p_path, p_key);
}

Error Application::load_encrypted(const String& p_path, const CryptoKey& p_key) {
    ConfigFile config_file;
    Error error = config_file.load_encrypted(p_path, p_key);
    if (error != Error::OK) {
        return error;
    }
    from_config_file(config_file);
    return Error::OK;
}

int Application::run() {
    print_line("Welcome to packer version "  VERSION_STRING  ".");
    print_line("Type a command or 'help' for a list of commands:");

    _load(DEFAULT_CONFIG_FILE_NAME);

    while (process_commands) {
        read_input(true);
        for (const auto& command : commands) {
            if (command.name == input) {
                if (command.has_prompt) {
                    print_line(command.prompt);
                    read_input(false);
                }
                (this->*command.function)();
                goto command_processed;
            }
        }
        print_line("Unknown command");
    command_processed: {}
    }

    return EXIT_SUCCESS;
}

Application::Application() :
#ifndef CONSOLE_FEATURES_DISABLED
    command_text_color(DEFAULT_COMMAND_TEXT_COLOR),
#endif // CONSOLE_FEATURES_DISABLED
#ifndef LOG_DISABLED
    log_file_name(DEFAULT_LOG_FILE_NAME),
#endif // LOG_DISABLED
    process_commands(true) {
    _add_command(&Application::_set_read_path, "read_path", "Change the path that you would like to read from", "Type the path (or 'current' to use the current directory):", false);
    _add_command(&Application::_set_write_path, "write_path", "Change the path that you would like to write to", "Type the path:", false);
    _add_command(&Application::_add_extension, "add_extension", "Add an extension to the extension list", "Type the extension to add:", false);
    _add_command(&Application::_remove_extension, "remove_extension", "Remove an extension from the extension list", "Type the extension to remove:", false);
    _add_command(&Application::_clear_extensions, "clear_extensions", "Clear all of the extensions in the extension list", false);
    _add_command(&Application::_set_pack_everything, "pack_everything", "Pack everything regardless of the extension", false);
    _add_command(&Application::_set_overwrite_files, "overwrite_files", "Overwrite existing files", false);
    _add_command(&Application::_set_move_files, "move_files", "Move the files", false);
    _add_command(&Application::_set_suffix_string, "suffix_string", "The suffix string to remove", "Type the suffix string to remove:", false);
    _add_command(&Application::_set_suffix_enabled, "suffix_enabled", "Enable suffix string removal", false);
    _add_command(&Application::_set_extension_insensitive, "extension_insensitive", "Ignore extension case in the extension list", false);
    _add_command(&Application::_set_extension_adjust, "extension_adjust", "Adjust the extension case", "Type '" + get_extension_case_string(ExtensionCase::Default) + "', '" + get_extension_case_string(ExtensionCase::Lower) + "', '" + get_extension_case_string(ExtensionCase::Upper) + ":", false);
#ifndef IGNORE_FILE_DISABLED
    _add_command(&Application::_set_ignore_file_name, "ignore_file_name", "Change the name of the ignore file", "Type the name of the ignore file (or 'default' to use to the default):", false);
    _add_command(&Application::_set_ignore_file_enabled, "ignore_file_enabled", "Check for an ignore file", false);
#endif // IGNORE_FILE_DISABLED
#ifndef LOG_DISABLED
    _add_command(&Application::_set_log_file_name, "log_file_name", "Change the name of the log file", "Type the name of the log file (or 'default' to use the default):", false);
    _add_command(&Application::_set_log_enabled, "log_enabled", "Enable logging", false);
#endif // LOG_DISABLED
    _add_command(&Application::_revert_state, "revert", "Revert all of the settings to their defaults", false);
    _add_command(&Application::_load_config, "load", "Load a state from a config file", "Type the name of the config file (or 'default' to use the default):", false);
    _add_command(&Application::_save_config, "save", "Save the state to a config file", "Type the name of the config file (or 'default' to use the default):", false);
    _add_command(&Application::_print_info, "info", "Print the current state of the packer", false);
    _add_command(&Application::_print_help, "help", "", true);
    _add_command(&Application::_run_packer, "run", "Run the packer", false);
    _add_command(&Application::_quit, "quit", "Quit the application", false);
}
