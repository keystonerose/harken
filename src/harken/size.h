#ifndef HARKEN_SIZE_H
#define HARKEN_SIZE_H

#include "global.h"

namespace Harken {
 
    template<typename T>
    class Size2 {
    public:
        
        Size2()
            : m_width{0}, m_height{0} {
        }
        
        Size2(const T width, const T height)
            : m_width{width}, m_height{height} {
        }
        
        T height() const {
            return m_height;
        }
        
        T width() const {
            return m_width;
        }
        
    private:
        
        T m_width;
        T m_height;
    };
}

#endif
