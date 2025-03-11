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

#pragma once

#include "typedefs.h"

PACKER_NAMESPACE_BEGIN

/**
 * @class Log
 * @brief Provides logging functionality for the application.
 */
class Log {
public:
    /**
     * @enum Level
     * @brief Enumerates log levels.
     */
    enum class Level {
        Unknown = -1, ///< Unknown log level.
        Info,         ///< Informational log level.
        Warn,         ///< Warning log level.
        Error,        ///< Error log level.
        Max           ///< Maximum value for log levels.
    };

    /**
     * @brief Callback function type for custom log handling.
     * @param p_data User-defined data to pass to the callback.
     * @param p_level The log level.
     * @param p_string The log message.
     */
    using Callback = void (*)(void* p_data, Level p_level, const String& p_string);

    /**
     * @brief Get a string representation of a log level.
     * @param p_level The log level.
     * @return A string describing the log level.
     */
    static String get_level_name(Level p_level);

    /**
     * @brief Find a log level by its string representation.
     * @param p_level The string representation of the log level.
     * @return The corresponding log level, or Level::Unknown if not found.
     */
    static Level find_level(const String& p_level);

    /**
     * @brief Add a callback function for custom log handling.
     * @param p_callback The callback function.
     * @param p_data User-defined data to pass to the callback.
     */
    static void add_callback(Callback p_callback, void* p_data);

    /**
     * @brief Remove a callback function for custom log handling.
     * @param p_callback The callback function to remove.
     * @param p_data User-defined data associated with the callback.
     */
    static void remove_callback(Callback p_callback, void* p_data);

    /**
     * @brief Log a string message with a specified log level.
     * @param p_level The log level.
     * @param p_string The log message.
     */
    static void log_string(Level p_level, const String& p_string);

    /**
     * @brief Log an informational message.
     * @param p_string The log message.
     */
    static void log_info(const String& p_string);

    /**
     * @brief Log a warning message.
     * @param p_string The log message.
     */
    static void log_warn(const String& p_string);

    /**
     * @brief Log an error message.
     * @param p_string The log message.
     */
    static void log_error(const String& p_string);
};

/**
 * @def LOG_STRING(p_level, p_string)
 * @brief Log a string message with a specified log level.
 * @param p_level The log level.
 * @param p_string The log message.
 */
#ifdef LOG_ENABLED
#define LOG_STRING(p_level, p_string) Log::log_string(p_level, p_string)
#else // LOG_ENABLED
#define LOG_STRING(p_level, p_string)
#endif // LOG_ENABLED

/**
* @def LOG_INFO(p_string)
* @brief Log an informational message.
* @param p_string The log message.
*/
#ifdef LOG_ENABLED
#define LOG_INFO(p_string) Log::log_info(p_string)
#else // LOG_ENABLED
#define LOG_INFO(p_string)
#endif // LOG_ENABLED

/**
* @def LOG_WARN(p_string)
* @brief Log a warning message.
* @param p_string The log message.
*/
#ifdef LOG_ENABLED
#define LOG_WARN(p_string) Log::log_warn(p_string)
#else // LOG_ENABLED
#define LOG_WARN(p_string)
#endif // LOG_ENABLED

/**
* @def LOG_ERROR(p_string)
* @brief Log an error message.
* @param p_string The log message.
*/
#ifdef LOG_ENABLED
#define LOG_ERROR(p_string) Log::log_error(p_string)
#else // LOG_ENABLED
#define LOG_ERROR(p_string)
#endif // LOG_ENABLED

PACKER_NAMESPACE_END