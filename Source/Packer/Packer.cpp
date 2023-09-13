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

#include "Packer.h"
#include "Defaults.h"

static const char* _pack_mode[] = {
    "include",
    "exclude",
    "everything"
};

String Packer::get_pack_mode_string(PackMode p_mode) {
    if (p_mode >= static_cast<PackMode>(0) && p_mode < PackMode::Max) {
        return _pack_mode[static_cast<size_t>(p_mode)];
    } else {
        return "unknown";
    }
}

Packer::PackMode Packer::find_pack_mode(const String& p_mode) {
    for (size_t i = 0; i < static_cast<size_t>(PackMode::Max); ++i) {
        if (p_mode == _pack_mode[i]) {
            return static_cast<PackMode>(i);
        }
    }
    return PackMode::Unknown;
}

static const char* _extension_adjust[] = {
    "default",
    "lower",
    "upper"
};

String Packer::get_extension_adjust_string(ExtensionAdjust p_adjust) {
    if (p_adjust >= static_cast<ExtensionAdjust>(0) && p_adjust < ExtensionAdjust::Max) {
        return _extension_adjust[static_cast<size_t>(p_adjust)];
    } else {
        return "unknown";
    }
}

Packer::ExtensionAdjust Packer::find_extension_adjust(const String& p_adjust) {
    for (size_t i = 0; i < static_cast<size_t>(ExtensionAdjust::Max); ++i) {
        if (p_adjust == _extension_adjust[i]) {
            return static_cast<ExtensionAdjust>(i);
        }
    }
    return ExtensionAdjust::Unknown;
}

template <bool OVERWRITE, bool MOVE, bool SUFFIX, bool EXT_INSENSITIVE, bool IGNORE_FILE, bool LOG, Packer::PackMode MODE, Packer::ExtensionAdjust EXT_ADJUST>
void Packer::_pack_files(const String& p_read_path, const String& p_write_path) {
#ifndef IGNORE_FILE_DISABLED
    if (IGNORE_FILE) {
        if (FileAccess::is_directory(p_read_path) == true) {
            if (FileAccess::exists(p_read_path + "/" + ignore_file_name)) {
                return;
            }
        }
    }
#endif //IGNORE_FILE_DISABLED

    bool write_directory_exists = false;

    for (auto& path : FileAccess::directory_iterator(p_read_path)) {
        String _read_path = path.path().string();
#if EXPERIMENTAL_FILESYSTEM
        normalize_path_separators(_read_path);
#endif // EXPERIMENTAL_FILESYSTEM

        if (FileAccess::is_directory(path)) {
            String _write_path = p_write_path + _read_path.substr(_read_path.find_last_of('/'));
            _pack_files<OVERWRITE, MOVE, SUFFIX, EXT_INSENSITIVE, IGNORE_FILE, LOG, MODE, EXT_ADJUST>(_read_path, _write_path);
        } else {
            if (MODE != PackMode::Everything) {
                String extension = _read_path.substr(_read_path.find_last_of('.') + 1);

                if (EXT_INSENSITIVE) {
                    std::transform(extension.begin(), extension.end(), extension.begin(), tolower);
                }

                bool skip_file = MODE == PackMode::Include;

                for (const String& e : extensions) {
                    if (EXT_INSENSITIVE) {
                        String transformed = e;
                        std::transform(transformed.begin(), transformed.end(), transformed.begin(), tolower);

                        if (extension == transformed) {
                            if (MODE == PackMode::Include) {
                                skip_file = false;
                                break;
                            } else if (MODE == PackMode::Exclude) {
                                skip_file = true;
                                break;
                            }
                        }
                    } else {
                        if (extension == e) {
                            if (MODE == PackMode::Include) {
                                skip_file = false;
                                break;
                            } else if (MODE == PackMode::Exclude) {
                                skip_file = true;
                                break;
                            }
                        }
                    }
                }
                if (skip_file) {
                    continue;
                }
            }

            String _write_path = p_write_path + _read_path.substr(_read_path.find_last_of('/'));

            if (SUFFIX) {
                remove_path_suffix(_write_path, suffix_string);
            }

            if (EXT_ADJUST == ExtensionAdjust::Lower) {
                size_t ext_pos = _write_path.find_last_of('.') + 1;
                std::transform(_write_path.begin() + ext_pos, _write_path.end(), _write_path.begin() + ext_pos, tolower);
            } else if (EXT_ADJUST == ExtensionAdjust::Upper) {
                size_t ext_pos = _write_path.find_last_of('.') + 1;
                std::transform(_write_path.begin() + ext_pos, _write_path.end(), _write_path.begin() + ext_pos, toupper);
            }

            if (!OVERWRITE) {
                if (FileAccess::exists(_write_path)) {
                    continue;
                }
            }

            if (!write_directory_exists) {
                write_directory_exists = true;
                FileAccess::create_directories(p_write_path);
            }

            if (!copy_file(_read_path, _write_path, FileAccess::copy_options::update_existing)) {
                continue;
            }

            if (MOVE) {
                FileAccess::remove(_read_path);
            }

            if (LOG) {
                LOG_INFO((MOVE ? "Moved " : "Copied ") + _read_path + " to " + _write_path + "\n");
            }
        }
    }
}

void Packer::set_read_path(const String& p_path) {
    read_path = p_path;
}

const String& Packer::get_read_path() const {
    return read_path;
}

void Packer::set_write_path(const String& p_path) {
    write_path = p_path;
}

const String& Packer::get_write_path() const {
    return write_path;
}

size_t Packer::get_extension_count() const {
    return extensions.size();
}

const String& Packer::get_extension(size_t p_index) const {
    return extensions[p_index];
}

bool Packer::add_extension(const String& p_extension) {
    for (const String& e : extensions) {
        if (p_extension == e) {
            return false;
        }
    }
    extensions.push_back(p_extension);
    return true;
}

bool Packer::remove_extension(const String& p_extension) {
    for (size_t i = 0; i < extensions.size(); ++i) {
        if (extensions[i] == p_extension) {
            extensions.erase(extensions.begin() + i);
            return true;
        }
    }
    return false;
}

bool Packer::has_extension(const String& p_extension) const {
    for (const String& e : extensions) {
        if (p_extension == e) {
            return true;
        }
    }
    return false;
}

void Packer::clear_extensions() {
    extensions.clear();
}

void Packer::set_pack_mode(PackMode p_mode) {
    if (p_mode < static_cast<PackMode>(0) || p_mode >= PackMode::Max) {
        return;
    }
    pack_mode = p_mode;
}

Packer::PackMode Packer::get_pack_mode() const {
    return pack_mode;
}

void Packer::set_overwrite_files(bool p_enable) {
    overwrite_files = p_enable;
}

bool Packer::get_overwrite_files() const {
    return overwrite_files;
}

void Packer::set_move_files(bool p_enable) {
    move_files = p_enable;
}

bool Packer::get_move_files() const {
    return move_files;
}

void Packer::set_suffix_string(const String& p_suffix) {
    suffix_string = p_suffix;
}

const String& Packer::get_suffix_string() const {
    return suffix_string;
}

void Packer::set_suffix_enabled(bool p_enable) {
    suffix_enabled = p_enable;
}

bool Packer::get_suffix_enabled() const {
    return suffix_enabled;
}

void Packer::set_extension_insensitive(bool p_enable) {
    extension_insensitive = p_enable;
}

bool Packer::get_extension_insensitive() const {
    return extension_insensitive;
}

void Packer::set_extension_adjust(ExtensionAdjust p_case) {
    if (p_case < static_cast<ExtensionAdjust>(0) || p_case >= ExtensionAdjust::Max) {
        return;
    }
    extension_adjust = p_case;
}

Packer::ExtensionAdjust Packer::get_extension_adjust() const {
    return extension_adjust;
}

#ifndef IGNORE_FILE_DISABLED

void Packer::set_ignore_file_name(const String& p_name) {
    ignore_file_name = p_name;
}

const String& Packer::get_ignore_file_name() const {
    return ignore_file_name;
}

void Packer::set_ignore_file_enabled(bool p_enable) {
    ignore_file_enabled = p_enable;
}

bool Packer::get_ignore_file_enabled() const {
    return ignore_file_enabled;
}

#endif //PACKER_IGNORE_FILE

#ifndef LOG_DISABLED

void Packer::set_log_enabled(bool p_enable) {
    log_enabled = p_enable;
}

bool Packer::get_log_enabled() const {
    return log_enabled;
}

#endif // LOG_DISABLED

void Packer::to_config_file(ConfigFile& p_file) const {
    p_file.set_value("read_path", read_path);
    p_file.set_value("write_path", write_path);
    p_file.set_value("extensions", extensions);
    p_file.set_value("pack_mode", static_cast<int>(pack_mode));
    p_file.set_value("overwrite_files", overwrite_files);
    p_file.set_value("move_files", move_files);
    p_file.set_value("suffix_string", suffix_string);
    p_file.set_value("suffix_enabled", suffix_enabled);
    p_file.set_value("extension_insensitive", extension_insensitive);
    p_file.set_value("extension_adjust", static_cast<int>(extension_adjust));

#ifndef IGNORE_FILE_DISABLED
    p_file.set_value("ignore_file_name", ignore_file_name);
    p_file.set_value("ignore_file_enabled", ignore_file_enabled);
#endif // IGNORE_FILE_DISABLED

#ifndef LOG_DISABLED
    p_file.set_value("log_enabled", log_enabled);
#endif // LOG_DISABLED
}

void Packer::from_config_file(const ConfigFile& p_file) {
    read_path = p_file.get_value("read_path", DEFAULT_READ_PATH);
    write_path = p_file.get_value("write_path", DEFAULT_WRITE_PATH);
    extensions = p_file.get_value("extensions", DEFAULT_EXTENTIONS);
    pack_mode = static_cast<PackMode>(p_file.get_value("pack_mode", static_cast<int>(DEFAULT_PACK_MODE)).operator const int());
    overwrite_files = p_file.get_value("overwrite_files", DEFAULT_OVERWRITE_FILES);
    move_files = p_file.get_value("move_files", DEFAULT_MOVE_FILES);
    suffix_string = p_file.get_value("suffix_string", DEFAULT_SUFFIX_STRING);
    suffix_enabled = p_file.get_value("suffix_enabled", DEFAULT_SUFFIX_ENABLED);
    extension_insensitive = p_file.get_value("extension_insensitive", DEFAULT_EXTENSION_INSENSITIVE);
    extension_adjust = static_cast<ExtensionAdjust>(p_file.get_value("extension_adjust", static_cast<int>(DEFAULT_EXTENSION_ADJUST)).operator const int());

#ifndef IGNORE_FILE_DISABLED
    ignore_file_name = p_file.get_value("ignore_file_name", DEFAULT_IGNORE_FILE_NAME);
    ignore_file_enabled = p_file.get_value("ignore_file_enabled", DEFAULT_IGNORE_FILE_ENABLED);
#endif // IGNORE_FILE_DISABLED

#ifndef LOG_DISABLED
    log_enabled = p_file.get_value("log_enabled", DEFAULT_LOG_ENABLED);
#endif // LOG_DISABLED
}

Error Packer::save(const String& p_path) const {
    ConfigFile config_file;
    to_config_file(config_file);
    return config_file.save(p_path);
}

Error Packer::load(const String& p_path) {
    ConfigFile config_file;
    Error error = config_file.load(p_path);
    if (error != Error::OK) {
        return error;
    }
    from_config_file(config_file);
    return Error::OK;
}

Error Packer::save_encrypted(const String& p_path, const CryptoKey& p_key) const {
    ConfigFile config_file;
    to_config_file(config_file);
    return config_file.save_encrypted(p_path, p_key);
}

Error Packer::load_encrypted(const String& p_path, const CryptoKey& p_key) {
    ConfigFile config_file;
    Error error = config_file.load_encrypted(p_path, p_key);
    if (error != Error::OK) {
        return error;
    }
    from_config_file(config_file);
    return Error::OK;
}

void Packer::revert_state() {
    read_path = DEFAULT_READ_PATH;
    write_path = DEFAULT_WRITE_PATH;
    extensions = DEFAULT_EXTENTIONS;
    pack_mode = DEFAULT_PACK_MODE;
    overwrite_files = DEFAULT_OVERWRITE_FILES;
    move_files = DEFAULT_MOVE_FILES;
    suffix_string = DEFAULT_SUFFIX_STRING;
    suffix_enabled = DEFAULT_SUFFIX_ENABLED;
    extension_insensitive = DEFAULT_EXTENSION_INSENSITIVE;
    extension_adjust = DEFAULT_EXTENSION_ADJUST;

#ifndef IGNORE_FILE_DISABLED
    ignore_file_name = DEFAULT_IGNORE_FILE_NAME;
    ignore_file_enabled = DEFAULT_IGNORE_FILE_ENABLED;
#endif // IGNORE_FILE_DISABLED

#ifndef LOG_DISABLED
    log_enabled = DEFAULT_LOG_ENABLED;
#endif // LOG_DISABLED
}

Error Packer::pack_files() {
    if (read_path.empty()) {
        return Error::Unconfigured;
    }

    if (write_path.empty()) {
        return Error::Unconfigured;
    }

    if (pack_mode == PackMode::Include) {
        if (extensions.empty()) {
            return Error::Unconfigured;
        }
    }

    String _read_path = read_path;
#if EXPERIMENTAL_FILESYSTEM
    normalize_path_separators(_read_path);
#endif // EXPERIMENTAL_FILESYSTEM

    if (!FileAccess::exists(_read_path)) {
        return Error::DoesNotExist;
    }

    if (!FileAccess::is_directory(_read_path)) {
        _read_path = FileAccess::path(_read_path).parent_path().string();
    }

    bool _suffix_enabled = suffix_enabled;
    if (suffix_enabled && suffix_string.empty()) {
        _suffix_enabled = false;
    }

    bool _ignore_file_enabled = false;
#ifndef IGNORE_FILE_DISABLED
    _ignore_file_enabled = ignore_file_enabled;
    if (ignore_file_enabled && ignore_file_name.empty()) {
        _ignore_file_enabled = false;
    }
#endif // IGNORE_FILE_DISABLED

    bool _log_enabled = false;
#ifndef IGNORE_FILE_DISABLED
    _log_enabled = log_enabled;
#endif // IGNORE_FILE_DISABLED

    String _write_path = write_path;
#if EXPERIMENTAL_FILESYSTEM
    normalize_path_separators(_write_path);
#endif // EXPERIMENTAL_FILESYSTEM

    if (overwrite_files) {
        if (move_files) {
            if (_suffix_enabled) {
                if (extension_insensitive) {
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, true, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, true, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, true, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, true, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, true, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, true, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, true, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, true, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, true, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, true, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, true, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, true, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, true, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, true, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, true, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, true, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, true, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, true, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, true, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, true, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, true, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, true, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, true, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, true, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, true, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, true, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, true, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, true, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, true, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, true, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, true, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, true, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, true, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, true, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, true, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, true, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // extension_insensitive
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, false, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, false, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, false, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, false, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, false, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, false, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, false, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, false, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, false, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, false, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, false, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, false, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, false, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, false, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, false, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, false, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, false, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, false, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, false, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, false, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, false, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, false, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, false, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, false, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, false, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, false, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, false, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, false, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, false, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, false, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, false, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, false, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, false, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, true, false, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, true, false, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, true, false, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                }
            } else { // _suffix_enabled
                if (extension_insensitive) {
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, true, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, true, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, true, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, true, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, true, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, true, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, true, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, true, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, true, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, true, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, true, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, true, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, true, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, true, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, true, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, true, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, true, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, true, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, true, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, true, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, true, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, true, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, true, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, true, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, true, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, true, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, true, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, true, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, true, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, true, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, true, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, true, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, true, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, true, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, true, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, true, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // extension_insensitive
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, false, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, false, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, false, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, false, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, false, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, false, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, false, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, false, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, false, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, false, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, false, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, false, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, false, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, false, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, false, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, false, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, false, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, false, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, false, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, false, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, false, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, false, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, false, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, false, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, false, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, false, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, false, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, false, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, false, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, false, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, false, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, false, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, false, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, true, false, false, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, true, false, false, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, true, false, false, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                }
            }
        } else { // move_files
            if (_suffix_enabled) {
                if (extension_insensitive) {
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, true, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, true, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, true, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, true, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, true, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, true, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, true, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, true, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, true, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, true, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, true, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, true, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, true, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, true, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, true, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, true, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, true, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, true, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, true, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, true, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, true, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, true, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, true, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, true, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, true, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, true, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, true, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, true, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, true, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, true, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, true, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, true, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, true, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, true, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, true, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, true, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // extension_insensitive
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, false, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, false, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, false, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, false, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, false, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, false, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, false, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, false, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, false, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, false, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, false, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, false, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, false, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, false, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, false, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, false, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, false, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, false, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, false, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, false, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, false, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, false, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, false, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, false, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, false, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, false, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, false, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, false, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, false, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, false, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, false, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, false, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, false, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, true, false, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, true, false, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, true, false, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                }
            } else { // _suffix_enabled
                if (extension_insensitive) {
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, true, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, true, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, true, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, true, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, true, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, true, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, true, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, true, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, true, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, true, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, true, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, true, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, true, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, true, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, true, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, true, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, true, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, true, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, true, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, true, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, true, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, true, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, true, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, true, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, true, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, true, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, true, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, true, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, true, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, true, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, true, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, true, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, true, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, true, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, true, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, true, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // extension_insensitive
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, false, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, false, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, false, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, false, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, false, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, false, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, false, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, false, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, false, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, false, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, false, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, false, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, false, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, false, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, false, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, false, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, false, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, false, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, false, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, false, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, false, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, false, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, false, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, false, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, false, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, false, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, false, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, false, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, false, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, false, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, false, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, false, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, false, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<true, false, false, false, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<true, false, false, false, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<true, false, false, false, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                }
            }
        }
    } else { // overwrite_files
        if (move_files) {
            if (_suffix_enabled) {
                if (extension_insensitive) {
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, true, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, true, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, true, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, true, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, true, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, true, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, true, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, true, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, true, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, true, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, true, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, true, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, true, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, true, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, true, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, true, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, true, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, true, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, true, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, true, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, true, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, true, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, true, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, true, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, true, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, true, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, true, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, true, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, true, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, true, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, true, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, true, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, true, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, true, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, true, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, true, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // extension_insensitive
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, false, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, false, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, false, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, false, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, false, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, false, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, false, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, false, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, false, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, false, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, false, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, false, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, false, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, false, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, false, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, false, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, false, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, false, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, false, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, false, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, false, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, false, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, false, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, false, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, false, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, false, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, false, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, false, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, false, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, false, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, false, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, false, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, false, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, true, false, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, true, false, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, true, false, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                }
            } else { // _suffix_enabled
                if (extension_insensitive) {
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, true, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, true, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, true, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, true, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, true, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, true, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, true, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, true, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, true, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, true, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, true, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, true, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, true, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, true, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, true, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, true, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, true, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, true, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, true, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, true, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, true, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, true, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, true, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, true, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, true, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, true, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, true, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, true, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, true, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, true, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, true, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, true, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, true, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, true, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, true, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, true, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // extension_insensitive
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, false, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, false, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, false, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, false, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, false, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, false, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, false, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, false, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, false, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, false, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, false, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, false, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, false, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, false, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, false, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, false, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, false, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, false, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, false, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, false, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, false, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, false, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, false, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, false, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, false, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, false, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, false, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, false, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, false, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, false, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, false, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, false, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, false, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, true, false, false, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, true, false, false, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, true, false, false, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                }
            }
        } else { // move_files
            if (_suffix_enabled) {
                if (extension_insensitive) {
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, true, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, true, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, true, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, true, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, true, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, true, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, true, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, true, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, true, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, true, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, true, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, true, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, true, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, true, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, true, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, true, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, true, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, true, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, true, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, true, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, true, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, true, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, true, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, true, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, true, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, true, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, true, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, true, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, true, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, true, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, true, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, true, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, true, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, true, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, true, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, true, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // extension_insensitive
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, false, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, false, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, false, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, false, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, false, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, false, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, false, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, false, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, false, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, false, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, false, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, false, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, false, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, false, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, false, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, false, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, false, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, false, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, false, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, false, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, false, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, false, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, false, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, false, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, false, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, false, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, false, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, false, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, false, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, false, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, false, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, false, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, false, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, true, false, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, true, false, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, true, false, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                }
            } else { // _suffix_enabled
                if (extension_insensitive) {
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, true, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, true, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, true, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, true, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, true, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, true, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, true, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, true, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, true, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, true, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, true, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, true, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, true, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, true, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, true, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, true, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, true, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, true, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, true, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, true, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, true, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, true, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, true, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, true, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, true, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, true, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, true, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, true, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, true, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, true, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, true, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, true, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, true, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, true, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, true, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, true, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // extension_insensitive
                    if (_ignore_file_enabled) {
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, false, true, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, false, true, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, false, true, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, false, true, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, false, true, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, false, true, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, false, true, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, false, true, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, false, true, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, false, true, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, false, true, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, false, true, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, false, true, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, false, true, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, false, true, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, false, true, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, false, true, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, false, true, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // _ignore_file_enabled
                        if (_log_enabled) {
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, false, false, true, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, false, false, true, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, false, false, true, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, false, false, true, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, false, false, true, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, false, false, true, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, false, false, true, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, false, false, true, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, false, false, true, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        } else { // _log_enabled
                            if (pack_mode == PackMode::Include) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, false, false, false, PackMode::Include, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, false, false, false, PackMode::Include, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, false, false, false, PackMode::Include, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Exclude) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, false, false, false, PackMode::Exclude, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, false, false, false, PackMode::Exclude, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, false, false, false, PackMode::Exclude, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            } else if (pack_mode == PackMode::Everything) {
                                if (extension_adjust == ExtensionAdjust::Default) {
                                    _pack_files<false, false, false, false, false, false, PackMode::Everything, ExtensionAdjust::Default>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Lower) {
                                    _pack_files<false, false, false, false, false, false, PackMode::Everything, ExtensionAdjust::Lower>(_read_path, _write_path);
                                } else if (extension_adjust == ExtensionAdjust::Upper) {
                                    _pack_files<false, false, false, false, false, false, PackMode::Everything, ExtensionAdjust::Upper>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return Error::OK;
}

Packer::Packer() :
#ifndef IGNORE_FILE_DISABLED
    ignore_file_name(DEFAULT_IGNORE_FILE_NAME),
    ignore_file_enabled(DEFAULT_IGNORE_FILE_ENABLED),
#endif // IGNORE_FILE_DISABLED
#ifndef LOG_DISABLED
    log_enabled(DEFAULT_LOG_ENABLED),
#endif // LOG_DISABLED
    read_path(DEFAULT_READ_PATH),
    write_path(DEFAULT_WRITE_PATH),
    extensions(DEFAULT_EXTENTIONS),
    pack_mode(DEFAULT_PACK_MODE),
    overwrite_files(DEFAULT_OVERWRITE_FILES),
    move_files(DEFAULT_MOVE_FILES),
    suffix_string(DEFAULT_SUFFIX_STRING),
    suffix_enabled(DEFAULT_SUFFIX_ENABLED),
    extension_insensitive(DEFAULT_EXTENSION_INSENSITIVE),
    extension_adjust(DEFAULT_EXTENSION_ADJUST) {
}
