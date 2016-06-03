#ifndef HARKEN_VERTEXARRAYOBJECT_H
#define HARKEN_VERTEXARRAYOBJECT_H

#include "global.h"
#include "glhandle.h"

#include <GL/glew.h>

namespace Harken {
    
    class VertexArrayObject : public GLHandle<VertexArrayObject> {
        friend class GLHandle<VertexArrayObject>;
        
    public:
        
        VertexArrayObject();
        
        ~VertexArrayObject();
        
    private:
        
        GLuint m_id = 0;
    };
}

#endif
