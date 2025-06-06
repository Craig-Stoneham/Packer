// See LICENSE for full copyright and licensing information.

#pragma once

#include "typedefs.h"

PACKER_NAMESPACE_BEGIN

/**
 * @enum Error
 * @brief Enumerates error codes.
 */
enum class Error {
    Unknown = -1,        ///< Unknown error.
    OK,                  ///< No error; operation successful.
    Failed,              ///< General operation failure.
    Unconfigured,        ///< Configuration not set.
    RangeError,          ///< Value out of valid range.
    FileNotFound,        ///< File not found.
    FileBadPath,         ///< Invalid file path.
    FileAlreadyInUse,    ///< File is already in use.
    FileCantOpen,        ///< Unable to open file.
    InvalidData,         ///< Invalid data format.
    DoesNotExist,        ///< The requested item does not exist.
    Max                  ///< Maximum value for error codes.
};

/**
 * @brief Get a string representation of an error code.
 * @param p_error The error code.
 * @return A string describing the error.
 */
String get_error_name(Error p_error);

/**
 * @brief Find an error code by its string representation.
 * @param p_error The string representation of the error.
 * @return The corresponding error code, or Error::Unknown if not found.
 */
Error find_error(const String& p_error);

PACKER_NAMESPACE_END