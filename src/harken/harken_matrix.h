#ifndef HARKEN_MATRIX_H
#define HARKEN_MATRIX_H

#include "harken_global.h"
#include "harken_vector.h"

#include <algorithm>
#include <array>

namespace Harken {

    /**
     * A mathematical matrix of arbitrary dimensions. For compatibility with OpenGL, data are stored
     * in column-major order; the interface of the Matrix, however, presents data in a row-major
     * order (as is the standard for multidimensional arrays in C-like languages). Elements of the
     * matrix are accessed via <tt>operator()</tt>.
     */

    template<typename T, int RowCount, int ColCount>
    class Matrix {
    private:

        static_assert(std::is_arithmetic<T>::value, "The component type of a Matrix must be arithmetic.");

        static constexpr auto DiagonalSize = std::min(RowCount, ColCount);
        static constexpr auto Size = RowCount * ColCount;

    public:

        using ComponentType = T;

        /**
         * Fills the matrix with zeroes and sets its diagonal components to @c 1, so that the square
         * portion of the matrix is the identity.
         */

        Matrix() {

            for (auto i = 0; i < DiagonalSize; ++i) {
                (*this)(i, i) = 1;
            }
        }

        /**
         * Constructs the Matrix and initialises its data from an argument list which specifies
         * either
         * - the complete contents of the matrix, in row-major order, or
         * - the diagonal elements of the matrix.
         * @p args should be a list of exactly <tt>ColCount * RowCount</tt> (in the first case) or
         * <tt>min(ColCount, RowCount)</tt> (in the second case) arguments, each convertible to the
         * Matrix component type @c T.
         */

        template<typename... Args>
        Matrix(Args... args) {

            constexpr auto ArgCount = sizeof...(Args);
            static_assert(ArgCount == Size || ArgCount == DiagonalSize,
                 "The number of arguments provided to the Matrix constructor must match its size "
                 "or its number of diagonal elements.");

            const T elements[] = {static_cast<T>(args)...};

            if (ArgCount == Size) {
                for (auto i = 0; i < RowCount; ++i) {
                    for (auto j = 0; j < ColCount; ++j) {
                        (*this)(i, j) = elements[i * ColCount + j];
                    }
                }
            }
            else if (ArgCount == DiagonalSize) {
                for (auto i = 0; i < DiagonalSize; ++i) {
                    (*this)(i, i) = elements[i];
                }
            }
        }

        T& operator()(const int row, const int col) {
            return m_data[col * RowCount + row];
        }

        T operator()(const int row, const int col) const {
            return m_data[col * RowCount + row];
        }

        /**
         * Gets a vector spanning a single column of the matrix. This vector refers directly to the
         * data contained within the matrix; mutating it will change the relevant column of the
         * matrix itself.
         */

        VectorSpan<T, RowCount> columnSpan(const int index) {
            return VectorSpan<T, RowCount>{m_data, index * RowCount};
        }

        /**
         * Gets a vector corresponding to a single column of the matrix. This vector is merely a
         * copy of the column of the matrix and cannot be used to mutate it.
         */

        const Vector<T, RowCount> column(const int index) const {

            Vector<T, RowCount> result;
            for (auto i = 0; i < RowCount; ++i) {
                result[i] = (*this)(i, index);
            }

            return result;
        }

        /**
         * Gets a vector spanning a single row of the matrix. This vector refers directly to the
         * data contained within the matrix; mutating it will change the relevant row of the matrix
         * itself.
         */

        VectorSpan<T, ColCount> rowSpan(const int index) {
            return VectorSpan<T, ColCount>{m_data, index, RowCount};
        }

        /**
         * Gets a vector corresponding to a single row of the matrix. This vector is merely a copy
         * of the row of the matrix and cannot be used to mutate it.
         */

        const Vector<T, ColCount> row(const int index) const {

            Vector<T, ColCount> result;
            for (auto i = 0; i < ColCount; ++i) {
                result[i] = (*this)(index, i);
            }

            return result;
        }

    private:

        std::array<T, Size> m_data{};
    };

    template<typename T, int RowCount, int ColCount>
    bool operator==(const Matrix<T, RowCount, ColCount>& lhs,
                    const Matrix<T, RowCount, ColCount>& rhs) {

        for (auto i = 0; i < RowCount; ++i) {
            for (auto j = 0; j < ColCount; ++j) {
                if (lhs(i, j) != rhs(i, j)) {
                    return false;
                }
            }
        }

        return true;
    }

    template<typename T, int RowCount, int ColCount>
    bool operator!=(const Matrix<T, RowCount, ColCount>& lhs,
                    const Matrix<T, RowCount, ColCount>& rhs) {

        return !(lhs == rhs);
    }

    /**
     * Prints @p matrix to @p os, using the current formatting manipulators on @p os. The printed
     * representation of the matrix is formatted in a single line.
     */

    template<typename T, int RowCount, int ColCount>
    std::ostream& operator<<(std::ostream& os, const Matrix<T, RowCount, ColCount>& matrix) {

        os << '[';

        for (auto i = 0; i < RowCount; ++i) {

            if (i > 0) {
                os << "; ";
            }

            for (auto j = 0; j < ColCount; ++j) {
                if (j > 0) {
                    os << ", ";
                }
                os << matrix(i, j);
            }
        }

        os << ']';
        return os;
    }
}

#endif
