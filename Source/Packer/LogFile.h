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
 * @file LogFile.h
 * @brief Defines the LogFile class for logging to a file.
 */

#pragma once

#include "Log.h"
#include "Error.h"

PACKER_NAMESPACE_BEGIN

/**
 * @class LogFile
 * @brief Provides functionality for logging to a file.
 */
class LogFile {
    FileStreamO stream; ///< The output file stream for logging.
    bool print_level; ///< Indicates whether to print log level details.

public:
    /**
     * @brief Log a string message with a specified log level to the file.
     * @param p_level The log level.
     * @param p_string The log message.
     */
    void log_string(Log::Level p_level, const String& p_string);

    /**
     * @brief Log an informational message to the file.
     * @param p_string The log message.
     */
    void log_info(const String& p_string);

    /**
     * @brief Log a warning message to the file.
     * @param p_string The log message.
     */
    void log_warn(const String& p_string);

    /**
     * @brief Log an error message to the file.
     * @param p_string The log message.
     */
    void log_error(const String& p_string);

    /**
     * @brief Set whether to print log level details.
     * @param p_enabled If true, log level details will be included in log messages.
     */
    void set_print_level(bool p_enabled);

    /**
     * @brief Get the current setting for printing log level details.
     * @return true if log level details are included in log messages, false otherwise.
     */
    bool get_print_level() const;

    /**
     * @brief Open the log file at the specified path.
     * @param p_path The path to the log file.
     * @return An Error code indicating the result of the operation.
     */
    Error open(const String& p_path);

    /**
     * @brief Close the log file.
     * @return An Error code indicating the result of the operation.
     */
    Error close();

    /**
     * @brief Default constructor for the LogFile class.
     * @param p_print_level If true, log level details will be included in log messages.
     */
    LogFile(bool p_print_level = false);

    /**
     * @brief Constructor for the LogFile class that opens a log file at the specified path.
     * @param p_path The path to the log file.
     * @param p_print_level If true, log level details will be included in log messages.
     */
    LogFile(const String& p_path, bool p_print_level = false);

    /**
     * @brief Destructor for the LogFile class.
     */
    virtual ~LogFile();
};

PACKER_NAMESPACE_END