// See LICENSE for full copyright and licensing information.

#include "packer.h"

PACKER_NAMESPACE_BEGIN

static const char* pack_mode_names[] = {
    "include",
    "exclude",
    "everything"
};

static Packer::Callback callback = nullptr;

String Packer::get_pack_mode_name(PackMode p_mode) {
    if (p_mode >= static_cast<PackMode>(0) && p_mode < PackMode::Max) {
        return pack_mode_names[static_cast<size_t>(p_mode)];
    } else {
        return "unknown";
    }
}

Packer::PackMode Packer::find_pack_mode(const String& p_mode) {
    for (size_t i = 0; i < static_cast<size_t>(PackMode::Max); ++i) {
        if (p_mode == pack_mode_names[i]) {
            return static_cast<PackMode>(i);
        }
    }
    return PackMode::Unknown;
}

static const char* extension_adjust_names[] = {
    "default",
    "lower",
    "upper"
};

String Packer::get_extension_adjust_name(ExtensionAdjust p_adjust) {
    if (p_adjust >= static_cast<ExtensionAdjust>(0) && p_adjust < ExtensionAdjust::Max) {
        return extension_adjust_names[static_cast<size_t>(p_adjust)];
    } else {
        return "unknown";
    }
}

Packer::ExtensionAdjust Packer::find_extension_adjust(const String& p_adjust) {
    for (size_t i = 0; i < static_cast<size_t>(ExtensionAdjust::Max); ++i) {
        if (p_adjust == extension_adjust_names[i]) {
            return static_cast<ExtensionAdjust>(i);
        }
    }
    return ExtensionAdjust::Unknown;
}

void Packer::_pack_files(const String& p_read_path, const String& p_write_path) {
#ifdef IGNORE_FILE_ENABLED
    if (ignore_file_enabled) {
        if (FileAccess::is_directory(p_read_path) == true) {
            if (FileAccess::exists(p_read_path + "/" + ignore_file_name)) {
                return;
            }
        }
    }
#endif //IGNORE_FILE_ENABLED

    bool write_directory_exists = false;

    for (auto& path : FileAccess::directory_iterator(p_read_path)) {
        String _read_path = path.path().string();
        normalize_path_separators(_read_path);

        if (FileAccess::is_directory(path)) {
            _pack_files(_read_path, p_write_path + _read_path.substr(_read_path.find_last_of('/')));
        } else {
            if (pack_mode != PackMode::Everything) {
                String extension = _read_path.substr(_read_path.find_last_of('.') + 1);
                bool skip_file = pack_mode == PackMode::Include;

                if (extension_insensitive) {
                    std::transform(extension.begin(), extension.end(), extension.begin(), tolower);
                    for (const String& e : extensions) {
                        String transformed = e;
                        std::transform(transformed.begin(), transformed.end(), transformed.begin(), tolower);

                        if (extension == transformed) {
                            skip_file = pack_mode == PackMode::Exclude;
                            break;
                        }
                    }
                } else {
                    for (const String& e : extensions) {
                        if (extension == e) {
                            skip_file = pack_mode == PackMode::Exclude;
                            break;
                        }                        
                    }
                }

                if (skip_file) {
                    continue;
                }
            }

            String _write_path = p_write_path + _read_path.substr(_read_path.find_last_of('/'));

            if (suffix_enabled) {
                remove_path_suffix(_write_path, suffix_string);
            }

            if (extension_adjust != ExtensionAdjust::Default) {
                size_t ext_pos = _write_path.find_last_of('.') + 1;
                if (ext_pos != String::npos) {
                    std::transform(_write_path.begin() + ext_pos, _write_path.end(), _write_path.begin() + ext_pos, extension_adjust == ExtensionAdjust::Lower ? tolower : toupper);
                }
            }

            if (overwrite_files == false) {
                if (FileAccess::exists(_write_path)) {
                    continue;
                }
            }

            if (write_directory_exists == false) {
                write_directory_exists = true;
                FileAccess::create_directories(p_write_path);
            }

            if (copy_file(_read_path, _write_path, FileAccess::copy_options::update_existing) == false) {
                continue;
            }

            if (move_files) {
                FileAccess::remove(_read_path);
            }

            if (callback) {
                callback(_read_path, _write_path, move_files);
            }

#ifdef LOG_ENABLED
            if (log_enabled) {
                LOG_INFO((move_files ? "Moved " : "Copied ") + _read_path + " to " + _write_path + "\n");
            }
#endif // LOG_ENABLED
        }
    }
}

void Packer::set_callback(Callback p_callback) {
    callback = p_callback;
}

Packer::Callback Packer::get_callback() {
    return callback;
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

#ifdef IGNORE_FILE_ENABLED

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

#ifdef LOG_ENABLED

void Packer::set_log_enabled(bool p_enable) {
    log_enabled = p_enable;
}

bool Packer::get_log_enabled() const {
    return log_enabled;
}

#endif // LOG_ENABLED

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

#ifdef IGNORE_FILE_ENABLED
    p_file.set_value("ignore_file_name", ignore_file_name);
    p_file.set_value("ignore_file_enabled", ignore_file_enabled);
#endif // IGNORE_FILE_ENABLED

#ifdef LOG_ENABLED
    p_file.set_value("log_enabled", log_enabled);
#endif // LOG_ENABLED
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

#ifdef IGNORE_FILE_ENABLED
    ignore_file_name = p_file.get_value("ignore_file_name", DEFAULT_IGNORE_FILE_NAME);
    ignore_file_enabled = p_file.get_value("ignore_file_enabled", DEFAULT_IGNORE_FILE_ENABLED);
#endif // IGNORE_FILE_ENABLED

#ifdef LOG_ENABLED
    log_enabled = p_file.get_value("log_enabled", DEFAULT_LOG_ENABLED);
#endif // LOG_ENABLED
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

#ifdef IGNORE_FILE_ENABLED
    ignore_file_name = DEFAULT_IGNORE_FILE_NAME;
    ignore_file_enabled = DEFAULT_IGNORE_FILE_ENABLED;
#endif // IGNORE_FILE_ENABLED

#ifdef LOG_ENABLED
    log_enabled = DEFAULT_LOG_ENABLED;
#endif // LOG_ENABLED
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
    normalize_path_separators(_read_path);

    if (!FileAccess::exists(_read_path)) {
        return Error::DoesNotExist;
    }

    if (!FileAccess::is_directory(_read_path)) {
        _read_path = FileAccess::path(_read_path).parent_path().string();
    }

    String _write_path = write_path;
    normalize_path_separators(_write_path);

    _pack_files(_read_path, _write_path);

    return Error::OK;
}

Packer::Packer() :
#ifdef IGNORE_FILE_ENABLED
    ignore_file_name(DEFAULT_IGNORE_FILE_NAME),
    ignore_file_enabled(DEFAULT_IGNORE_FILE_ENABLED),
#endif // IGNORE_FILE_ENABLED
#ifdef LOG_ENABLED
    log_enabled(DEFAULT_LOG_ENABLED),
#endif // LOG_ENABLED
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

PACKER_NAMESPACE_END
