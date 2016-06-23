#include "harken_matrix.h"
#include "harken_vector.h"

#include <boost/test/unit_test.hpp>

#include <array>
#include <tuple>
#include <vector>

using Harken::Matrix;
using Harken::Matrix4;
using Harken::Vector;

template<typename T> using Matrix2 = Harken::Matrix<T, 2, 2>;
using Matrix2i = Matrix2<int>;

using Matrix4i = Matrix4<int>;

template<typename T> using Matrix3x4 = Matrix<T, 3, 4>;
using Matrix3x4i = Matrix3x4<int>;

template<typename T> using Matrix4x3 = Matrix<T, 4, 3>;
using Matrix4x3i = Matrix4x3<int>;

template<typename T> using Vector4 = Vector<T, 4>;
using Vector4i = Vector4<int>;

BOOST_AUTO_TEST_SUITE(matrix)

BOOST_AUTO_TEST_CASE(construction_accessors) {

    const Matrix4i identity_1;
    const Matrix4i identity_2{1, 1, 1, 1};
    const Matrix4i identity_3{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    BOOST_CHECK_EQUAL(identity_1, identity_3);
    BOOST_CHECK_EQUAL(identity_2, identity_3);

    const Matrix4i diagonal4x4_1{1, 2, 3, 4};
    const Matrix4i diagonal4x4_2{
        1, 0, 0, 0,
        0, 2, 0, 0,
        0, 0, 3, 0,
        0, 0, 0, 4
    };

    BOOST_CHECK_EQUAL(diagonal4x4_1, diagonal4x4_2);

    const Matrix3x4i diagonal3x4_1{1, 2, 3};
    const Matrix3x4i diagonal3x4_2{
        1, 0, 0, 0,
        0, 2, 0, 0,
        0, 0, 3, 0
    };

    BOOST_CHECK_EQUAL(diagonal3x4_1, diagonal3x4_2);

    const Matrix4x3i diagonal4x3_1{1, 2, 3};
    const Matrix4x3i diagonal4x3_2{
        1, 0, 0,
        0, 2, 0,
        0, 0, 3,
        0, 0, 0
    };

    BOOST_CHECK_EQUAL(diagonal4x3_1, diagonal4x3_2);

    const Matrix4i constPopulated{
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    };

    BOOST_CHECK_EQUAL(constPopulated.row(0), Vector4i(1, 2, 3, 4));
    BOOST_CHECK_EQUAL(constPopulated.row(1), Vector4i(5, 6, 7, 8));
    BOOST_CHECK_EQUAL(constPopulated.row(2), Vector4i(9, 10, 11, 12));
    BOOST_CHECK_EQUAL(constPopulated.row(3), Vector4i(13, 14, 15, 16));

    BOOST_CHECK_EQUAL(constPopulated.column(0), Vector4i(1, 5, 9, 13));
    BOOST_CHECK_EQUAL(constPopulated.column(1), Vector4i(2, 6, 10, 14));
    BOOST_CHECK_EQUAL(constPopulated.column(2), Vector4i(3, 7, 11, 15));
    BOOST_CHECK_EQUAL(constPopulated.column(3), Vector4i(4, 8, 12, 16));

    BOOST_CHECK_EQUAL(constPopulated(0, 0), 1);
    BOOST_CHECK_EQUAL(constPopulated(1, 3), 8);
    BOOST_CHECK_EQUAL(constPopulated(3, 1), 14);
    BOOST_CHECK_EQUAL(constPopulated(3, 3), 16);

    Matrix4i mutablePopulated{
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    };

    BOOST_CHECK_EQUAL(mutablePopulated.rowSpan(0), Vector4i(1, 2, 3, 4));
    BOOST_CHECK_EQUAL(mutablePopulated.rowSpan(1), Vector4i(5, 6, 7, 8));
    BOOST_CHECK_EQUAL(mutablePopulated.rowSpan(2), Vector4i(9, 10, 11, 12));
    BOOST_CHECK_EQUAL(mutablePopulated.rowSpan(3), Vector4i(13, 14, 15, 16));

    BOOST_CHECK_EQUAL(mutablePopulated.columnSpan(0), Vector4i(1, 5, 9, 13));
    BOOST_CHECK_EQUAL(mutablePopulated.columnSpan(1), Vector4i(2, 6, 10, 14));
    BOOST_CHECK_EQUAL(mutablePopulated.columnSpan(2), Vector4i(3, 7, 11, 15));
    BOOST_CHECK_EQUAL(mutablePopulated.columnSpan(3), Vector4i(4, 8, 12, 16));

    mutablePopulated.rowSpan(1)[2] = 0;
    BOOST_CHECK_EQUAL(mutablePopulated(1, 2), 0);

    mutablePopulated.columnSpan(3)[0] = 0;
    BOOST_CHECK_EQUAL(mutablePopulated(0, 3), 0);
}

BOOST_AUTO_TEST_CASE(column_major_data) {

    const Matrix2i matrix{
        1, 2,
        3, 4
    };

    const auto * colMajorData = matrix.data();

    BOOST_CHECK_EQUAL(colMajorData[0], 1);
    BOOST_CHECK_EQUAL(colMajorData[1], 3);
    BOOST_CHECK_EQUAL(colMajorData[2], 2);
    BOOST_CHECK_EQUAL(colMajorData[3], 4);
}

BOOST_AUTO_TEST_CASE(multiplication) {


}

BOOST_AUTO_TEST_SUITE_END()
