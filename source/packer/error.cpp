// See LICENSE for full copyright and licensing information.

#include "error.h"

PACKER_NAMESPACE_BEGIN

static const char* error_names[] = {
    "ok",
    "failed",
    "unconfigured",
    "range error",
    "file not found",
    "file bad path",
    "file already in use",
    "file cant open",
    "invalid data",
    "does not exist",
};

String get_error_name(Error p_error) {
    if (p_error >= static_cast<Error>(0) && p_error < Error::Max) {
        return error_names[static_cast<size_t>(p_error)];
    }  else {
        return "unknown";
    }
}

Error find_error(const String& p_error) {
    for (int i = 0; i < static_cast<size_t>(Error::Max); ++i) {
        if (p_error == error_names[i]) {
            return static_cast<Error>(i);
        }
    }
    return Error::Unknown;
}

PACKER_NAMESPACE_END
