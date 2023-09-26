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

#include "Log.h"

PACKER_NAMESPACE_BEGIN

static const char* level_names[] = {
    "Info",
    "Warn",
    "Error"
};

String Log::get_level_name(Level p_level) {
    if (p_level >= static_cast<Level>(0) && p_level < Level::Max) {
        return level_names[static_cast<size_t>(p_level)];
    } else {
        return "Unknown";
    }
}

Log::Level Log::find_level(const String& p_level) {
    for (size_t i = 0; i < static_cast<size_t>(Level::Max); ++i) {
        if (p_level == level_names[i]) {
            return static_cast<Level>(i);
        }
    }
    return static_cast<Level>(-1);
}

#ifdef LOG_ENABLED
struct CallbackData {
    Log::Callback callback;
    void* data;
};

static Vector<CallbackData> callbacks;
#endif // LOG_ENABLED


void Log::add_callback(Callback p_callback, void* p_data) {
#ifdef LOG_ENABLED
    CallbackData callback_data;

    callback_data.callback = p_callback;
    callback_data.data = p_data;

    callbacks.push_back(callback_data);
#endif // LOG_ENABLED
}

void Log::remove_callback(Callback p_callback, void* p_data) {
#ifdef LOG_ENABLED
    for (size_t i = 0; i < callbacks.size(); ++i) {
        if (callbacks[i].callback == p_callback && callbacks[i].data == p_data) {
            callbacks.erase(callbacks.begin() + i);
            break;
        }
    }
#endif // LOG_ENABLED
}

void Log::log_string(Level p_level, const String& p_string) {
#ifdef LOG_ENABLED
    for (const auto& callback_data : callbacks) {
        callback_data.callback(callback_data.data, p_level, p_string);
    }
#endif // LOG_ENABLED
}

void Log::log_info(const String& p_string) {
#ifdef LOG_ENABLED
    log_string(Level::Info, p_string);
#endif // LOG_ENABLED
}

void Log::log_warn(const String& p_string) {
#ifdef LOG_ENABLED
    log_string(Level::Warn, p_string);
#endif // LOG_ENABLED
}

void Log::log_error(const String& p_string) {
#ifdef LOG_ENABLED
    log_string(Level::Error, p_string);
#endif // LOG_ENABLED
}

PACKER_NAMESPACE_END
