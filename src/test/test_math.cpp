#include "harken/math.h"
#include "harken/vector.h"

#include <boost/test/unit_test.hpp>
#include <limits>

using Harken::Vector;
using Vector3f = Vector<float, 3>;

BOOST_AUTO_TEST_SUITE(math)

BOOST_AUTO_TEST_CASE(almost_equal) {
    
    constexpr auto Epsilon = std::numeric_limits<float>::epsilon();
    
    BOOST_CHECK(Harken::almostEqual(1.0f, 1.0f));
    BOOST_CHECK(Harken::almostEqual(1.0f, 1.0f + Epsilon));
    
    const auto one = 1.0f;
    auto oneWithError = 0.0f;
    for (auto i = 0; i < 10; ++i) {
        oneWithError += 0.1f;
    }
    
    BOOST_CHECK(Harken::almostEqual(one, oneWithError));
    
    BOOST_CHECK(!Harken::almostEqual(0.0f, Epsilon));
    BOOST_CHECK(!Harken::almostEqual(1e-38f, 2e-38f));
    
    const Vector3f onesVector{1.0f, 1.0f, 1.0f};
    const Vector3f almostOnesVector{1.0f + Epsilon, 1.0f, 1.0f + Epsilon};
    BOOST_CHECK(Harken::almostEqual(onesVector, onesVector));
    BOOST_CHECK(Harken::almostEqual(onesVector, almostOnesVector));
    
    const Vector3f zeroVector{0.0f, 0.0f, 0.0f};
    const Vector3f offsetZeroVector{0.0f, 0.0f, Epsilon};
    BOOST_CHECK(!Harken::almostEqual(zeroVector, offsetZeroVector));
}

BOOST_AUTO_TEST_SUITE_END()
