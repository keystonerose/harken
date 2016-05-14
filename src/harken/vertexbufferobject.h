#ifndef HARKEN_VERTEXBUFFEROBJECT_H
#define HARKEN_VERTEXBUFFEROBJECT_H

#include "vector.h"

#include <initializer_list>
#include <vector>

namespace Harken {
    
    class VertexBufferObject {
    public:
        
        VertexBufferObject(std::initializer_list<Vector3> vertices);
        
    private:
        std::vector<GLfloat> m_data;
    };
}

#endif
