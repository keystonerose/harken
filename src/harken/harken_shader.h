#ifndef HARKEN_SHADER_H
#define HARKEN_SHADER_H

#include "harken_global.h"
#include "harken_exception.h"
#include "harken_glhandle.h"

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

    /**
     * Provides a handle to an OpenGL shader object, and associated functionality. This is an RAII
     * class that makes the necessary OpenGL calls to create a shader object on construction and to
     * delete that same shader object on destruction. No reference counting is implemented, so the
     * Shader cannot be copied; it can, however, be moved (in which case the moved-from Shader
     * becomes a null handle and performs no action when deleted). Once constructed, Shaders should
     * be linked into a ShaderProgram.
     */

    class Shader : public GLHandle<Shader> {
        friend class GLHandle<Shader>;
        friend class ShaderProgram;

    public:

        /**
         * Creates a new OpenGL shader object of the specified @p type, loads it source from the
         * file at @p sourcefilePath, and compiles it. The resulting compiled shader object can then
         * be linked into a ShaderProgram.
         */

        Shader(GLenum type, const char * sourceFilePath);

    private:

        /**
         * Instructs OpenGL to create a shader object and initialises this Shader as a handle to it.
         * Called by the GLHandle base class.
         */

        void create(GLenum type);

        /**
         * Instructs OpenGL to delete the shader object managed by this Shader object. Called by the
         * GLHandle base class.
         */

        void destroy();
    };
}

#endif
