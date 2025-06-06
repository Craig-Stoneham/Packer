// See LICENSE for full copyright and licensing information.

#pragma once

#include "log.h"

PACKER_NAMESPACE_BEGIN

/**
 * @class Console
 * @brief Provides functionality for managing console output and text colors.
 * 
 * The `Console` class allows you to control the text colors used for different log levels
 * and provides methods for printing and logging messages to the console with various log levels.
 */
class Console {
public:
#ifdef CONSOLE_FEATURES_ENABLED
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
#endif // CONSOLE_FEATURES_ENABLED

private:
#ifdef CONSOLE_FEATURES_ENABLED
    Color text_colors[(int)Log::Level::Max]; ///< Array of text colors for different log levels.
    Color current_color; ///< The current text color.
#endif // CONSOLE_FEATURES_ENABLED

public:
#ifdef CONSOLE_FEATURES_ENABLED
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
#endif // CONSOLE_FEATURES_ENABLED

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

PACKER_NAMESPACE_END