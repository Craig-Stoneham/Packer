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

/**
 * @file Console.h
 * @brief Defines a console utility class for managing console output and text colors.
 */

#pragma once

#include "Log.h"

/**
 * @class Console
 * @brief Provides functionality for managing console output and text colors.
 * 
 * The `Console` class allows you to control the text colors used for different log levels
 * and provides methods for printing and logging messages to the console with various log levels.
 */
class Console {
public:
#ifndef CONSOLE_FEATURES_DISABLED
    /**
     * @enum Color
     * @brief Represents text color options.
     *
     * This enumeration defines various text color options that can be used
     * in the application's console output.
     */
    enum class Color {
        None,           /**< No color */
        Red,            /**< Red color */
        Green,          /**< Green color */
        Blue,           /**< Blue color */
        Yellow,         /**< Yellow color */
        Magenta,        /**< Magenta color */
        Cyan,           /**< Cyan color */
        White,          /**< White color */
        Grey,           /**< Grey color */
        LightRed,       /**< Light red color */
        LightGreen,     /**< Light green color */
        LightBlue,      /**< Light blue color */
        LightYellow,    /**< Light yellow color */
        LightMagenta,   /**< Light magenta color */
        LightCyan,      /**< Light cyan color */
        BrightWhite,    /**< Bright white color */
        Max             /**< Maximum color value (for internal use) */
    };
#endif // CONSOLE_FEATURES_DISABLED

private:
#ifndef CONSOLE_FEATURES_DISABLED
    Color text_colors[(int)Log::Level::Max]; ///< Array of text colors for different log levels.
    Color current_color; ///< The current text color.
#endif // CONSOLE_FEATURES_DISABLED

public:
#ifndef CONSOLE_FEATURES_DISABLED
    /**
     * @brief Set the text color for a specific log level.
     * @param p_level The log level to set the text color for.
     * @param p_color The color to set.
     */
    void set_text_color(Log::Level p_level, Color p_color);

    /**
     * @brief Get the text color for a specific log level.
     * @param p_level The log level to get the text color for.
     * @return The color associated with the specified log level.
     */
    Color get_text_color(Log::Level p_level) const;

    /**
     * @brief Set the text color for all log levels.
     * @param p_color The color to set.
     */
    void set_text_color(Color p_color);

    /**
     * @brief Get the current text color.
     * @return The current text color.
     */
    Color get_text_color() const;
#endif // CONSOLE_FEATURES_DISABLED

    /**
     * @brief Print a string to the console without a newline.
     * @param p_string The string to print.
     */
    void print_string(const String& p_string);

    /**
     * @brief Print a string to the console with a newline.
     * @param p_string The string to print.
     */
    void print_line(const String& p_string);

    /**
     * @brief Print a string to the console with the specified log level's text color.
     * @param p_level The log level to use for text color.
     * @param p_string The string to print.
     */
    void print_string(Log::Level p_level, const String& p_string);

    /**
     * @brief Print a string to the console with the "Info" log level's text color.
     * @param p_string The string to print.
     */
    void print_info(const String& p_string);

    /**
     * @brief Print a string to the console with the "Warn" log level's text color.
     * @param p_string The string to print.
     */
    void print_warn(const String& p_string);

    /**
     * @brief Print a string to the console with the "Error" log level's text color.
     * @param p_string The string to print.
     */
    void print_error(const String& p_string);

    /**
     * @brief Log a string with the specified log level's text color.
     * @param p_level The log level to use for text color.
     * @param p_string The string to log.
     */
    void log_string(Log::Level p_level, const String& p_string);

    /**
     * @brief Log a string with the "Info" log level's text color.
     * @param p_string The string to log.
     */
    void log_info(const String& p_string);

    /**
     * @brief Log a string with the "Warn" log level's text color.
     * @param p_string The string to log.
     */
    void log_warn(const String& p_string);

    /**
     * @brief Log a string with the "Error" log level's text color.
     * @param p_string The string to log.
     */
    void log_error(const String& p_string);

    /**
     * @brief Constructor for the Console class.
     */
    Console();

    /**
     * @brief Destructor for the Console class.
     */
    virtual ~Console();
};
