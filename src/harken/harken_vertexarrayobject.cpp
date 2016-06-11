#include "harken_vertexarrayobject.h"

namespace Harken {

    void VertexArrayObject::bind() {
        glBindVertexArray(m_id);
    }

    void VertexArrayObject::create() {
        glGenVertexArrays(1, &m_id);
    }

    void VertexArrayObject::destroy() {
        glDeleteVertexArrays(1, &m_id);
    }

    void VertexArrayObject::unbind() {
        glBindVertexArray(0);
    }
}
