#ifndef HARKEN_VERTEXBUFFEROBJECT_H
#define HARKEN_VERTEXBUFFEROBJECT_H

#include "global.h"
#include "glhandle.h"
#include "vector.h"

#include <initializer_list>
#include <vector>

namespace Harken {
    
    class VertexBufferObject : public GLHandle<VertexBufferObject> {
        friend class GLHandle<VertexBufferObject>;
        
    public:
        
        /**
         * Creates a new OpenGL vertex buffer object of the specified @p type.
         */

        VertexBufferObject(GLenum type);
        
        /**
         * When called the first time, sets up a new vertex buffer object in OpenGL; when called
         * subsequently, makes that vertex buffer object active.
         */
        
        void bind();
        
        /**
         * Instructs OpenGL to stop using vertex buffer objects of the specified type.
         */
        
        static void unbind(GLenum type);
        
    private:
        
        /**
         * Instructs OpenGL to create a single vertex buffer object and initialises this
         * VertexBufferObject as a handle to it. Called by the GLHandle base class.
         */
        
        void create();
        
        /**
         * Instructs OpenGL to delete the vertex buffer object managed by this VertexBufferObject
         * object. Called by the GLHandle base class.
         */
        
        void destroy();
        
        const GLenum m_type;
    };
}

#endif
