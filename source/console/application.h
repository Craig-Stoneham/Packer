/**************************************************************************/
/* Copyright (c) 2021-present Craig Stoneham.                             */
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

#include "typedefs.h"

#include <console.h>
#include <log_file.h>
#include <packer.h>
#include <version.h>

PACKER_NAMESPACE_BEGIN

/**
 * @def DEFAULT_CONFIG_FILE_NAME
 * @brief The default configuration file name for Packer Console.
 *
 * This macro defines the default name of the configuration file used by Packer Console.
 * You can change this value to specify a different configuration file name if needed.
 */
#define DEFAULT_CONFIG_FILE_NAME "packer.cfg"

/**
 * @def DEFAULT_LOG_FILE_NAME
 * @brief The default log file name for Packer Console.
 *
 * This macro defines the default name of the log file used by Packer Console.
 * You can change this value to specify a different log file name if needed.
 */
#define DEFAULT_LOG_FILE_NAME "packer.log"

/**
 * @def DEFAULT_COMMAND_TEXT_COLOR
 * @brief The default text color for command text in Packer Console.
 *
 * This macro defines the default text color used for displaying command text in the console.
 * You can change this value to specify a different default text color for command text.
 */
#define DEFAULT_COMMAND_TEXT_COLOR Color::LightGreen

/**
 * @class Application
 * @brief Manages the Packer application, providing a command-line interface.
 *
 * The Application class acts as the user interface for the Packer application. It allows users
 * to interact with Packer through various commands, such as setting paths, configuring options,
 * and executing packing operations. Users can provide input and receive output through the console.
 */
class Application : public Console {
    /**
     * @typedef Function
     * @brief Represents a member function pointer type within the Application class.
     *
     * This type is used to define pointers to member functions that correspond to various
     * commands or actions that can be executed within the Application class.
     */
    using Function = void (Application::*)();

    /**
     * @struct Command
     * @brief Represents a command definition used within the Application class.
     *
     * A Command contains information about a specific action or operation that can be executed
     * within the Application. Each command has a name, a description, and may require additional
     * user input when of type 'Prompt'.
     */
    struct Command {
        /**
         * @enum Type
         * @brief Enumeration representing the type of a command.
         *
         * The type determines how the command is presented to the user and whether it requires
         * additional user input.
         */
        enum Type {
            Hidden, ///< Hidden commands are not displayed to the user.
            Simple, ///< Simple commands can be executed with a single action.
            Prompt  ///< Prompt commands require additional user input.
        };

        Type type;            ///< The type of the command.
        Function function;    ///< Pointer to the member function implementing the command.
        String name;          ///< The name or identifier of the command.
        String description;   ///< A brief description of what the command does.
        String prompt;        ///< Prompt for additional user input (used when type is 'Prompt').
    };


    Packer packer; ///< The Packer instance used for file packing.
    String input; ///< The user's input.
    Vector<Command> commands; ///< List of available console commands.
    bool process_commands; ///< Flag indicating whether to process user commands.

#ifdef CONSOLE_FEATURES_ENABLED
    Color command_text_color; ///< The text color for command text in the console.
#endif // CONSOLE_FEATURES_ENABLED

#ifdef LOG_ENABLED
    String log_file_name; ///< The name of the log file.
#endif // LOG_ENABLED

    /**
     * @brief Adds a hidden command.
     * @param p_function The command's function.
     * @param p_name The name of the command.
     */
    void _add_hidden_command(Function p_function, const String& p_name);

    /**
     * @brief Adds a simple command.
     * @param p_function The command's function.
     * @param p_name The name of the command.
     * @param p_description The description of the command.
     */
    void _add_simple_command(Function p_function, const String& p_name, const String& p_description);

    /**
     * @brief Adds a prompt command.
     * @param p_function The command's function.
     * @param p_name The name of the command.
     * @param p_description The description of the command.
     * @param p_prompt The user prompt for the command.
     */
    void _add_prompt_command(Function p_function, const String& p_name, const String& p_description, const String& p_prompt);

    /**
     * @brief Sets the read path for file packing.
     */
    void _set_read_path();

    /**
     * @brief Sets the write path for file packing.
     */
    void _set_write_path();

    /**
     * @brief Adds an extension to the list of extensions for packing.
     */
    void _add_extension();

    /**
     * @brief Removes an extension from the list of extensions for packing.
     */
    void _remove_extension();

    /**
     * @brief Clears the list of extensions for packing.
     */
    void _clear_extensions();

    /**
     * @brief Sets the packing mode (Include, Exclude, Everything).
     */
    void _set_pack_mode();

    /**
     * @brief Sets whether to overwrite existing files during packing.
     */
    void _set_overwrite_files();

    /**
     * @brief Sets whether to move files instead of copying during packing.
     */
    void _set_move_files();

    /**
     * @brief Sets the suffix string to be added to packed files.
     */
    void _set_suffix_string();

    /**
     * @brief Sets whether the suffix is enabled for packed files.
     */
    void _set_suffix_enabled();

    /**
     * @brief Sets whether file extension comparisons are case-insensitive.
     */
    void _set_extension_insensitive();

    /**
     * @brief Sets how file extensions are adjusted (Default, Lower, Upper).
     */
    void _set_extension_adjust();

#ifdef IGNORE_FILE_ENABLED
    /**
     * @brief Sets the name of the ignore file.
     */
    void _set_ignore_file_name();

    /**
     * @brief Sets whether the ignore file is enabled.
     */
    void _set_ignore_file_enabled();
#endif // IGNORE_FILE_ENABLED

#ifdef LOG_ENABLED
    /**
     * @brief Sets the name of the log file.
     */
    void _set_log_file_name();

    /**
     * @brief Sets whether logging is enabled.
     */
    void _set_log_enabled();
#endif // LOG_ENABLED

    /**
     * @brief Swaps the read and write paths.
     */
    void _swap_paths();

    /**
     * @brief Reverts the application state to defaults.
     */
    void _revert_state();

    /**
     * @brief Saves configuration settings to a file.
     */
    void _save_config();

    /**
     * @brief Loads configuration settings from a file.
     */
    void _load_config();

    /**
     * @brief Prints information about the current configuration.
     */
    void _print_info();

    /**
     * @brief Runs the Packer to process files.
     */
    void _run_packer();

    /**
     * @brief Quits the application.
     */
    void _quit_program();

    /**
     * @brief Prints the help menu with available commands.
     */
    void _print_help();

    /**
     * @brief Reads user input from the console.
     * @param p_lower_case Whether to convert input to lowercase.
     */
    void _read_input(bool p_lower_case = false);

    /**
     * @brief Writes configuration settings to a file.
     * @param p_file The ConfigFile to write to.
     */
    void _to_config_file(ConfigFile& p_file) const;

    /**
     * @brief Reads configuration settings from a file.
     * @param p_file The ConfigFile to read from.
     */
    void _from_config_file(const ConfigFile& p_file);

    /**
     * @brief Saves configuration settings to a file.
     * @param p_path The path to the configuration file.
     * @return An Error code indicating the success of the operation.
     */
    Error _save(const String& p_path);

    /**
     * @brief Loads configuration settings from a file.
     * @param p_path The path to the configuration file.
     * @return An Error code indicating the success of the operation.
     */
    Error _load(const String& p_path);


public:
    /**
     * @brief Runs the application.
     * @return The application's exit code.
     */
    int run();

    /**
     * @brief Default constructor for the Application class.
     */
    Application();
};

PACKER_NAMESPACE_END
