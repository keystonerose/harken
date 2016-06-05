#ifndef HARKEN_VERTEXARRAYOBJECT_H
#define HARKEN_VERTEXARRAYOBJECT_H

#include "global.h"
#include "glhandle.h"

#include <GL/glew.h>

namespace Harken {
    
    /**
     * RAII class that provides a handle to an OpenGL vertex array object, which is used to keep
     * track of how vertex data should be specified for the OpenGL pipeline.
     */
    
    class VertexArrayObject : public GLHandle<VertexArrayObject> {
        friend class GLHandle<VertexArrayObject>;
        
    public:
        
        /**
         * When called the first time, sets up a new vertex array object in OpenGL so that vertex
         * buffer data may be associated with it; when called subsequently, makes that vertex array
         * object active.
         */
        
        void bind();
        
        /**
         * Instructs OpenGL to stop using any user-created vertex array objects and to return to the
         * default state for vertex arrays.
         */
        
        static void unbind();
        
    private:
        
        /**
         * Instructs OpenGL to create a single vertex array object and initialises this
         * VertexArrayObject as a handle to it. Called by the GLHandle base class.
         */
        
        void create();
        
        /**
         * Instructs OpenGL to delete the vertex array object managed by this VertexArrayObject
         * object. Called by the GLHandle base class.
         */
        
        void destroy();
    };
}

#endif
