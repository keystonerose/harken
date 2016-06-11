#ifndef HARKEN_EXCEPTION_H
#define HARKEN_EXCEPTION_H

#include "harken_global.h"

#include <stdexcept>
#include <string>

namespace Harken {

    /**
     * A wrapper around @c std::runtime_error serving as a common base class for all exceptions
     * thrown by Harken. Usage and construction is the same as for @c std::runtime_error.
     */

    class Exception : public std::runtime_error {
    public:
        explicit Exception(const char * message);
        explicit Exception(const std::string& message);
    };

    /**
     * Generic exception class thrown when an input/output error occurs. May be derived from to
     * indicate more specific classes of I/O errors.
     */

    class IOException : public Exception {
    public:
        explicit IOException(const char * message);
        explicit IOException(const std::string& message);
    };
}

#endif
