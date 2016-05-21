#ifndef HARKEN_MATH_H
#define HARKEN_MATH_H

#include "vector.h"

#include <cmath>
#include <limits>
#include <type_traits>

namespace Harken {

    /**
     * Determines whether two floating-point numbers are within some maximum relative distance of
     * each other. Returns @c true if the ratio of the difference between @p lhs and @p rhs to the
     * largest of their magnitudes is less than @p maxRelDiff and @c false otherwise. The default
     * @p maxRelDiff value is generally sufficient to account for the error arising from simple
     * calculations, but for more complex ones it may be necessary to empirically determine a
     * specific value for this parameter.
     */
    
    template<
        typename Float,
        typename = std::enable_if_t<std::is_floating_point<Float>::value>
    >    
    bool almostEqual(const Float lhs, const Float rhs,
                     const Float maxRelDiff = std::numeric_limits<Float>::epsilon()) {
        
        const auto absLHS = std::abs(lhs);
        const auto absRHS = std::abs(rhs);        
        const auto maxAbs = (absLHS > absRHS) ? absLHS : absRHS;
        
        const auto diff = std::abs(lhs - rhs);
        return diff <= maxAbs * maxRelDiff;
    }
    
    /**
     * Uses almostEqual() to compare two vectors of floating-point numbers. Returns @c true if every
     * component of @p lhs is almost equal to the corresponding component of @p rhs and @c false
     * otherwise.
     */

    template<
        typename Float, int Size,
        template<typename, int> class LHSOwnershipPolicy,
        template<typename, int> class RHSOwnershipPolicy,
        typename = std::enable_if_t<std::is_floating_point<Float>::value>
    >    
    bool almostEqual(const Vector<Float, Size, LHSOwnershipPolicy>& lhs,
                     const Vector<Float, Size, RHSOwnershipPolicy>& rhs,
                     const Float maxRelDiff = std::numeric_limits<Float>::epsilon()) {
        
        auto i = 0;
        while (i < Size && almostEqual(lhs[i], rhs[i], maxRelDiff)) {
            ++i;
        }        
        return i == Size;
    }
};

#endif
