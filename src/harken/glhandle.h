#ifndef HARKEN_GLHANDLE_H
#define HARKEN_GLHANDLE_H

#include <GL/glew.h>

namespace Harken {
    
    /**
     * Provides a handle to an object managed by OpenGL. This provides generalised RAII behaviour as
     * a CRTP base class for classes handling specific types of OpenGL entity: these inheriting
     * classes must implement <tt>create()</tt> and <tt>destroy()</tt> member functions that specify
     * how OpenGL should be instructed to generate and delete the object being managed.
     * 
     * No reference counting is implemented, so the handle cannot be copied; it can, however, be 
     * moved (in which case the moved-from handle enters a null state and performs no action when
     * deleted).
     */
    
    template<typename Resource>
    class GLHandle {
    public:
        
        /**
         * Calls the derived class's implementation of Resource::create(), which should make the 
         * necessary OpenGL call to create an object of the desired type (which will be managed by
         * the GLHandle for the remainder of its lifetime). Resource::create() should set the
         * @c m_id member to the handle ID assigned by OpenGL to this object, and receives the same
         * parameters passed (by value) to the GLHandle parent class constructor.
         */
        
        template<typename... Args>
        GLHandle(Args... args) {
            static_cast<Resource *>(this)->create(args...);
        }
        
        /**
         * Calls the derived class's implementation of Resource::destroy(), which should make the 
         * necessary OpenGL call to destroy the object managed by the GLHandle.
         */
        
        ~GLHandle() {
            static_cast<Resource *>(this)->destroy();
        }
        
        GLHandle(const GLHandle<Resource>&) = delete;
        GLHandle<Resource>& operator=(const GLHandle<Resource>&) = delete;
        
        /**
         * Constructs a new GLHandle instance that replaces @p rhs as a handle to the underlying
         * OpenGL object. After the new handle is constructed, @p rhs no longer refers to a valid
         * object, and will not cause OpenGL to destroy the previously referenced object when its
         * lifetime ends.
         */
        
        GLHandle(GLHandle<Resource>&& rhs)
            : m_id{rhs.m_id} {

            rhs.m_id = 0;
        }
        
        /**
         * Assigns the calling GLHandle instance to replace @p rhs as a handle to the underlying
         * OpenGL object. After this assignment is performed, @p rhs no longer refers to a valid
         * object, and will not cause OpenGL to destroy the previously referenced object when its
         * lifetime ends.
         */
        
        GLHandle<Resource>& operator=(GLHandle<Resource>&& rhs) {

            m_id = rhs.m_id;
            rhs.m_id = 0;

            return *this;
        }
        
    protected:
        
        GLuint m_id = 0;
    };
}

#endif
