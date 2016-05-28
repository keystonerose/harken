#ifndef HARKEN_SHADER_H
#define HARKEN_SHADER_H

#include "exception.h"
#include "global.h"

#include <GL/glew.h>

namespace Harken {
    
    /**
     * Exception type thrown when OpenGL reports an error compiling a shader module. Carries
     * information identifying which shader source file caused the compilation errors and the
     * details of the compilation errors themselves.
     */
    
    class ShaderCompilationException : public Exception {
    public:
        
        /**
         * Constructs the ShaderCompilationException to carry information about compilation errors 
         * in a specified shader source file. Neither of the string parameters (@p sourceFilePath
         * and @p infoLog) need have lifetimes that extend beyond the construction of this exception
         * object.
         * @param type           The OpenGL enumerated code indicating the type of shader that
         *                       raised the compilation error.
         * @param sourceFilePath The filesystem path to the file on disk containing the shader 
         *                       source code.
         * @param infoLog        The output of a call to <tt>glGetShaderInfoLog()</tt> for the
         *                       shader that has encountered a compilation error. 
         */
        
        explicit ShaderCompilationException(GLenum type,
                                            const char* sourceFilePath,
                                            const char* infoLog);
    };
    
    class Shader {
        friend class ShaderProgram;
        
    public:
        
        Shader(GLenum type, const char* sourceFilePath);
        ~Shader();
        
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        
        Shader(Shader&&) = default;
        Shader& operator=(Shader&&) = default;
    
    private:
        
        GLuint m_id = 0;
    };
}

#endif
