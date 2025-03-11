// See LICENSE for full copyright and licensing information.

#include "log_file.h"

PACKER_NAMESPACE_BEGIN

static void log_callback(void* p_logger, Log::Level p_level, const String& p_string) {
    static_cast<LogFile*>(p_logger)->log_string(p_level, p_string);
}

void LogFile::log_string(Log::Level p_level, const String& p_string) {
    if (!stream.is_open()) {
        return;
    }
    if (print_level) {
        stream << Log::get_level_name(p_level) << ":";
    }
    stream << p_string;
}

void LogFile::log_info(const String& p_string) {
    log_string(Log::Level::Info, p_string);
}

void LogFile::log_warn(const String& p_string) {
    log_string(Log::Level::Warn, p_string);
}

void LogFile::log_error(const String& p_string) {
    log_string(Log::Level::Error, p_string);
}

void LogFile::set_print_level(bool p_enabled) {
    print_level = p_enabled;
}

bool LogFile::get_print_level() const {
    return print_level;
}

Error LogFile::open(const String& p_path) {
    if (stream.is_open()) {
        return Error::FileAlreadyInUse;
    }

    stream.open(p_path, std::ios::binary);

    return stream.is_open() ? Error::OK : Error::FileCantOpen;
}

Error LogFile::close() {
    if (stream.is_open()) {
        return Error::FileAlreadyInUse;
    } else {
        stream.close();
        return Error::OK;
    }
}

LogFile::LogFile(bool p_print_level) :
    print_level(p_print_level) {
#ifdef LOG_ENABLED
    Log::add_callback(log_callback, this);
#endif // LOG_ENABLED
}

LogFile::LogFile(const String& p_path, bool p_print_level) :
    print_level(p_print_level) {
#ifdef LOG_ENABLED
    Log::add_callback(log_callback, this);
#endif // LOG_ENABLED
    stream.open(p_path, std::ios::binary);
}

LogFile::~LogFile() {
#ifdef LOG_ENABLED
    Log::remove_callback(log_callback, this);
#endif // LOG_ENABLED
    if (stream.is_open()) {
        stream.close();
    }
}

PACKER_NAMESPACE_END
