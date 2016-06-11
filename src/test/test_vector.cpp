#include "harken_math.h"
#include "harken_vector.h"

#include <boost/test/unit_test.hpp>
#include <array>

template<typename T> using Vector3 = Harken::Vector<T, 3>;
using Vector3i = Vector3<int>;
using Vector3f = Vector3<float>;

template<typename T> using VectorSpan3 = Harken::VectorSpan<T, 3>;
using VectorSpan3i = VectorSpan3<int>;
using VectorSpan3f = VectorSpan3<float>;

template<typename VectorType, typename ElementType>
bool elementsEqual(const VectorType& v, const ElementType x, const ElementType y, const ElementType z) {
    return v[0] == x && v[1] == y && v[2] == z;
}

BOOST_AUTO_TEST_SUITE(vector)

BOOST_AUTO_TEST_CASE(construction_accessors) {

    const Vector3i defaultConstructedVector;
    BOOST_CHECK_EQUAL(defaultConstructedVector, Vector3i(0, 0, 0));

    const Vector3i valueConstructedVector{1, 2, 3};
    BOOST_CHECK_EQUAL(valueConstructedVector, valueConstructedVector);

    BOOST_CHECK_EQUAL(valueConstructedVector.x(), 1);
    BOOST_CHECK_EQUAL(valueConstructedVector.y(), 2);
    BOOST_CHECK_EQUAL(valueConstructedVector.z(), 3);

    BOOST_CHECK(elementsEqual(valueConstructedVector, 1, 2, 3));

    std::array<int, 3> externalData{1, 2, 3};
    const VectorSpan3i vectorSpan{externalData};
    BOOST_CHECK_EQUAL(vectorSpan, vectorSpan);

    BOOST_CHECK_EQUAL(vectorSpan.x(), 1);
    BOOST_CHECK_EQUAL(vectorSpan.y(), 2);
    BOOST_CHECK_EQUAL(vectorSpan.z(), 3);

    BOOST_CHECK(elementsEqual(vectorSpan, 1, 2, 3));

    const Vector3i copyConstructedVector{valueConstructedVector};
    BOOST_CHECK_EQUAL(copyConstructedVector, valueConstructedVector);

    const Vector3i spanCopyConstructedVector{vectorSpan};
    BOOST_CHECK_EQUAL(spanCopyConstructedVector, vectorSpan);

    const VectorSpan3i spanCopyConstructedVectorSpan{vectorSpan};
    BOOST_CHECK_EQUAL(spanCopyConstructedVectorSpan, vectorSpan);

    externalData[0] = 0;
    BOOST_CHECK_EQUAL(vectorSpan, Vector3i(0, 2, 3));
}

BOOST_AUTO_TEST_CASE(stride) {

    std::array<int, 9> externalData{1, 2, 3, 4, 5, 6, 7, 8, 9};
    VectorSpan3i stridedSpan1{externalData, 0, 3};
    VectorSpan3i stridedSpan2{externalData, 1, 3};
    VectorSpan3i stridedSpan3{externalData, 2, 3};

    BOOST_CHECK_EQUAL(stridedSpan1, Vector3i(1, 4, 7));
    BOOST_CHECK_EQUAL(stridedSpan2, Vector3i(2, 5, 8));
    BOOST_CHECK_EQUAL(stridedSpan3, Vector3i(3, 6, 9));

    stridedSpan1 = Vector3i{0, 0, 0};
    stridedSpan2 *= 2;

    const std::array<int, 9> mutatedData{0, 4, 3, 0, 10, 6, 0, 16, 9};
    BOOST_CHECK(externalData == mutatedData);
}

BOOST_AUTO_TEST_CASE(assignment) {

    std::array<int, 3> externalData{1, 2, 3};
    const VectorSpan3i vectorSpanToCopyFrom{externalData};

    Vector3f mutatingVector;
    mutatingVector = vectorSpanToCopyFrom;

    BOOST_CHECK(Harken::almostEqual(mutatingVector, Vector3f(1.0f, 2.0f, 3.0f)));

    Vector3i vectorToCopyFrom{1, 2, 3};

    std::array<float, 3> mutatingExternalData{0.0f, 0.0f, 0.0f};
    VectorSpan3f mutatingVectorSpan{mutatingExternalData};
    mutatingVectorSpan = vectorToCopyFrom;

    BOOST_CHECK(Harken::almostEqual(mutatingVectorSpan, Vector3f(1.0f, 2.0f, 3.0f)));
    BOOST_CHECK(Harken::almostEqual(mutatingExternalData[0], 1.0f));
    BOOST_CHECK(Harken::almostEqual(mutatingExternalData[1], 2.0f));
    BOOST_CHECK(Harken::almostEqual(mutatingExternalData[2], 3.0f));
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
    VectorSpan3i mutatingVectorSpan{externalData};

    mutatingVectorSpan.setX(1);
    mutatingVectorSpan.setY(2);
    mutatingVectorSpan.setZ(3);

    BOOST_CHECK_EQUAL(mutatingVectorSpan, Vector3i(1, 2, 3));
    BOOST_CHECK(elementsEqual(externalData, 1, 2, 3));

    mutatingVectorSpan[0] = 4;
    mutatingVectorSpan[1] = 5;
    mutatingVectorSpan[2] = 6;

    BOOST_CHECK_EQUAL(mutatingVectorSpan, Vector3i(4, 5, 6));
    BOOST_CHECK(elementsEqual(externalData, 4, 5, 6));

    VectorSpan3i mutatingVectorSpanCopy{mutatingVectorSpan};

    mutatingVectorSpanCopy.setX(7);
    mutatingVectorSpanCopy.setY(8);
    mutatingVectorSpanCopy.setZ(9);

    BOOST_CHECK_EQUAL(mutatingVectorSpan, Vector3i(7, 8, 9));
    BOOST_CHECK(elementsEqual(externalData, 7, 8, 9));
}

BOOST_AUTO_TEST_CASE(addition_subtraction) {

    Vector3i mutatingVector{1, 2, 3};
    const Vector3i offset{2, 4, 6};

    mutatingVector += offset;
    BOOST_CHECK_EQUAL(mutatingVector, Vector3i(3, 6, 9));

    mutatingVector -= offset;
    BOOST_CHECK_EQUAL(mutatingVector, Vector3i(1, 2, 3));

    BOOST_CHECK_EQUAL(mutatingVector + offset, Vector3i(3, 6, 9));
    BOOST_CHECK_EQUAL(mutatingVector - offset, Vector3i(-1, -2, -3));

    std::array<int, 3> externalData{1, 2, 3};
    VectorSpan3i mutatingVectorSpan{externalData};
    mutatingVectorSpan += offset;

    BOOST_CHECK_EQUAL(mutatingVectorSpan, Vector3i(3, 6, 9));
    BOOST_CHECK(elementsEqual(externalData, 3, 6, 9));

    mutatingVectorSpan -= offset;
    BOOST_CHECK_EQUAL(mutatingVectorSpan, Vector3i(1, 2, 3));
    BOOST_CHECK(elementsEqual(externalData, 1, 2, 3));

    BOOST_CHECK_EQUAL(mutatingVectorSpan + offset, Vector3i(3, 6, 9));
    BOOST_CHECK_EQUAL(mutatingVectorSpan - offset, Vector3i(-1, -2, -3));
}

BOOST_AUTO_TEST_CASE(scalar_multiplication) {

    std::array<int, 3> externalData{1, 2, 3};
    VectorSpan3i vectorSpan{externalData};

    auto vectorCopy = +vectorSpan;
    BOOST_CHECK_EQUAL(vectorCopy, vectorSpan);

    vectorCopy.setX(0);
    BOOST_CHECK_EQUAL(vectorCopy, Vector3i(0, 2, 3));
    BOOST_CHECK(elementsEqual(externalData, 1, 2, 3));

    auto negVector = -vectorSpan;
    BOOST_CHECK_EQUAL(negVector, Vector3i(-1, -2, -3));

    negVector.setX(0);
    BOOST_CHECK_EQUAL(negVector, Vector3i(0, -2, -3));
    BOOST_CHECK(elementsEqual(externalData, 1, 2, 3));

    auto leftMult = 2 * vectorSpan;
    BOOST_CHECK_EQUAL(leftMult, Vector3i(2, 4, 6));

    leftMult.setX(0);
    BOOST_CHECK_EQUAL(leftMult, Vector3i(0, 4, 6));
    BOOST_CHECK(elementsEqual(externalData, 1, 2, 3));

    auto rightMult = vectorSpan * 2;
    BOOST_CHECK_EQUAL(rightMult, Vector3i(2, 4, 6));

    rightMult.setX(0);
    BOOST_CHECK_EQUAL(rightMult, Vector3i(0, 4, 6));
    BOOST_CHECK(elementsEqual(externalData, 1, 2, 3));

    auto divided = vectorSpan / 2;
    BOOST_CHECK_EQUAL(divided, Vector3i(0, 1, 1));

    divided.setX(1);
    BOOST_CHECK_EQUAL(divided, Vector3i(1, 1, 1));
    BOOST_CHECK(elementsEqual(externalData, 1, 2, 3));

    vectorSpan *= 2;
    BOOST_CHECK_EQUAL(vectorSpan, Vector3i(2, 4, 6));
    BOOST_CHECK(elementsEqual(externalData, 2, 4, 6));

    vectorSpan /= 2;
    BOOST_CHECK_EQUAL(vectorSpan, Vector3i(1, 2, 3));
    BOOST_CHECK(elementsEqual(externalData, 1, 2, 3));

    const auto origVector = Vector3f{1.0f, 2.0f, 3.0f};
    const auto floatProduct1 = origVector * -0.5f;
    BOOST_CHECK(Harken::almostEqual(floatProduct1, Vector3f(-0.5f, -1.0f, -1.5f)));

    const auto floatProduct2 = 0.0f * origVector;
    BOOST_CHECK(Harken::almostEqual(floatProduct2, Vector3f(0.0f, 0.0f, 0.0f)));

    const auto floatQuotient = origVector / 2.0f;
    BOOST_CHECK(Harken::almostEqual(floatQuotient, Vector3f(0.5f, 1.0f, 1.5f)));
}

BOOST_AUTO_TEST_CASE(vector_products) {

    BOOST_CHECK_EQUAL(Harken::dot(Vector3i(1, 2, 3), Vector3i(4, 5, 6)), 32);
    BOOST_CHECK_EQUAL(Harken::dot(Vector3i(1, 1, 0), Vector3i(-1, 1, 0)), 0);

    BOOST_CHECK_EQUAL(Harken::cross(Vector3i(1, 2, 3), Vector3i(4, 5, 6)), Vector3i(-3, 6, -3));
    BOOST_CHECK_EQUAL(Harken::cross(Vector3i(1, 2, 3), Vector3i(2, 4, 6)), Vector3i(0, 0, 0));
}

BOOST_AUTO_TEST_SUITE_END()
