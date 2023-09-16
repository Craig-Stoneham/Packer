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

#include "Console.h"

#ifndef CONSOLE_FEATURES_DISABLED
#ifdef _WIN32
#include <windows.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <iostream>
#endif // (__unix__) || defined(__APPLE__)
#endif // CONSOLE_FEATURES_DISABLED

PACKER_NAMESPACE_BEGIN

#ifndef CONSOLE_FEATURES_DISABLED
#ifdef _WIN32
static WORD text_colors[] = {
    0, // None
    FOREGROUND_RED, // Red
    FOREGROUND_GREEN, // Green
    FOREGROUND_BLUE, // Blue
    FOREGROUND_RED | FOREGROUND_GREEN, // Yellow
    FOREGROUND_RED | FOREGROUND_BLUE, // Magenta
    FOREGROUND_GREEN | FOREGROUND_BLUE, // Cyan
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, // White
    0 | FOREGROUND_INTENSITY, // Grey
    FOREGROUND_RED | FOREGROUND_INTENSITY, // LightRed
    FOREGROUND_GREEN | FOREGROUND_INTENSITY, // LightGreen
    FOREGROUND_BLUE | FOREGROUND_INTENSITY, // LightBlue
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, // LightYellow
    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // LightMagenta
    FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // LightCyan
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY // BrightWhite
};
#elif defined(__unix__) || defined(__APPLE__)
static int text_colors[] = {
    0,   // None
    31,  // Red
    32,  // Green
    34,  // Blue
    33,  // Yellow
    35,  // Magenta
    36,  // Cyan
    37,  // White
    90,  // Grey
    91,  // LightRed
    92,  // LightGreen
    94,  // LightBlue
    93,  // LightYellow
    95,  // LightMagenta
    96,  // LightCyan
    97   // BrightWhite
};
#endif // (__unix__) || defined(__APPLE__)
#endif // CONSOLE_FEATURES_DISABLED

#ifndef CONSOLE_FEATURES_DISABLED
static bool set_console_text_color(Console::Color p_color) {
    if (p_color < static_cast<Console::Color>(0) || p_color >= Console::Color::Max) {
        return false;
    }
#ifdef _WIN32
    if (!SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text_colors[static_cast<int>(p_color)])) {
        return false;
    }
#elif defined(__unix__) || defined(__APPLE__)
    std::cout << "\x1B[" << text_colors[p_color] << "m";
#endif // (__unix__) || defined(__APPLE__)
    return true;
}
#endif // CONSOLE_FEATURES_DISABLED

#ifndef LOG_DISABLED
static void log_callback(void* p_logger, Log::Level p_level, const String& p_string) {
    static_cast<Console*>(p_logger)->print_string(p_level, p_string);
}
#endif // LOG_DISABLED

#ifndef CONSOLE_FEATURES_DISABLED

void Console::set_text_color(Log::Level p_level, Color p_color) {
    if (static_cast<int>(p_level) < 0 || p_level >= Log::Level::Max) {
        return;
    }
    if (static_cast<int>(p_color) < 0 || p_color >= Color::Max) {
        return;
    }
    text_colors[static_cast<int>(p_level)] = p_color;
}

Console::Color Console::get_text_color(Log::Level p_level) const {
    if (static_cast<int>(p_level) < 0 || p_level >= Log::Level::Max) {
        return Color::None;
    }
    return text_colors[static_cast<int>(p_level)];
}

void Console::set_text_color(Color p_color) {
    if (static_cast<int>(p_color) < 0 || p_color >= Color::Max) {
        return;
    }
    current_color = p_color;
    set_console_text_color(p_color);
}

Console::Color Console::get_text_color() const {
    return current_color;
}

#endif // CONSOLE_FEATURES_DISABLED

void Console::print_string(const String& p_string) {
    std::cout << p_string;
}

void Console::print_line(const String& p_string) {
    print_string(p_string + "\n");
}

void Console::print_string(Log::Level p_level, const String& p_string) {
#ifndef CONSOLE_FEATURES_DISABLED
    set_console_text_color(text_colors[static_cast<int>(p_level)]);
#endif // CONSOLE_FEATURES_DISABLED

    print_string(p_string);

#ifndef CONSOLE_FEATURES_DISABLED
    set_console_text_color(current_color);
#endif // CONSOLE_FEATURES_DISABLED
}

void Console::print_info(const String& p_string) {
    print_string(Log::Level::Info, p_string);
}

void Console::print_warn(const String& p_string) {
    print_string(Log::Level::Warn, p_string);
}

void Console::print_error(const String& p_string) {
    print_string(Log::Level::Error, p_string);
}

void Console::log_string(Log::Level p_level, const String& p_string) {
    LOG_STRING(p_level, p_string);
}

void Console::log_info(const String& p_string) {
    LOG_INFO(p_string);
}

void Console::log_warn(const String& p_string) {
    LOG_WARN(p_string);
}

void Console::log_error(const String& p_string) {
    LOG_ERROR(p_string);
}

Console::Console()
#ifndef CONSOLE_FEATURES_DISABLED
    :
text_colors{ Color::White, Color::LightRed, Color::Red },
current_color(Color::White)
#endif // CONSOLE_FEATURES_DISABLED 
{
#ifndef LOG_DISABLED
    Log::add_callback(log_callback, this);
#endif // LOG_DISABLED
}

Console::~Console() {
#ifndef LOG_DISABLED
    Log::remove_callback(log_callback, this);
#endif // LOG_DISABLED
}

PACKER_NAMESPACE_END
