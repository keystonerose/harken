#ifndef HARKEN_MATRIX_H
#define HARKEN_MATRIX_H

#include "vector.h"

#include <array>
#include <type_traits>

namespace Harken {

    /**
     * A mathematical matrix of arbitrary dimensions. For compatibility with OpenGL, data are stored
     * in column-major order; the interface of the Matrix, however, presents data in a row-major 
     * order (as is the standard for multidimensional arrays in C-like languages).
     */

    template<typename T, int RowCount, int ColCount>
    class Matrix {
    public:
        
        static_assert(std::is_arithmetic<T>::value, "The component type of a Matrix must be arithmetic.");

        Matrix() {
        }

        VectorSpan<T, ColCount> operator[](const int i) {
            return VectorSpan<T, ColCount>{m_data, i, RowCount};
        }
        
        const VectorSpan<T, ColCount> operator[](const int i) const {
            return VectorSpan<T, ColCount>{m_data, i, RowCount};
        }

    private:

        std::array<T, ColCount * RowCount> m_data{};
    };
}

#endif
