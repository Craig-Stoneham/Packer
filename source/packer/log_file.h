// See LICENSE for full copyright and licensing information.

#pragma once

#include "log.h"
#include "error.h"

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