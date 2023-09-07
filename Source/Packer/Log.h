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

#include "Typedefs.h"

class Log {
public:
    enum class Level {
        Unknown = -1,
        Info,
        Warn,
        Error,
        Max
    };

    using Callback = void (*)(void* p_data, Level p_level, const String& p_string);

    static String get_level_string(Level p_level);
    static Level find_level(const String& p_level);

    static void add_callback(Callback p_callback, void* p_data);
    static void remove_callback(Callback p_callback, void* p_data);

    static void log_string(Level p_level, const String& p_string);
    static void log_info(const String& p_string);
    static void log_warn(const String& p_string);
    static void log_error(const String& p_string);
};

#ifndef LOG_DISABLED

#define LOG_STRING(p_level, p_string) Log::log_string(p_level, p_string)
#define LOG_INFO(p_string) Log::log_info(p_string)
#define LOG_WARN(p_string) Log::log_warn(p_string)
#define LOG_ERROR(p_string) Log::log_error(p_string)

#else // LOG_DISABLED

#define LOG_STRING(p_level, p_string)
#define LOG_INFO(p_string)
#define LOG_WARN(p_string)
#define LOG_ERROR(p_string)

#endif // LOG_DISABLED
