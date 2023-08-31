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

template <bool EVERYTHING, bool OVERWRITE, bool MOVE, bool SUFFIX, bool EXT_INSENSITIVE, ExtensionCase EXT_ADJUST, bool IGNORE_FILE, bool LOG>
void Packer::_pack_files(const String& p_read_path, const String& p_write_path) {
#ifndef IGNORE_FILE_DISABLED
    if (IGNORE_FILE) {
        if (FileAccess::is_directory(p_read_path) == true) {
            if (FileAccess::exists(p_read_path + "\\" + ignore_file_name)) {
                return;
            }
        }
    }
#endif //IGNORE_FILE_DISABLED

    bool write_directory_exists = false;

    for (auto& path : FileAccess::directory_iterator(p_read_path)) {
        String _read_path = path.path().string();

        if (FileAccess::is_directory(path)) {
            String _write_path = p_write_path + _read_path.substr(_read_path.find_last_of('\\'));
            _pack_files<EVERYTHING, OVERWRITE, MOVE, SUFFIX, EXT_INSENSITIVE, EXT_ADJUST, IGNORE_FILE, LOG>(_read_path, _write_path);
        } else {
            if (!EVERYTHING) {
                String extension = _read_path.substr(_read_path.find_last_of('.') + 1);

                if (EXT_INSENSITIVE) {
                    std::transform(extension.begin(), extension.end(), extension.begin(), tolower);
                }

                for (const String& e : extensions) {
                    if (EXT_INSENSITIVE) {
                        String transformed = e;
                        std::transform(transformed.begin(), transformed.end(), transformed.begin(), tolower);

                        if (extension == transformed) {
                            goto extension_found;
                        }
                    } else {
                        if (extension == e) {
                            goto extension_found;
                        }
                    }
                }
                continue;
            extension_found: {}
            }

            String _write_path = p_write_path + _read_path.substr(_read_path.find_last_of('\\'));

            if (SUFFIX) {
                size_t suffix_pos = _write_path.find(suffix_string, _write_path.find('\\'));
                if (suffix_pos != String::npos) {
                    _write_path.erase(suffix_pos, suffix_string.length());
                }
            }

            if (EXT_ADJUST == ExtensionCase::Lower) {
                size_t ext_pos = _write_path.find_last_of('.') + 1;
                std::transform(_write_path.begin() + ext_pos, _write_path.end(), _write_path.begin() + ext_pos, tolower);
            } else if (EXT_ADJUST == ExtensionCase::Upper) {
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

const String Packer::get_extension(size_t p_index) const {
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

void Packer::set_pack_everything(bool p_enable) {
    pack_everything = p_enable;
}

bool Packer::get_pack_everything() const {
    return pack_everything;
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

void Packer::set_extension_adjust(ExtensionCase p_case) {
    extension_adjust = static_cast<int>(p_case);
}

ExtensionCase Packer::get_extension_adjust() const {
    return static_cast<ExtensionCase>(extension_adjust);
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
    auto set_value = [&p_file](auto& p_name, auto& p_param) {
        p_file.set_value(p_name, p_param);
        };

    p_file.set_value("Version", VERSION_STRING);

    set_value("read_path", read_path);
    set_value("write_path", write_path);
    set_value("extensions", extensions);
    set_value("pack_everything", pack_everything);
    set_value("overwrite_files", overwrite_files);
    set_value("move_files", move_files);
    set_value("suffix_string", suffix_string);
    set_value("suffix_enabled", suffix_enabled);
    set_value("extension_insensitive", extension_insensitive);
    set_value("extension_adjust", extension_adjust);

#ifndef IGNORE_FILE_DISABLED
    set_value("ignore_file_name", ignore_file_name);
    set_value("ignore_file_enabled", ignore_file_enabled);
#endif // IGNORE_FILE_DISABLED

#ifndef LOG_DISABLED
    set_value("log_enabled", log_enabled);
#endif // LOG_DISABLED
}

void Packer::from_config_file(const ConfigFile& p_file) {
    auto set_value = [&p_file](auto& p_name, auto& p_param, auto p_default) {
        if (p_file.has_value(p_name)) {
            p_param = p_file.get_value(p_name);
        } else {
            p_param = p_default;
        }
        };

    set_value("read_path", read_path, DEFAULT_READ_PATH);
    set_value("write_path", write_path, DEFAULT_WRITE_PATH);
    set_value("extensions", extensions, DEFAULT_EXTENTIONS);
    set_value("pack_everything", pack_everything, DEFAULT_PACK_EVERYTHING);
    set_value("overwrite_files", overwrite_files, DEFAULT_OVERWRITE_FILES);
    set_value("move_files", move_files, DEFAULT_MOVE_FILES);
    set_value("suffix_string", suffix_string, DEFAULT_SUFFIX_STRING);
    set_value("suffix_enabled", suffix_enabled, DEFAULT_SUFFIX_ENABLED);
    set_value("extension_insensitive", extension_insensitive, DEFAULT_EXTENSION_INSENSITIVE);
    set_value("extension_adjust", extension_adjust, DEFAULT_EXTENSION_ADJUST);

#ifndef IGNORE_FILE_DISABLED
    set_value("ignore_file_name", ignore_file_name, DEFAULT_IGNORE_FILE_NAME);
    set_value("ignore_file_enabled", ignore_file_enabled, DEFAULT_IGNORE_FILE_ENABLED);
#endif // IGNORE_FILE_DISABLED

#ifndef LOG_DISABLED
    set_value("log_enabled", log_enabled, DEFAULT_LOG_ENABLED);
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
    auto set_value = [](auto p_param, auto p_default) {
        p_param = p_default;
        };

    set_value(read_path, DEFAULT_READ_PATH);
    set_value(write_path, DEFAULT_WRITE_PATH);
    set_value(extensions, DEFAULT_EXTENTIONS);
    set_value(pack_everything, DEFAULT_PACK_EVERYTHING);
    set_value(overwrite_files, DEFAULT_OVERWRITE_FILES);
    set_value(move_files, DEFAULT_MOVE_FILES);
    set_value(suffix_string, DEFAULT_SUFFIX_STRING);
    set_value(suffix_enabled, DEFAULT_SUFFIX_ENABLED);
    set_value(extension_insensitive, DEFAULT_EXTENSION_INSENSITIVE);
    set_value(extension_adjust, DEFAULT_EXTENSION_ADJUST);

#ifndef IGNORE_FILE_DISABLED
    set_value(ignore_file_name, DEFAULT_IGNORE_FILE_NAME);
    set_value(ignore_file_enabled, DEFAULT_IGNORE_FILE_ENABLED);
#endif // IGNORE_FILE_DISABLED

#ifndef LOG_DISABLED
    set_value(log_enabled, DEFAULT_LOG_ENABLED);
#endif // LOG_DISABLED
}

Error Packer::pack_files() {

    if (read_path.empty()) {
        return Error::Unconfigured;
    }

    if (write_path.empty()) {
        return Error::Unconfigured;
    }

    if (extensions.empty()) {
        return Error::Unconfigured;
    }

    String _read_path = read_path;

    if (!FileAccess::exists(_read_path)) {
        return Error::DoesNotExist;
    }

    if (!FileAccess::is_directory(_read_path)) {
        _read_path = FileAccess::path(_read_path).parent_path().string();
    }

    String _write_path = write_path;

    if (!FileAccess::is_directory(_write_path)) {
        //_write_path = FileAccess::path(_write_path).parent_path().string();
    }

    bool _suffix_enabled = suffix_enabled;
    if (suffix_enabled && suffix_string.empty()) {
        _suffix_enabled = false;
    }

    ExtensionCase _extension_adjust = static_cast<ExtensionCase>(extension_adjust);

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

    if (pack_everything) {
        if (overwrite_files) {
            if (move_files) {
                if (_suffix_enabled) {
                    if (extension_insensitive) {
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, true, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, true, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, true, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, true, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, true, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, true, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, true, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, true, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, true, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, true, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, true, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, true, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, true, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, true, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, true, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, true, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, true, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, true, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, true, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, true, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, true, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, true, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, true, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, true, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // _suffix_enabled
                    if (extension_insensitive) {
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, false, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, false, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, false, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, false, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, false, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, false, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, false, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, false, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, false, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, false, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, false, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, false, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, false, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, false, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, false, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, false, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, false, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, false, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, false, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, false, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, false, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, true, false, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, true, false, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, true, false, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
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
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, true, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, true, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, true, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, true, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, true, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, true, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, true, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, true, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, true, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, true, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, true, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, true, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, true, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, true, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, true, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, true, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, true, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, true, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, true, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, true, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, true, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, true, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, true, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, true, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // _suffix_enabled
                    if (extension_insensitive) {
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, false, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, false, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, false, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, false, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, false, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, false, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, false, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, false, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, false, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, false, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, false, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, false, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, false, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, false, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, false, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, false, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, false, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, false, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, false, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, false, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, false, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, true, false, false, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, true, false, false, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, true, false, false, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
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
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, true, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, true, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, true, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, true, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, true, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, true, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, true, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, true, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, true, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, true, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, true, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, true, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, true, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, true, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, true, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, true, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, true, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, true, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, true, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, true, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, true, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, true, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, true, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, true, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // _suffix_enabled
                    if (extension_insensitive) {
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, false, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, false, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, false, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, false, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, false, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, false, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, false, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, false, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, false, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, false, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, false, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, false, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, false, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, false, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, false, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, false, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, false, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, false, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, false, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, false, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, false, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, true, false, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, true, false, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, true, false, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
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
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, true, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, true, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, true, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, true, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, true, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, true, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, true, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, true, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, true, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, true, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, true, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, true, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, true, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, true, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, true, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, true, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, true, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, true, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, true, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, true, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, true, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, true, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, true, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, true, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // _suffix_enabled
                    if (extension_insensitive) {
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, false, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, false, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, false, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, false, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, false, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, false, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, false, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, false, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, false, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, false, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, false, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, false, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, false, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, false, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, false, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, false, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, false, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, false, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, false, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, false, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, false, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<true, false, false, false, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<true, false, false, false, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<true, false, false, false, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                }
            }
        }
    } else { // pack_everything
        if (overwrite_files) {
            if (move_files) {
                if (_suffix_enabled) {
                    if (extension_insensitive) {
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, true, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, true, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, true, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, true, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, true, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, true, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, true, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, true, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, true, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, true, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, true, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, true, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, true, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, true, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, true, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, true, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, true, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, true, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, true, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, true, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, true, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, true, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, true, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, true, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // _suffix_enabled
                    if (extension_insensitive) {
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, false, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, false, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, false, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, false, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, false, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, false, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, false, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, false, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, false, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, false, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, false, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, false, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, false, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, false, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, false, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, false, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, false, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, false, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, false, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, false, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, false, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, true, false, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, true, false, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, true, false, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
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
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, true, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, true, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, true, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, true, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, true, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, true, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, true, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, true, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, true, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, true, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, true, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, true, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, true, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, true, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, true, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, true, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, true, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, true, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, true, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, true, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, true, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, true, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, true, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, true, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // _suffix_enabled
                    if (extension_insensitive) {
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, false, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, false, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, false, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, false, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, false, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, false, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, false, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, false, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, false, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, false, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, false, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, false, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, false, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, false, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, false, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, false, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, false, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, false, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, false, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, false, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, false, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, true, false, false, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, true, false, false, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, true, false, false, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
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
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, true, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, true, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, true, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, true, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, true, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, true, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, true, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, true, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, true, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, true, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, true, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, true, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, true, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, true, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, true, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, true, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, true, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, true, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, true, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, true, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, true, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, true, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, true, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, true, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // _suffix_enabled
                    if (extension_insensitive) {
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, false, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, false, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, false, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, false, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, false, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, false, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, false, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, false, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, false, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, false, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, false, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, false, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, false, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, false, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, false, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, false, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, false, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, false, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, false, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, false, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, false, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, true, false, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, true, false, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, true, false, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
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
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, true, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, true, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, true, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, true, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, true, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, true, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, true, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, true, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, true, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, true, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, true, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, true, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, true, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, true, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, true, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, true, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, true, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, true, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, true, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, true, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, true, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, true, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, true, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, true, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    }
                } else { // _suffix_enabled
                    if (extension_insensitive) {
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, false, true, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, false, true, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, false, true, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, false, true, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, false, true, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, false, true, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, false, true, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, false, true, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, false, true, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, false, true, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, false, true, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, false, true, ExtensionCase::Upper, false, false>(_read_path, _write_path);
                                }
                            }
                        }
                    } else { // extension_insensitive
                        if (_ignore_file_enabled) {
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, false, false, ExtensionCase::Default, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, false, false, ExtensionCase::Lower, true, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, false, false, ExtensionCase::Upper, true, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, false, false, ExtensionCase::Default, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, false, false, ExtensionCase::Lower, true, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, false, false, ExtensionCase::Upper, true, false>(_read_path, _write_path);
                                }
                            }
                        } else { // _ignore_file_enabled
                            if (_log_enabled) {
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, false, false, ExtensionCase::Default, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, false, false, ExtensionCase::Lower, false, true>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, false, false, ExtensionCase::Upper, false, true>(_read_path, _write_path);
                                }
                            } else { // _log_enabled
                                if (_extension_adjust == ExtensionCase::Default) {
                                    _pack_files<false, false, false, false, false, ExtensionCase::Default, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Lower) {
                                    _pack_files<false, false, false, false, false, ExtensionCase::Lower, false, false>(_read_path, _write_path);
                                } else if (_extension_adjust == ExtensionCase::Upper) {
                                    _pack_files<false, false, false, false, false, ExtensionCase::Upper, false, false>(_read_path, _write_path);
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
    pack_everything(DEFAULT_PACK_EVERYTHING),
    overwrite_files(DEFAULT_OVERWRITE_FILES),
    move_files(DEFAULT_MOVE_FILES),
    suffix_string(DEFAULT_SUFFIX_STRING),
    suffix_enabled(DEFAULT_SUFFIX_ENABLED),
    extension_insensitive(DEFAULT_EXTENSION_INSENSITIVE),
    extension_adjust(DEFAULT_EXTENSION_ADJUST) {
}
