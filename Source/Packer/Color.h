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
 * @file Color.h
 * @brief Defines the Color enumeration.
 */

#pragma once

#include "Typedefs.h"

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
