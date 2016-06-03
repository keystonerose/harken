#include "shaderprogram.h"
#include "stringbuilder.h"

#include <utility>

namespace Harken {

    ShaderLinkException::ShaderLinkException(const char* const infoLog)
        : Exception{StringBuilder{} << "Error linking shader program. " << infoLog} {
    }

    ShaderProgram::ShaderProgram(std::initializer_list<std::shared_ptr<Shader>> shaders) {

        for (const auto& shader : shaders) {
            attach(std::move(shader));
        }

        link();
    }

    void ShaderProgram::attach(std::shared_ptr<Shader> shader) {

        if (!shader) {
            return;
        }

        glAttachShader(m_id, shader->m_id);
        m_attachedShaders.push_back(std::move(shader));
    }
    
    void ShaderProgram::create() {
        m_id = glCreateProgram();
    }
    
    void ShaderProgram::destroy() {
        glDeleteProgram(m_id);
    }

    void ShaderProgram::link() {

        glLinkProgram(m_id);

        GLint success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success) {

            GLint logLength;
            glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &logLength);
            const auto infoLog = std::unique_ptr<char[]>{new char[logLength]};

            glGetProgramInfoLog(m_id, logLength, nullptr, infoLog.get());
            throw ShaderLinkException{infoLog.get()};
        }

        // Once the shader program has been linked, the intermediate shader objects are no longer
        // needed and can be detached from the program (and automatically destroyed if they are no
        // longer referenced by the calling code)

        for (const auto& shader : m_attachedShaders) {
            glDetachShader(m_id, shader->m_id);
        }

        m_attachedShaders.clear();
    }

    void ShaderProgram::use() {
        glUseProgram(m_id);
    }
}
