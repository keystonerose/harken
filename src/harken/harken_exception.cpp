#include "harken_exception.h"

namespace Harken {

    Exception::Exception(const char * const message)
        : runtime_error{message} {
    }

    Exception::Exception(const std::string& message)
        : runtime_error{message} {
    }

    IOException::IOException(const char * const message)
        : Exception{message} {
    }

    IOException::IOException(const std::string& message)
        : Exception{message} {
    }
}
