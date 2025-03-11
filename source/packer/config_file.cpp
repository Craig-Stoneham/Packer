// See LICENSE for full copyright and licensing information.

#include "config_file.h"

PACKER_NAMESPACE_BEGIN

void ConfigFile::set_value(const String& p_key, const Variant& p_value) {
    values[p_key] = p_value;
}

Variant ConfigFile::get_value(const String& p_key, Variant p_default) const {
    const auto& it = values.find(p_key);
    if (it != values.end()) {
        return it->second;
    }
    return p_default;
}

bool ConfigFile::has_value(const String& p_key) const {
    return values.find(p_key) != values.end();
}

bool ConfigFile::erase_value(const String& p_key) {
    const auto& it = values.find(p_key);
    if (it != values.end()) {
        values.erase(it);
        return true;
    }
    return false;
}

void ConfigFile::clear() {
    values.clear();
}

const Map<String, Variant>& ConfigFile::get_values() const {
    return values;
}

void ConfigFile::set_values(const Map<String, Variant>& p_values) {
    values = p_values;
}

void ConfigFile::get_keys(Vector<String>& p_keys) const {
    p_keys.resize(values.size());

    size_t index = 0;
    for (const auto& entry : values) {
        p_keys[index++] = entry.first;
    }
}

Error ConfigFile::save(const String& p_path) const {
    FileStreamO file(p_path, std::ios::binary);
    if (!file.is_open()) {
        return Error::FileNotFound;
    }

    for (const auto& pair : values) {
        String line = pair.first + "=" + pair.second.serialize() + "\n";
        file << line;
    }

    return Error::OK;
}

Error ConfigFile::load(const String& p_path) {
    FileStreamI file(p_path, std::ios::binary);
    if (!file.is_open()) {
        return Error::FileNotFound;
    }

    StringStream buffer;
    buffer << file.rdbuf();

    StringStreamI stream(buffer.str());
    String line;
    while (std::getline(stream, line)) {
        String key, value;
        size_t separator = line.find('=');
        if (separator != String::npos) {
            key = line.substr(0, separator);
            value = line.substr(separator + 1);
            Variant variant;
            variant.parse(value);
            values[key] = variant; // Store regardless of the parse error
        }
    }

    return Error::OK;
}

Error ConfigFile::save_encrypted(const String& p_path, const CryptoKey& p_key) const {
    FileStreamO file(p_path, std::ios::binary);
    if (!file.is_open()) {
        return Error::FileNotFound;
    }

    StringStreamO output;

    for (const auto& entry : values) {
        output << entry.first << "=" << entry.second.serialize() << "\n";
    }

    file << Crypto::encrypt_decrypt(output.str(), p_key);

    return Error::OK;
}

Error ConfigFile::load_encrypted(const String& p_path, const CryptoKey& p_key) {
    FileStreamI file(p_path, std::ios::binary);
    if (!file.is_open()) {
        return Error::FileNotFound;
    }

    StringStream buffer;
    buffer << file.rdbuf();

    String decrypted_data = Crypto::encrypt_decrypt(buffer.str(), p_key);

    StringStreamI stream(decrypted_data);
    String line;
    while (std::getline(stream, line)) {
        String key, value;
        size_t separator = line.find('=');
        if (separator != String::npos) {
            key = line.substr(0, separator);
            value = line.substr(separator + 1);
            Variant variant;
            variant.parse(value);
            values[key] = variant; // Store regardless of the parse error
        }
    }

    return Error::OK;
}

ConfigFile::ConfigFile() {
}

PACKER_NAMESPACE_END