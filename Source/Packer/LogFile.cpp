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

#include "LogFile.h"

PACKER_NAMESPACE_BEGIN

static void log_callback(void* p_logger, Log::Level p_level, const String& p_string) {
    static_cast<LogFile*>(p_logger)->log_string(p_level, p_string);
}

void LogFile::log_string(Log::Level p_level, const String& p_string) {
    if (!stream.is_open()) {
        return;
    }
    if (print_level_details) {
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

void LogFile::set_print_level_details(bool p_enabled) {
    print_level_details = p_enabled;
}

bool LogFile::get_print_level_details() const {
    return print_level_details;
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

LogFile::LogFile(bool p_print_level_details) :
    print_level_details(p_print_level_details) {
#ifdef LOG_ENABLED
    Log::add_callback(log_callback, this);
#endif // LOG_ENABLED
}

LogFile::LogFile(const String& p_path, bool p_print_level_details) :
    print_level_details(p_print_level_details) {
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
