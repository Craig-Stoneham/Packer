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

#include "Extension.h"
#include "ConfigFile.h"
#include "Log.h"

class Packer {
public:
    enum class PackMode {
        Unknown = -1,
        Include,
        Exclude,
        Everything,
        Max
    };

    static String get_pack_mode_string(PackMode p_mode);
    static PackMode find_pack_mode(const String& p_mode);

private:
    String read_path;
    String write_path;

    Vector<String> extensions;

    PackMode pack_mode;
    bool overwrite_files;
    bool move_files;

    String suffix_string;
    bool suffix_enabled;

    bool extension_insensitive;
    ExtensionCase extension_adjust;

#ifndef IGNORE_FILE_DISABLED
    String ignore_file_name;
    bool ignore_file_enabled;
#endif // IGNORE_FILE_DISABLED

#ifndef IGNORE_FILE_DISABLED
    bool log_enabled;
#endif // IGNORE_FILE_DISABLED

    template <PackMode, bool, bool, bool, bool, ExtensionCase, bool, bool>
    void _pack_files(const String& p_read_path, const String& p_write_path);

public:
    void set_read_path(const String& p_path);
    const String& get_read_path() const;
    void set_write_path(const String& p_path);
    const String& get_write_path() const;

    size_t get_extension_count() const;
    const String get_extension(size_t p_index) const;
    bool add_extension(const String& p_extension);
    bool remove_extension(const String& p_extension);
    bool has_extension(const String& p_extension) const;
    void clear_extensions();

    void set_pack_mode(PackMode p_mode);
    PackMode get_pack_mode() const;
    void set_overwrite_files(bool p_enable);
    bool get_overwrite_files() const;
    void set_move_files(bool p_enable);
    bool get_move_files() const;

    void set_suffix_string(const String& p_suffix);
    const String& get_suffix_string() const;
    void set_suffix_enabled(bool p_enable);
    bool get_suffix_enabled() const;

    void set_extension_insensitive(bool p_enable);
    bool get_extension_insensitive() const;
    void set_extension_adjust(ExtensionCase p_case);
    ExtensionCase get_extension_adjust() const;

#ifndef IGNORE_FILE_DISABLED
    void set_ignore_file_name(const String& p_name);
    const String& get_ignore_file_name() const;
    void set_ignore_file_enabled(bool p_enable);
    bool get_ignore_file_enabled() const;
#endif //PACKER_IGNORE_FILE

#ifndef LOG_DISABLED
    void set_log_enabled(bool p_enable);
    bool get_log_enabled() const;
#endif // LOG_DISABLED

    void to_config_file(ConfigFile& p_file) const;
    void from_config_file(const ConfigFile& p_file);

    Error save(const String& p_path) const;
    Error load(const String& p_path);

    Error save_encrypted(const String& p_path, const CryptoKey& p_key) const;
    Error load_encrypted(const String& p_path, const CryptoKey& p_key);

    void revert_state();

    Error pack_files();

    Packer();
};
