#include "harken_vertexbufferobject.h"

namespace Harken {

    VertexBufferObject::VertexBufferObject(const GLenum type)
        : m_type{type} {
    }

    void VertexBufferObject::bind() {
        glBindBuffer(m_type, m_id);
    }

    void VertexBufferObject::create() {
        glGenBuffers(1, &m_id);
    }

    void VertexBufferObject::destroy() {
        glDeleteBuffers(1, &m_id);
    }

    void VertexBufferObject::unbind(const GLenum type) {
        glBindBuffer(type, 0);
    }
}
