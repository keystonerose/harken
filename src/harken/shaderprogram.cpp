#include "shaderprogram.h"
#include "stringbuilder.h"

namespace Harken {
    
    ShaderLinkException::ShaderLinkException(const char* const infoLog)    
        : Exception{StringBuilder{} << "Error linking shader program. " << infoLog} {
    }
    
    ShaderProgram::ShaderProgram() {
        m_id = glCreateProgram();
    }
    
    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(m_id);
    }
    
    void ShaderProgram::attach(Shader&& shader) {
        glAttachShader(m_id, shader.m_id);
        m_attachedShaders.emplace_back(std::move(shader));
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
        // needed and can be detached from the program and destroyed.
        
        for (const auto& shader : m_attachedShaders) {
            glDetachShader(m_id, shader.m_id);
        }
        
        m_attachedShaders.clear();
    }
}