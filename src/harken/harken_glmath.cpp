#include "harken_glmath.h"

namespace Harken {
    
    Matrix4f translationMatrix(const Vector3f& offset) {
        return translationMatrix(offset.x(), offset.y(), offset.z());
    }
    
    Matrix4f translationMatrix(GLfloat offsetX, GLfloat offsetY, GLfloat offsetZ) {
        
        Matrix4f transformation;
        
        transformation(0, 3) = offsetX;
        transformation(1, 3) = offsetY;
        transformation(2, 3) = offsetZ;
        
        return transformation;
    }
}
