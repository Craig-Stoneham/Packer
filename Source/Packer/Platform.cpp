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

#include "Platform.h"
#include "Console.h"

#ifndef CONSOLE_FEATURES_DISABLED

#ifdef _WIN32
#include <windows.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <iostream>
#endif // _WIN32

#ifdef _WIN32
#ifndef CONSOLE_FEATURES_DISABLED
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
#endif // CONSOLE_FEATURES_DISABLED

bool Platform::set_console_text_color(Color p_color) {
#ifndef CONSOLE_FEATURES_DISABLED
    if (p_color >= static_cast<Color>(0) && p_color < Color::Max) {
        if (!SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text_colors[static_cast<int>(p_color)])) {
            return false;
        }
    } else {
        return false;
    }
#endif // CONSOLE_FEATURES_DISABLED
    return true;
}

#elif defined(__unix__) || defined(__APPLE__)

#ifndef CONSOLE_FEATURES_DISABLED
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
#endif // CONSOLE_FEATURES_DISABLED

bool Platform::set_console_text_color(int p_color) {
#ifndef CONSOLE_FEATURES_DISABLED
    if (p_color >= static_cast<Color>(0) && p_color < Color::Max) {
        std::cout << "\x1B[" << text_colors[p_color] << "m";
    } else {
        return false;
    }
#endif // CONSOLE_FEATURES_DISABLED

    return true;
}

#endif // defined(__unix__) || defined(__APPLE__)

#endif // CONSOLE_FEATURES_DISABLED
