#ifndef HARKEN_SHADERPROGRAM_H
#define HARKEN_SHADERPROGRAM_H

#include "global.h"
#include "shader.h"

#include <GL/glew.h>
#include <initializer_list>
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

    /**
     * Provides a handle to an OpenGL shader program object, and associated functionality. This is
     * an RAII class that makes the necessary OpenGL calls to create a shader program on
     * construction and to delete that same shader program on destruction. No reference counting is
     * implemented, so the ShaderProgram can therefore not be copied; it can, however, be moved (in
     * which case the moved-from ShaderProgram becomes a null handle and performs no action when
     * deleted).
     *
     * The general setup for a ShaderProgram is to create a number of Shader instances, then pass
     * each of these to ShaderProgram::attach(), then call ShaderProgram::link() to link the
     * program. Alternatively, the initialiser-list constructor may be used to combine these steps.
     */

    class ShaderProgram {
    public:

        /**
         * Instructs OpenGL to create a shader program and initialises this ShaderProgram object as
         * a handle to it.
         */

        ShaderProgram();

        /**
         * Instructs OpenGL to create a shader program and initialises this ShaderProgram object as
         * a handle to it, then attaches a collection of shader objects to the program and links it.
         * Equivalent to default-constructing the ShaderProgram, then calling attach() for each item
         * in @p shaders, then calling link().
         */

        explicit ShaderProgram(std::initializer_list<std::shared_ptr<Shader>> shaders);

        /**
         * Instructs OpenGL to delete the shader program handled by this ShaderProgram object.
         */

        ~ShaderProgram();

        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        /**
         * Constructs a new ShaderProgram instance that replaces @p rhs as a handle to the
         * underlying OpenGL shader program. After the new ShaderProgram is constructed, @p rhs is
         * no longer a handle to a valid shader program, and will not cause OpenGL to destroy it
         * when its lifetime ends.
         */

        ShaderProgram(ShaderProgram&& rhs);

        /**
         * @see ShaderProgram(ShaderProgram&&)
         */

        ShaderProgram& operator=(ShaderProgram&& rhs);

        /**
         * Attaches a shader object to the program so that it will be linked together with other
         * shader objects to form a complete shader program when link() is called. The ShaderProgram
         * will take partial ownership of this shader until link() is called, at which point it will
         * detach the shader from itself and relinquish ownership of it (causing it to be deleted
         * if no other owner is present).
         */

        void attach(std::shared_ptr<Shader> shader);

        /**
         * Links shader objects previously passed into attach() into a complete shader program, and
         * performs error checking to ensure that the program was successfully linked (throwing a
         * ShaderLinkException if not). If the link was successful, this function then detaches and
         * deletes the program's shader objects (which are no longer needed).
         */

        void link();

        /**
         * Engages the shader program so that it becomes active in the OpenGL rendering pipeline.
         */

        void use();

    private:

        std::vector<std::shared_ptr<Shader>> m_attachedShaders;
        GLuint m_id = 0;
    };
}

#endif
