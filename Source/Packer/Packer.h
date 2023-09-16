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

/**
 * @file Packer.h
 * @brief Defines a utility for packing and managing files based on user-defined rules.
 */

#pragma once

#include "ConfigFile.h"
#include "Log.h"

PACKER_NAMESPACE_BEGIN

/**
 * @class Packer
 * @brief A utility class for packing and managing files based on user-defined rules.
 * 
 * The `Packer` class allows you to specify rules for packing and managing files in a directory.
 * It supports various packing modes, extension handling, file suffixes, and more.
 */
class Packer {
public:
    /**
     * @enum PackMode
     * @brief Enumeration defining different packing modes.
     *
     * This enumeration represents various packing modes that control how files are packed.
     */
    enum class PackMode {
        Unknown = -1, ///< An unknown packing mode.
        Include,      ///< Pack only extensions found in the extensions list.
        Exclude,      ///< Pack everything excluding extensions found in the extensions list.
        Everything,   ///< Pack everything regardless of extension.
        Max           ///< The maximum value for the PackMode enumeration.
    };

    /**
     * @enum ExtensionAdjust
     * @brief Enumeration defining different extension adjustment options.
     *
     * This enumeration represents options for adjusting the case of file extensions.
     */
    enum class ExtensionAdjust {
        Unknown = -1, ///< An unknown extension adjustment option.
        Default,      ///< Leave extension case as-is.
        Lower,        ///< Convert extension case to lower case.
        Upper,        ///< Convert extension case to upper case.
        Max           ///< The maximum value for the ExtensionAdjust enumeration.
    };

private:
    String read_path; ///< The source directory to pack files from.
    String write_path; ///< The destination directory to write packed files to.

    Vector<String> extensions; ///< A list of extensions to consider when packing files.

    PackMode pack_mode; ///< The packing mode to use.
    bool overwrite_files; ///< Flag indicating whether to overwrite existing files.
    bool move_files; ///< Flag indicating whether to move files instead of copying them.

    String suffix_string; ///< The suffix to add to packed file names.
    bool suffix_enabled; ///< Flag indicating whether suffixes are enabled.

    bool extension_insensitive; ///< Flag indicating case-insensitivity for extensions.
    ExtensionAdjust extension_adjust; ///< The adjustment to apply to file extensions.

#ifndef IGNORE_FILE_DISABLED
    String ignore_file_name; ///< The name of the ignore file to use.
    bool ignore_file_enabled; ///< Flag indicating whether ignore files are enabled.
#endif // IGNORE_FILE_DISABLED

#ifndef LOG_DISABLED
    bool log_enabled; ///< Flag indicating whether logging is enabled.
#endif // LOG_DISABLED

    template <bool, bool, bool, bool, bool, bool, PackMode, ExtensionAdjust>
    void _pack_files(const String& p_read_path, const String& p_write_path);

public:
    /**
     * @brief Get a string representation of a PackMode enum value.
     * @param p_mode The PackMode enum value.
     * @return A string representation of the PackMode.
     */
    static String get_pack_mode_name(PackMode p_mode);

    /**
     * @brief Find a PackMode enum value based on its string representation.
     * @param p_mode The string representation of the PackMode.
     * @return The corresponding PackMode enum value.
     */
    static PackMode find_pack_mode(const String& p_mode);

    /**
     * @brief Get a string representation of an ExtensionAdjust enum value.
     * @param p_adjust The ExtensionAdjust enum value.
     * @return A string representation of the ExtensionAdjust.
     */
    static String get_extension_adjust_name(ExtensionAdjust p_adjust);

    /**
     * @brief Find an ExtensionAdjust enum value based on its string representation.
     * @param p_adjust The string representation of the ExtensionAdjust.
     * @return The corresponding ExtensionAdjust enum value.
     */
    static ExtensionAdjust find_extension_adjust(const String& p_adjust);

    /**
     * @brief Set the source directory to pack files from.
     * @param p_path The path to the source directory.
     */
    void set_read_path(const String& p_path);

    /**
     * @brief Get the source directory where files are packed from.
     * @return The source directory path.
     */
    const String& get_read_path() const;

    /**
     * @brief Set the destination directory to write packed files to.
     * @param p_path The path to the destination directory.
     */
    void set_write_path(const String& p_path);

    /**
     * @brief Get the destination directory where packed files are written to.
     * @return The destination directory path.
     */
    const String& get_write_path() const;

    /**
     * @brief Get the number of extensions in the list.
     * @return The number of extensions.
     */
    size_t get_extension_count() const;

    /**
     * @brief Get an extension at a specified index in the list.
     * @param p_index The index of the extension to retrieve.
     * @return The extension at the specified index.
     */
    const String& get_extension(size_t p_index) const;

    /**
     * @brief Add an extension to the list of extensions.
     * @param p_extension The extension to add.
     * @return `true` if the extension was added, `false` if it already exists.
     */
    bool add_extension(const String& p_extension);

    /**
     * @brief Remove an extension from the list of extensions.
     * @param p_extension The extension to remove.
     * @return `true` if the extension was removed, `false` if it does not exist.
     */
    bool remove_extension(const String& p_extension);

    /**
     * @brief Check if a specific extension exists in the list.
     * @param p_extension The extension to check.
     * @return `true` if the extension exists, `false` otherwise.
     */
    bool has_extension(const String& p_extension) const;

    /**
     * @brief Clear the list of extensions.
     */
    void clear_extensions();

    /**
     * @brief Set the packing mode.
     * @param p_mode The packing mode to set.
     */
    void set_pack_mode(PackMode p_mode);

    /**
     * @brief Get the current packing mode.
     * @return The current packing mode.
     */
    PackMode get_pack_mode() const;

    /**
     * @brief Set whether to overwrite existing files.
     * @param p_enable `true` to enable overwriting, `false` to disable it.
     */
    void set_overwrite_files(bool p_enable);

    /**
     * @brief Check if overwriting existing files is enabled.
     * @return `true` if overwriting is enabled, `false` otherwise.
     */
    bool get_overwrite_files() const;

    /**
     * @brief Set whether to move files instead of copying them.
     * @param p_enable `true` to enable moving, `false` to disable it.
     */
    void set_move_files(bool p_enable);

    /**
     * @brief Check if moving files is enabled.
     * @return `true` if moving files is enabled, `false` otherwise.
     */
    bool get_move_files() const;

    /**
     * @brief Set the suffix string to append to packed file names.
     * @param p_suffix The suffix string to set.
     */
    void set_suffix_string(const String& p_suffix);

    /**
     * @brief Get the current suffix string.
     * @return The current suffix string.
     */
    const String& get_suffix_string() const;

    /**
     * @brief Enable or disable suffixes.
     * @param p_enable `true` to enable suffixes, `false` to disable them.
     */
    void set_suffix_enabled(bool p_enable);

    /**
     * @brief Check if suffixes are enabled.
     * @return `true` if suffixes are enabled, `false` otherwise.
     */
    bool get_suffix_enabled() const;

    /**
     * @brief Enable or disable case-insensitive handling of extensions.
     * @param p_enable `true` to enable case-insensitivity, `false` to disable it.
     */
    void set_extension_insensitive(bool p_enable);

    /**
     * @brief Check if case-insensitive handling of extensions is enabled.
     * @return `true` if case-insensitivity is enabled, `false` otherwise.
     */
    bool get_extension_insensitive() const;

    /**
     * @brief Set the adjustment to apply to file extensions.
     * @param p_case The extension adjustment to set.
     */
    void set_extension_adjust(ExtensionAdjust p_case);

    /**
     * @brief Get the current extension adjustment.
     * @return The current extension adjustment.
     */
    ExtensionAdjust get_extension_adjust() const;

#ifndef IGNORE_FILE_DISABLED
    /**
     * @brief Set the name of the ignore file to use.
     * @param p_name The name of the ignore file.
     */
    void set_ignore_file_name(const String& p_name);

    /**
     * @brief Get the name of the ignore file.
     * @return The name of the ignore file.
     */
    const String& get_ignore_file_name() const;

    /**
     * @brief Enable or disable the use of ignore files.
     * @param p_enable `true` to enable ignore files, `false` to disable them.
     */
    void set_ignore_file_enabled(bool p_enable);

    /**
     * @brief Check if ignore files are enabled.
     * @return `true` if ignore files are enabled, `false` otherwise.
     */
    bool get_ignore_file_enabled() const;
#endif //PACKER_IGNORE_FILE

#ifndef LOG_DISABLED
    /**
     * @brief Enable or disable logging.
     * @param p_enable `true` to enable logging, `false` to disable it.
     */
    void set_log_enabled(bool p_enable);

    /**
     * @brief Check if logging is enabled.
     * @return `true` if logging is enabled, `false` otherwise.
     */
    bool get_log_enabled() const;
#endif // LOG_DISABLED

    /**
     * @brief Serialize the Packer configuration to a ConfigFile.
     * @param p_file The ConfigFile to store the configuration in.
     */
    void to_config_file(ConfigFile& p_file) const;

    /**
     * @brief Deserialize the Packer configuration from a ConfigFile.
     * @param p_file The ConfigFile containing the configuration data.
     */
    void from_config_file(const ConfigFile& p_file);

    /**
     * @brief Save the Packer configuration to a file.
     * @param p_path The path to the file where the configuration will be saved.
     * @return An `Error` code indicating the success or failure of the operation.
     */
    Error save(const String& p_path) const;

    /**
     * @brief Load the Packer configuration from a file.
     * @param p_path The path to the file from which configuration data will be loaded.
     * @return An `Error` code indicating the success or failure of the operation.
     */
    Error load(const String& p_path);

    /**
     * @brief Save the encrypted Packer configuration to a file.
     * @param p_path The path to the file where the encrypted configuration will be saved.
     * @param p_key The encryption key used to protect the configuration.
     * @return An `Error` code indicating the success or failure of the operation.
     */
    Error save_encrypted(const String& p_path, const CryptoKey& p_key) const;

    /**
     * @brief Load encrypted Packer configuration data from a file.
     * @param p_path The path to the file from which encrypted configuration data will be loaded.
     * @param p_key The encryption key used to decrypt the configuration.
     * @return An `Error` code indicating the success or failure of the operation.
     */
    Error load_encrypted(const String& p_path, const CryptoKey& p_key);

    /**
     * @brief Revert the Packer's state to its default values.
     */
    void revert_state();

    /**
     * @brief Pack files based on the Packer's configuration.
     * @return An `Error` code indicating the success or failure of the packing operation.
     */
    Error pack_files();

    /**
     * @brief Constructor for the Packer class.
     */
    Packer();
};

PACKER_NAMESPACE_END