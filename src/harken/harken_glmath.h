#ifndef HARKEN_GLMATH_H
#define HARKEN_GLMATH_H

#include "harken_global.h"
#include "harken_matrix.h"
#include "harken_vector.h"

#include <GL/glew.h>

namespace Harken {
    
    using Vector3f = Vector3<GLfloat>;
    using Vector4f = Vector4<GLfloat>;
    using Matrix4f = Matrix4<GLfloat>;
    
    /**
     * Generates a 4x4 transformation matrix that can be used to translate a 3D vector represented
     * as homogeneous coordinates (that is, padded to a @c Vector4). @p offsetX, @p offsetY and
     * @p offsetZ specify the amounts to translate by in the x, y, and z directions.
     */
    
    Matrix4f translationMatrix(GLfloat offsetX, GLfloat offsetY, GLfloat offsetZ);
    
    /**
     * @see translationMatrix(GLfloat, GLfloat, GLfloat)
     */
    
    Matrix4f translationMatrix(const Vector3f& offset);
}

#endif
