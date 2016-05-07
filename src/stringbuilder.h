#ifndef HARKEN_APP_STRINGBUILDER_H
#define HARKEN_APP_STRINGBUILDER_H

#include <sstream>
#include <string>

namespace Util {

    /**
     * Facilitates the formation of temporary in-place-formatted strings to be passed as arguments to various other
     * functions; for example, when constructing an exception with a custom message.
     */

    class StringBuilder {
        public:
            
            template<typename T>
            StringBuilder& operator<<(const T& item) {
                m_buffer << item;
                return *this;
            }
            
            operator std::string() const {
                return m_buffer.str();
            }
            
        private:
            
            std::ostringstream m_buffer;
    };
}

#endif
