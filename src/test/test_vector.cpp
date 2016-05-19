#include "harken/vector.h"

#include <boost/test/unit_test.hpp>
#include <array>

using Harken::Vector;
using Harken::VectorSpan;

using Vector3i = Vector<int, 3>;
using Vector3f = Vector<float, 3>;
using VectorSpan3i = VectorSpan<int, 3>;

BOOST_AUTO_TEST_SUITE(vector)

BOOST_AUTO_TEST_CASE(construction_accessors) {
    
    const Vector3i defaultConstructedVector;
    const Vector3i zeroVector{0, 0, 0};
    BOOST_CHECK_EQUAL(defaultConstructedVector, zeroVector);
    
    const Vector3i valueConstructedVector{1, 2, 3};
    BOOST_CHECK_EQUAL(valueConstructedVector, valueConstructedVector);
    
    BOOST_CHECK_EQUAL(valueConstructedVector.x(), 1);
    BOOST_CHECK_EQUAL(valueConstructedVector.y(), 2);
    BOOST_CHECK_EQUAL(valueConstructedVector.z(), 3);
    
    BOOST_CHECK_EQUAL(valueConstructedVector[0], 1);
    BOOST_CHECK_EQUAL(valueConstructedVector[1], 2);
    BOOST_CHECK_EQUAL(valueConstructedVector[2], 3);
    
    std::array<int, 3> externalData{1, 2, 3};
    const VectorSpan3i vectorSpan{externalData.data()};
    BOOST_CHECK_EQUAL(vectorSpan, vectorSpan);
    
    BOOST_CHECK_EQUAL(vectorSpan.x(), 1);
    BOOST_CHECK_EQUAL(vectorSpan.y(), 2);
    BOOST_CHECK_EQUAL(vectorSpan.z(), 3);
    
    BOOST_CHECK_EQUAL(vectorSpan[0], 1);
    BOOST_CHECK_EQUAL(vectorSpan[1], 2);
    BOOST_CHECK_EQUAL(vectorSpan[2], 3);
    
    const Vector3i copyConstructedVector{valueConstructedVector};
    BOOST_CHECK_EQUAL(copyConstructedVector, valueConstructedVector);
    
    const Vector3i spanCopyConstructedVector{vectorSpan};
    BOOST_CHECK_EQUAL(spanCopyConstructedVector, vectorSpan);
    
    const VectorSpan3i spanCopyConstructedVectorSpan{vectorSpan};
    BOOST_CHECK_EQUAL(spanCopyConstructedVectorSpan, vectorSpan);
}

BOOST_AUTO_TEST_CASE(mutators) {
    
    Vector3i mutatingVector;
    
    mutatingVector.setX(1);
    mutatingVector.setY(2);
    mutatingVector.setZ(3);
    
    BOOST_CHECK_EQUAL(mutatingVector, Vector3i(1, 2, 3));
    
    mutatingVector[0] = 4;
    mutatingVector[1] = 5;
    mutatingVector[2] = 6;
    
    BOOST_CHECK_EQUAL(mutatingVector, Vector3i(4, 5, 6));
    
    std::array<int, 3> externalData{0, 0, 0};
    VectorSpan3i mutatingVectorSpan{externalData.data()};
    
    mutatingVectorSpan.setX(1);
    mutatingVectorSpan.setY(2);
    mutatingVectorSpan.setZ(3);
    
    BOOST_CHECK_EQUAL(mutatingVectorSpan, Vector3i(1, 2, 3));
    BOOST_CHECK_EQUAL(externalData[0], 1);
    BOOST_CHECK_EQUAL(externalData[1], 2);
    BOOST_CHECK_EQUAL(externalData[2], 3);
    
    mutatingVectorSpan[0] = 4;
    mutatingVectorSpan[1] = 5;
    mutatingVectorSpan[2] = 6;
    
    BOOST_CHECK_EQUAL(mutatingVectorSpan, Vector3i(4, 5, 6));
    BOOST_CHECK_EQUAL(externalData[0], 4);
    BOOST_CHECK_EQUAL(externalData[1], 5);
    BOOST_CHECK_EQUAL(externalData[2], 6);
    
    VectorSpan3i mutatingVectorSpanCopy{mutatingVectorSpan};
    
    mutatingVectorSpanCopy.setX(7);
    mutatingVectorSpanCopy.setY(8);
    mutatingVectorSpanCopy.setZ(9);
    
    BOOST_CHECK_EQUAL(mutatingVectorSpan, Vector3i(7, 8, 9));
    BOOST_CHECK_EQUAL(externalData[0], 7);
    BOOST_CHECK_EQUAL(externalData[1], 8);
    BOOST_CHECK_EQUAL(externalData[2], 9);
}

BOOST_AUTO_TEST_CASE(addition_subtraction) {
    
    Vector3i mutatingVector{1, 2, 3};
    const Vector3i offset{2, 4, 6};
    
    mutatingVector += offset;
    BOOST_CHECK_EQUAL(mutatingVector, Vector3i(3, 6, 9));
    
    mutatingVector -= offset;
    BOOST_CHECK_EQUAL(mutatingVector, Vector3i(1, 2, 3));
    
    BOOST_CHECK_EQUAL(mutatingVector + Vector3i(2, 4, 6), Vector3i(3, 6, 9));
    BOOST_CHECK_EQUAL(mutatingVector - Vector3i(2, 4, 6), Vector3i(-1, -2, -3));
}

BOOST_AUTO_TEST_SUITE_END()
