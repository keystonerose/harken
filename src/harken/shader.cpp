#include "exception.h"
#include "shader.h"
#include "stringbuilder.h"

#include <cassert>
#include <fstream>
#include <memory>
#include <streambuf>
#include <string>

namespace Harken {

    namespace {

        /**
         * Reads the contents of a file on disk into a string. Throws an IOException of the file at
         * @p filePath could not be opened for whatever reason.
         * @param filePath An absolute filesystem path to the file to read.
         */

        std::string readFile(const char* const filePath) {

            std::ifstream inputFile{filePath};
            if (!inputFile) {
                throw IOException{StringBuilder{} << "Could not open file \"" << filePath << "\"."};
            }

            return std::string{std::istreambuf_iterator<char>{inputFile},
                               std::istreambuf_iterator<char>{}};
        }

        /**
         * Returns a string naming the type of shader referred to by the OpenGL enumerated code
         * @p type. @p type must be one of: @c GL_VERTEX_SHADER, @c GL_TESS_CONTROL_SHADER,
         * @c GL_TESS_EVALUATION_SHADER, @c GL_GEOMETRY_SHADER or @c GL_FRAGMENT_SHADER.
         */

        const char* shaderTypeName(const GLenum type) {

            switch (type) {
            case GL_FRAGMENT_SHADER:
                return "fragment";
            case GL_VERTEX_SHADER:
                return "vertex";
            case GL_TESS_EVALUATION_SHADER:
                return "tessellation evaluation";
            case GL_TESS_CONTROL_SHADER:
                return "tessellation control";
            case GL_GEOMETRY_SHADER:
                return "geometry";
            default:
                return "";
            }
        }
    }

    ShaderCompilationException::ShaderCompilationException(const GLenum type,
                                                           const char* const sourceFilePath,
                                                           const char* const infoLog)

        : Exception{StringBuilder{} << "Error compiling " << shaderTypeName(type)
                                    << " shader from \"" << sourceFilePath << "\". " << infoLog} {
    }

    Shader::Shader(const GLenum type, const char* const sourceFilePath)
        : GLHandle<Shader>{type} {

        const auto source = readFile(sourceFilePath);
        const char* const glSource[] = {source.c_str()};

        glShaderSource(m_id, sizeof(glSource) / sizeof(char *), glSource, nullptr);
        glCompileShader(m_id);

        GLint success;
        glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
        if (!success) {

            GLint logLength;
            glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &logLength);
            const auto infoLog = std::unique_ptr<char[]>{new char[logLength]};

            glGetShaderInfoLog(m_id, logLength, nullptr, infoLog.get());
            throw ShaderCompilationException{type, sourceFilePath, infoLog.get()};
        }
    }
    
    void Shader::create(const GLenum type) {
        m_id = glCreateShader(type);
    }

    void Shader::destroy() {
        glDeleteShader(m_id);
    }
}
