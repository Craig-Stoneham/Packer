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
 * @file Defaults.h
 * @brief Defines default configuration values for the Packer Console.
 */

#pragma once

/**
 * @def DEFAULT_CONFIG_FILE_NAME
 * @brief The default configuration file name for Packer Console.
 *
 * This macro defines the default name of the configuration file used by Packer Console.
 * You can change this value to specify a different configuration file name if needed.
 */
#define DEFAULT_CONFIG_FILE_NAME "packer.cfg"

/**
 * @def DEFAULT_LOG_FILE_NAME
 * @brief The default log file name for Packer Console.
 *
 * This macro defines the default name of the log file used by Packer Console.
 * You can change this value to specify a different log file name if needed.
 */
#define DEFAULT_LOG_FILE_NAME "packer.log"

/**
 * @def DEFAULT_COMMAND_TEXT_COLOR
 * @brief The default text color for command text in Packer Console.
 *
 * This macro defines the default text color used for displaying command text in the console.
 * You can change this value to specify a different default text color for command text.
 */
#define DEFAULT_COMMAND_TEXT_COLOR Color::LightGreen
