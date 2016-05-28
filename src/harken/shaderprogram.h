#ifndef HARKEN_SHADERPROGRAM_H
#define HARKEN_SHADERPROGRAM_H

#include "global.h"
#include "shader.h"

#include <GL/glew.h>
#include <memory>
#include <vector>

namespace Harken {
    
    /**
     * Exception type thrown when OpenGL reports an error linking a shader program. Carries the
     * details of the errors produced during linking.
     */
    
    class ShaderLinkException : public Exception {
    public:
        
        /**
         * Constructs the ShaderLinkException to carry information about linking errors from a 
         * particular shader program.
         * @param infoLog The output of a call to <tt>glGetProgramInfoLog()</tt> for the shader
         *                program that has encountered a linking error. This pointer need not have
         *                a lifetime that extends beyond the construction of this exception.
         */
        
        explicit ShaderLinkException(const char* infoLog);
    };
 
    class ShaderProgram {
    public:
        
        ShaderProgram();
        ~ShaderProgram();
        
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        
        ShaderProgram(ShaderProgram&&) = default;
        ShaderProgram& operator=(ShaderProgram&&) = default;
        
        /**
         * Attaches a shader object to the program so that it will be linked together with other
         * shader objects to form a complete shader program when link() is called. Since shader
         * objects are move-only types, the shader specified here must be moved into the
         * ShaderProgram (which from this point forth takes ownership of it, and will delete it 
         * when it is no longer needed).
         */
        
        void attach(Shader&& shader);
        
        /**
         * Links shader objects previously passed into attach() into a complete shader program, and
         * performs error checking to ensure that the program was successfully linked (throwing a
         * ShaderLinkException if not). If the link was successful, this function then detaches and
         * deletes the program's shader objects (which are no longer needed). 
         */
        
        void link();
        
    private:
        
        std::vector<Shader> m_attachedShaders;
        GLuint m_id = 0;
    };
}

#endif
