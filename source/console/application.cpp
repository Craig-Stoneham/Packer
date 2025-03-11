// See LICENSE for full copyright and licensing information.

#include "application.h"

PACKER_NAMESPACE_BEGIN

void Application::_add_hidden_command(Function p_function, const String& p_name) {
    Command command;
    command.type = Command::Hidden;
    command.function = p_function;
    command.name = p_name;
    commands.push_back(command);
}

void Application::_add_simple_command(Function p_function, const String& p_name, const String& p_description) {
    Command command;
    command.type = Command::Simple;
    command.function = p_function;
    command.name = p_name;
    command.description = p_description;
    commands.push_back(command);
}

void Application::_add_prompt_command(Function p_function, const String& p_name, const String& p_description, const String& p_prompt) {
    Command command;
    command.type = Command::Prompt;
    command.function = p_function;
    command.name = p_name;
    command.description = p_description;
    command.prompt = p_prompt;
    commands.push_back(command);
}

void Application::_set_read_path() {
    String read_path = input != "current" ? input : FileAccess::current_path().string();
    if (read_path == packer.get_read_path()) {
        print_line("Read path is already '" + read_path + "'.");
        return;
    }
    if (read_path == packer.get_write_path()) {
        print_line("Read path '" + read_path + "' cannot be the same as the write path.");
        return;
    }
    if (FileAccess::exists(read_path) == false) {
        print_line("Path does not exist.");
        return;
    }
    packer.set_read_path(read_path);
    print_line("Read path changed to '" + read_path + "'.");
}

void Application::_set_write_path() {
    String write_path = input;
    if (write_path == packer.get_write_path()) {
        print_line("Write path is already '" + write_path + "'.");
        return;
    }
    if (write_path == packer.get_read_path()) {
        print_line("Write path '" + write_path + "' cannot be the same as the read path.");
        return;
    }
    packer.set_write_path(write_path);
    print_line("Write path changed to '" + write_path + "'.");
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

void Application::_set_pack_mode() {
    Packer::PackMode pack_mode = Packer::find_pack_mode(input);
    if (pack_mode == Packer::PackMode::Unknown) {
        print_line("Pack mode '" + input + "' is invalid.");
        return;
    }
    if (pack_mode == packer.get_pack_mode()) {
        print_line("Pack mode is already '" + input + "'.");
        return;
    }
    packer.set_pack_mode(pack_mode);
    print_line("Pack mode changed to '" + input + "'.");
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
    Packer::ExtensionAdjust ext_case = Packer::find_extension_adjust(input);
    if (ext_case == Packer::ExtensionAdjust::Unknown) {
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

#ifdef IGNORE_FILE_ENABLED
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

#endif // IGNORE_FILE_ENABLED

#ifdef LOG_ENABLED

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

#endif // LOG_ENABLED

void Application::_swap_paths() {
	const String& read_path = packer.get_read_path();
	const String& write_path = packer.get_write_path();
    if (FileAccess::exists(write_path) == false) {
        print_line("Wrtie path does not exist.");
        return;
    }
    packer.set_read_path(write_path);
    packer.set_write_path(read_path);
    print_line("Paths swapped.");
}

void Application::_revert_state() {
#ifdef LOG_ENABLED
    log_file_name = DEFAULT_LOG_FILE_NAME;
#endif // LOG_ENABLED

    packer.revert_state();

    print_line("State reverted.");
}

void Application::_save_config() {
    String file_name = input != "default" ? input : DEFAULT_CONFIG_FILE_NAME;

    if (_save(file_name) == Error::OK) {
        print_line("Config file '" + file_name + "' saved.");
    } else {
        print_line("Failed to save config file '" + file_name + "'.");
    }
}

void Application::_load_config() {
    String file_name = input != "default" ? input : DEFAULT_CONFIG_FILE_NAME;

    if (_load(file_name) == Error::OK) {
        print_line("Config file '" + file_name + "' loaded.");
    } else {
        print_line("Failed to load config file '" + file_name + "'.");
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
    print_line("Pack mode: " + Packer::get_pack_mode_name(packer.get_pack_mode()));
    print_line("Overwrite files: " + String(packer.get_overwrite_files() ? "enabled" : "disabled"));
    print_line("Move files: " + String(packer.get_move_files() ? "enabled" : "disabled"));
    print_line("Suffix string: " + packer.get_suffix_string());
    print_line("Suffix: " + String(packer.get_suffix_enabled() ? "enabled" : "disabled"));
    print_line("Extension insensitive: " + String(packer.get_extension_insensitive() ? "enabled" : "disabled"));
    print_line("Extension adjust: " + Packer::get_extension_adjust_name(packer.get_extension_adjust()));
#ifdef IGNORE_FILE_ENABLED
    print_line("Ignore file name: " + packer.get_ignore_file_name());
    print_line("Ignore file: " + String(packer.get_ignore_file_enabled() ? "enabled" : "disabled"));
#endif // IGNORE_FILE_ENABLED
#ifdef LOG_ENABLED
    print_line("Log file name: " + log_file_name);
    print_line("Log: " + String(packer.get_log_enabled() ? "enabled" : "disabled"));
#endif // LOG_ENABLED
}

void Application::_run_packer() {
#ifdef LOG_ENABLED
    LogFile log_file;
    if (packer.get_log_enabled() && log_file_name.length()) {
        log_file.open(log_file_name);
    }
#endif // LOG_ENABLED

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
    if (packer.get_pack_mode() == Packer::PackMode::Include) {
        if (packer.get_extension_count() == 0) {
            LOG_ERROR("No extensions are added\n");
            return;
        }
    }

    LOG_INFO("Running packer version " VERSION_STRING "\n");

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
    LOG_INFO("Pack mode: " + Packer::get_pack_mode_name(packer.get_pack_mode()) + "\n");
    LOG_INFO("Overwrite files: " + String(packer.get_overwrite_files() ? "enabled" : "disabled") + "\n");
    LOG_INFO("Move files: " + String(packer.get_move_files() ? "enabled" : "disabled") + "\n");
    LOG_INFO("Suffix string: " + packer.get_suffix_string() + "\n");
    LOG_INFO("Suffix: " + String(packer.get_suffix_enabled() ? "enabled" : "disabled") + "\n");
    if (packer.get_suffix_enabled() && packer.get_suffix_string().empty()) {
        LOG_WARN("Suffix is enabled but suffix string is invalid\n");
    }
    LOG_INFO("Extension insensitive: " + String(packer.get_extension_insensitive() ? "enabled" : "disabled") + "\n");
    LOG_INFO("Extension adjust: " + Packer::get_extension_adjust_name(packer.get_extension_adjust()) + "\n");
#ifdef IGNORE_FILE_ENABLED
    LOG_INFO("Ignore file name: " + packer.get_ignore_file_name() + "\n");
    LOG_INFO("Ignore file: " + String(packer.get_ignore_file_enabled() ? "enabled" : "disabled") + "\n");
    if (packer.get_ignore_file_enabled() && packer.get_ignore_file_name().empty()) {
        LOG_WARN("Ignore file is enabled but ignore file name is invalid\n");
    }
#endif // IGNORE_FILE_ENABLED
#ifdef LOG_ENABLED
    LOG_INFO("Log file name: " + log_file_name + "\n");
    LOG_INFO("Log: " + String(packer.get_log_enabled() ? "enabled" : "disabled") + "\n");
    if (packer.get_log_enabled() && log_file_name.empty()) {
        LOG_WARN("Log is enabled but log file name is invalid\n");
    }
#endif // LOG_ENABLED

    print_line("Packing files...");

    packer.pack_files();

    print_line("Finished packing");
}

void Application::_quit_program() {
    process_commands = false;
}

void Application::_print_help() {
    print_line("Available commands:");

    size_t longest_string = 0;
    for (const auto& command : commands) {
        if (longest_string < command.name.length()) {
            longest_string = command.name.length();
        }
    }

    for (const auto& command : commands) {
        if (command.type == Command::Hidden) {
            continue;
        }
        print_line(command.name + String(longest_string - command.name.length(), ' ') + " - " + command.description + ".");
    }
}

void Application::_read_input(bool p_lower_case) {
#ifdef CONSOLE_FEATURES_ENABLED
    set_text_color(command_text_color);
#endif // CONSOLE_FEATURES_ENABLED

    static String line;
    bool is_string = false;
    int read_chars = 0;
    bool skip_space = true;

    input.clear();

    if (line.empty()) {
        std::getline(std::cin, line);
    }

    for (char c : line) {
        ++read_chars;
        if (c == '"') {
            is_string = !is_string;
            skip_space = false;
        } else if (!is_string && skip_space && c == ' ') {
            continue;
        } else if (!is_string && c == ' ') {
            break;
        } else {
            input += c;
            skip_space = false;
        }
    }

    line = line.substr(read_chars);

    if (p_lower_case) {
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    }

#ifdef CONSOLE_FEATURES_ENABLED
    set_text_color(Color::White);
#endif // CONSOLE_FEATURES_ENABLED
}

void Application::_to_config_file(ConfigFile& p_file) const {
#ifdef LOG_ENABLED
    p_file.set_value("log_file_name", log_file_name);
#endif // LOG_ENABLED
    packer.to_config_file(p_file);
}

void Application::_from_config_file(const ConfigFile& p_file) {
#ifdef LOG_ENABLED
    log_file_name = p_file.get_value("log_file_name", DEFAULT_LOG_FILE_NAME);
#endif // LOG_ENABLED
    packer.from_config_file(p_file);
}

Error Application::_save(const String& p_path) {
    ConfigFile config_file;
    _to_config_file(config_file);
#ifndef CONFIG_FILE_ENCRYPTION_ENABLED
    return config_file.save(p_path);
#else // CONFIG_FILE_ENCRYPTION_ENABLED
    return config_file.save_encrypted(p_path, CryptoKey(*this));
#endif // CONFIG_FILE_ENCRYPTION_ENABLED
}

Error Application::_load(const String& p_path) {
    ConfigFile config_file;
#ifndef CONFIG_FILE_ENCRYPTION_ENABLED
    Error error = config_file.load(p_path);
#else // CONFIG_FILE_ENCRYPTION_ENABLED
    Error error = config_file.load_encrypted(p_path, CryptoKey(*this));
#endif // CONFIG_FILE_ENCRYPTION_ENABLED
    if (error != Error::OK) {
        return error;
    }
    _from_config_file(config_file);
    return Error::OK;
}

int Application::run() {
    print_line("Welcome to packer version "  VERSION_STRING  ".");
    print_line("Type a command or 'help' for a list of commands:");

    _load(DEFAULT_CONFIG_FILE_NAME);

    while (process_commands) {
        _read_input(true);
        bool processed = false;
        for (const auto& command : commands) {
            if (command.name == input) {
                if (command.type == Command::Prompt) {
                    print_line(command.prompt);
                    _read_input(false);
                }
                (this->*command.function)();
                processed = true;
                break;
            }
        }
        if (!processed) {
            print_line("Unknown command");
        }
    }
    return EXIT_SUCCESS;
}

Application::Application() :
#ifdef CONSOLE_FEATURES_ENABLED
    command_text_color(DEFAULT_COMMAND_TEXT_COLOR),
#endif // CONSOLE_FEATURES_ENABLED
#ifdef LOG_ENABLED
    log_file_name(DEFAULT_LOG_FILE_NAME),
#endif // LOG_ENABLED
    process_commands(true) {
    _add_prompt_command(&Application::_set_read_path, "read_path", "Change the path that you would like to read from", "Type the path (or 'current' to use the current directory):");
    _add_prompt_command(&Application::_set_write_path, "write_path", "Change the path that you would like to write to", "Type the path:");
    _add_prompt_command(&Application::_add_extension, "add_extension", "Add an extension to the extension list", "Type the extension to add:");
    _add_prompt_command(&Application::_remove_extension, "remove_extension", "Remove an extension from the extension list", "Type the extension to remove:");
    _add_simple_command(&Application::_clear_extensions, "clear_extensions", "Clear all of the extensions in the extension list");
    _add_prompt_command(&Application::_set_pack_mode, "pack_mode", "Pack matching extensions, exclude matching extensions or pack everything", "Type '" + Packer::get_pack_mode_name(Packer::PackMode::Include) + "', '" + Packer::get_pack_mode_name(Packer::PackMode::Exclude) + "', '" + Packer::get_pack_mode_name(Packer::PackMode::Everything) + ":");
    _add_simple_command(&Application::_set_overwrite_files, "overwrite_files", "Overwrite existing files");
    _add_simple_command(&Application::_set_move_files, "move_files", "Move the files");
    _add_prompt_command(&Application::_set_suffix_string, "suffix_string", "The suffix string to remove", "Type the suffix string to remove:");
    _add_simple_command(&Application::_set_suffix_enabled, "suffix_enabled", "Enable suffix string removal");
    _add_simple_command(&Application::_set_extension_insensitive, "extension_insensitive", "Ignore extension case in the extension list");
    _add_prompt_command(&Application::_set_extension_adjust, "extension_adjust", "Adjust the extension case", "Type '" + Packer::get_extension_adjust_name(Packer::ExtensionAdjust::Default) + "', '" + Packer::get_extension_adjust_name(Packer::ExtensionAdjust::Lower) + "', '" + Packer::get_extension_adjust_name(Packer::ExtensionAdjust::Upper) + ":");
#ifdef IGNORE_FILE_ENABLED
    _add_prompt_command(&Application::_set_ignore_file_name, "ignore_file_name", "Change the name of the ignore file", "Type the name of the ignore file (or 'default' to use to the default):");
    _add_simple_command(&Application::_set_ignore_file_enabled, "ignore_file_enabled", "Check for an ignore file");
#endif // IGNORE_FILE_ENABLED
#ifdef LOG_ENABLED
    _add_prompt_command(&Application::_set_log_file_name, "log_file_name", "Change the name of the log file", "Type the name of the log file (or 'default' to use the default):");
    _add_simple_command(&Application::_set_log_enabled, "log_enabled", "Enable logging");
#endif // LOG_ENABLED
    _add_simple_command(&Application::_swap_paths, "swap_paths", "Swap the read and write directories");
    _add_simple_command(&Application::_revert_state, "revert", "Revert all of the settings to their defaults");
    _add_prompt_command(&Application::_save_config, "save", "Save the state to a config file", "Type the name of the config file (or 'default' to use the default):");
    _add_prompt_command(&Application::_load_config, "load", "Load a state from a config file", "Type the name of the config file (or 'default' to use the default):");
    _add_simple_command(&Application::_print_info, "info", "Print the current state of the packer");
    _add_simple_command(&Application::_run_packer, "run", "Run the packer");
    _add_simple_command(&Application::_quit_program, "quit", "Quit the application");
    _add_hidden_command(&Application::_print_help, "help");
}

PACKER_NAMESPACE_END
