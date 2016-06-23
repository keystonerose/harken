#include "harken_glmath.h"
#include "harken_math.h"

#include <boost/test/unit_test.hpp>

#include <tuple>
#include <vector>

using Harken::Matrix4f;
using Harken::Vector3f;
using Harken::Vector4f;

BOOST_AUTO_TEST_SUITE(gl_math)

BOOST_AUTO_TEST_CASE(translation) {

    const Matrix4f identity;
    const auto noTranslation = Harken::translationMatrix(0.0f, 0.0f, 0.0f);

    BOOST_CHECK(Harken::almostEqual(noTranslation, identity));

    const Vector4f initial{2.0f, 1.0f, 0.5f, 1.0f};
    const auto translation1 = Harken::translationMatrix(1.0f, 0.0f, -3.0f);
    const auto translation2 = Harken::translationMatrix(-2.0f, -1.0f, -0.5f);

    BOOST_CHECK(Harken::almostEqual(translation1 * initial, Vector4f{3.0f, 1.0f, -2.5f, 1.0f}));
    BOOST_CHECK(Harken::almostEqual(translation2 * initial, Vector4f{0.0f, 0.0f, 0.0f, 1.0f}));

    const auto compositeTranslation1 = translation1 * translation2;
    const auto compositeTranslation2 = translation2 * translation1;
    const Vector4f compositeTranslated{1.0f, 0.0f, -3.0f, 1.0f};

    BOOST_CHECK(Harken::almostEqual(compositeTranslation1 * initial, compositeTranslated));
    BOOST_CHECK(Harken::almostEqual(compositeTranslation2 * initial, compositeTranslated));
}

BOOST_AUTO_TEST_SUITE_END()
