#ifndef HARKEN_VECTOR_H
#define HARKEN_VECTOR_H

#include "harken_global.h"

#include <array>
#include <cassert>
#include <cstddef>
#include <ostream>
#include <type_traits>

namespace Harken {

    template<typename T, int Size, template<typename, int> class OwnershipPolicy>
    class Vector;

    /**
     * A base class that can be specialised to provide named accessors and mutators for vectors of
     * specific sizes (like x, y, and z when @p Size = 3). This may only be used as a CRTP base
     * class of a Vector ownership policy, since it relies upon that policy class to access data
     * from the vector when implementing the named accessors.
     */

    template<typename T, int Size, template<typename, int> class OwnershipPolicy>
    class NamedVectorAccessPolicy {
    protected:
        ~NamedVectorAccessPolicy() = default;
    };

    template<typename T, int Size, template<typename, int> class OwnershipPolicy>
    T get(const NamedVectorAccessPolicy<T, Size, OwnershipPolicy>& accessPolicy, const int i) {
        return static_cast<const OwnershipPolicy<T, Size>&>(accessPolicy)[i];
    }

    template<typename T, int Size, template<typename, int> class OwnershipPolicy>
    void set(NamedVectorAccessPolicy<T, Size, OwnershipPolicy>& accessPolicy, const int i, const T value) {
        static_cast<OwnershipPolicy<T, Size>&>(accessPolicy)[i] = value;
    }

    template<typename T, template<typename, int> class OwnershipPolicy>
    class NamedVectorAccessPolicy<T, 2, OwnershipPolicy> {
    public:

        T x() const {
            return get(*this, 0);
        }

        T y() const {
            return get(*this, 1);
        }

        void setX(const T value) {
            set(*this, 0, value);
        }

        void setY(const T value) {
            set(*this, 1, value);
        }

    protected:
        ~NamedVectorAccessPolicy() = default;
    };

    template<typename T, template<typename, int> class OwnershipPolicy>
    class NamedVectorAccessPolicy<T, 3, OwnershipPolicy> {
    public:

        T x() const {
            return get(*this, 0);
        }

        T y() const {
            return get(*this, 1);
        }

        T z() const {
            return get(*this, 2);
        }

        void setX(const T value) {
            set(*this, 0, value);
        }

        void setY(const T value) {
            set(*this, 1, value);
        }

        void setZ(const T value) {
            set(*this, 2, value);
        }

    protected:
        ~NamedVectorAccessPolicy() = default;
    };
    
    template<typename T, template<typename, int> class OwnershipPolicy>
    class NamedVectorAccessPolicy<T, 4, OwnershipPolicy> {
    public:

        T x() const {
            return get(*this, 0);
        }

        T y() const {
            return get(*this, 1);
        }

        T z() const {
            return get(*this, 2);
        }
        
        T w() const {
            return get(*this, 3);
        }

        void setX(const T value) {
            set(*this, 0, value);
        }

        void setY(const T value) {
            set(*this, 1, value);
        }

        void setZ(const T value) {
            set(*this, 2, value);
        }
        
        void setW(const T value) {
            set(*this, 3, value);
        }

    protected:
        ~NamedVectorAccessPolicy() = default;
    };

    /**
     * Ownership policy for a vector that owns its coordinate data. Provides an implementation of
     * <tt>operator[]</tt> for accessing and mutating these data, and constructors for initialising
     * them.
     */

    template<typename T, int Size>
    class OwningVectorPolicy : public NamedVectorAccessPolicy<T, Size, OwningVectorPolicy> {
    public:

        /**
         * Constructs the vector and initialises it to zero.
         */

        OwningVectorPolicy() = default;

        /**
         * Constructs the vector and initialises its coordinates to the provided values. This
         * constructor must be called with exactly @c Size arguments, each convertible to the
         * vector's component type @c T and corresponding to a component of the vector.
         */

        template<typename... Args>
        OwningVectorPolicy(Args... args)
            : m_v{args...} {

            static_assert(sizeof...(Args) == Size,
                 "The number of arguments provided to the Vector constructor must match its "
                 "dimension.");
        }

        /**
         * Constructs the vector by copying its data from another vector. @p rhs must be of the
         * same dimension as the vector being constructed, but may have a different type or
         * ownership policy.
         */

        template<typename X, template<typename, int> class RHSOwnershipPolicy>
        explicit OwningVectorPolicy(const Vector<X, Size, RHSOwnershipPolicy>& rhs) {

            for (auto i = 0; i < Size; ++i) {
                m_v[i] = rhs[i];
            }
        }

        T& operator[](const int i) {
            return m_v[i];
        }

        T operator[](const int i) const {
            return m_v[i];
        }

    protected:
        ~OwningVectorPolicy() = default;

    private:
        std::array<T, Size> m_v{};
    };

    /**
     * Ownership policy for a vector whose coordinate data are stored elsewhere (that is, a vector
     * "span"). Provides an implementation of <tt>operator[]</tt> for accessing and mutating these
     * data, and a constructor for specifying where the data should be observed from and written to.
     * Vectors using this ownership policy are cheap to copy.
     *
     * The data in the underlying array that correspond to the coordinates of the vector span may be
     * interleaved with other data; if so, the @c stride parameter may be specified at construction
     * to specify how much each item of the vector is offset from the previous item. A value of @c 1
     * specifies that the vector data are contiguous.
     */

    template<typename T, int Size>
    class SpanVectorPolicy : public NamedVectorAccessPolicy<T, Size, SpanVectorPolicy>  {
    public:

        /**
         * Constructs a vector spanning data at @p v. There must be at least @c Size coordinate
         * items offset @p stride items from each other at the location pointed to by @p v, since
         * the vector may attempt to read and write data via this pointer up to an offset of
         * <tt>(Size - 1) * stride</tt>.
         * @param v The array containing the data describing the components of the vector span.
         * @param stride The offset of each component from the previous one in the underlying data
         *               array.
         */

        explicit SpanVectorPolicy(T * const v, const int stride = 1)
            : m_v{v}, m_stride{stride} {
        }

        /**
         * Constructs a vector spanning data contained within the <tt>std::array</tt> @p v. The
         * offset of the data to span within @p v is given by @p offset, and the stride of the data
         * to span is given by @p stride. There must be at least <tt>(Size - 1) * stride</tt>
         * elements at @p offset within @p v.
         */

        template<std::size_t TargetSize>
        explicit SpanVectorPolicy(std::array<T, TargetSize>& v, const int offset = 0, const int stride = 1)
            : m_v{v.data() + offset}, m_stride{stride} {

            assert(offset + (Size - 1) * m_stride < TargetSize && "Vector span target is outside of array bounds.");
        }

        T& operator[](const int i) {
            return m_v[i * m_stride];
        }

        T operator[](const int i) const {
            return m_v[i * m_stride];
        }

    protected:

        ~SpanVectorPolicy() = default;

    private:

        T * const m_v;
        const int m_stride;
    };

    /**
     * A mathematical vector of arbitrary size that provides common operations for performing
     * spatial computations. The ownership strategy of the Vector is separated out into a separate
     * policy class. The most common and simple case is when the vector itself owns its coordinate
     * data, but the policy-based design also trivially allows the case when the vector is a view
     * onto data stored elsewhere (such as in a matrix).
     *
     * The interface for the vector's basic operations is based upon that of <tt>std::complex</tt>.
     * Calling <tt>operator[]</tt> with an index outside the valid range (from <tt>0</tt> to
     * <tt>Size - 1</tt>) is undefined behaviour.
     *
     * @tparam T The type of data to be contained as the components of the vector. Must be an
     *            arithmetic type.
     * @tparam Size The number of components that the vector should contain.
     * @tparam OwnershipPolicy The ownership policy describing how this vector should store and
     *                         access its data. Ownership policies must provide (at a minimum) const
     *                         and non-const implementations of <tt>operator[]</tt>, which become
     *                         the basis for the vector's other operations. They will typically also
     *                         provide constructors which are inherited by the vector itself.
     */

    template<typename T, int Size, template<typename, int> class OwnershipPolicy = OwningVectorPolicy>
    class Vector : public OwnershipPolicy<T, Size> {
    public:

        static_assert(std::is_arithmetic<T>::value, "The component type of a Vector must be arithmetic.");

        using ComponentType = T;
        using OwnershipPolicy<T, Size>::OwnershipPolicy;

        // Even though the constructors for the vector are provided by the ownership policy class,
        // we must explicitly provide a default constructor so that Vector is not considered a POD
        // type; this prevents us from getting an error when a const Vector is default-constructed
        // (to be zero). This constructor will only actually be usable if the ownership policy
        // supports default construction.

        Vector() {}

        // Vectors of any ownership policy can be assigned to from vectors of any other ownership
        // policy (and compatible type). However, only owning vectors can be constructed as copies
        // of another vector (since a non-owning vector must first be told where to access its data
        // before it can be set); this behaviour is implemented in the ownership policy.

        template<typename X, template<typename, int> class RHSOwnershipPolicy>
        Vector<T, Size, OwnershipPolicy>& operator=(const Vector<X, Size, RHSOwnershipPolicy>& rhs) {

            for (auto i = 0; i < Size; ++i) {
                (*this)[i] = rhs[i];
            }
            return *this;
        }

        template<template<typename, int> class RHSOwnershipPolicy>
        Vector<T, Size, OwnershipPolicy>& operator+=(const Vector<T, Size, RHSOwnershipPolicy>& rhs) {

            for (auto i = 0; i < Size; ++i) {
                (*this)[i] += rhs[i];
            }
            return *this;
        }

        template<template<typename, int> class RHSOwnershipPolicy>
        Vector<T, Size, OwnershipPolicy>& operator-=(const Vector<T, Size, RHSOwnershipPolicy>& rhs) {

            for (auto i = 0; i < Size; ++i) {
                (*this)[i] -= rhs[i];
            }
            return *this;
        }

        Vector<T, Size, OwnershipPolicy>& operator*=(const T rhs) {

            for (auto i = 0; i < Size; ++i) {
                (*this)[i] *= rhs;
            }
            return *this;
        }

        Vector<T, Size, OwnershipPolicy>& operator/=(const T rhs) {

            for (auto i = 0; i < Size; ++i) {
                (*this)[i] /= rhs;
            }
            return *this;
        }
    };

    template<
        typename T, int Size,
        template<typename, int> class LHSOwnershipPolicy,
        template<typename, int> class RHSOwnershipPolicy
    >
    bool operator==(const Vector<T, Size, LHSOwnershipPolicy>& lhs,
                    const Vector<T, Size, RHSOwnershipPolicy>& rhs) {

        for (auto i = 0; i < Size; ++i) {
            if (lhs[i] != rhs[i]) {
                return false;
            }
        }

        return true;
    }

    template<
        typename T, int Size,
        template<typename, int> class LHSOwnershipPolicy,
        template<typename, int> class RHSOwnershipPolicy
    >
    bool operator!=(const Vector<T, Size, LHSOwnershipPolicy>& lhs,
                    const Vector<T, Size, RHSOwnershipPolicy>& rhs) {

        return !(lhs == rhs);
    }

    // Note that all non-assigning arithmetic operations upon a non-owning vector return a vector
    // that owns (a copy of) the original data, to prevent surprising mutations-at-a-distance of the
    // external data.

    template<typename T, int Size, template<typename, int> class OwnershipPolicy>
    Vector<T, Size> operator+(const Vector<T, Size, OwnershipPolicy>& val) {
        return Vector<T, Size>{val};
    }

    template<typename T, int Size, template<typename, int> class OwnershipPolicy>
    Vector<T, Size> operator-(const Vector<T, Size, OwnershipPolicy>& val) {
        return Vector<T, Size>{-1 * val};
    }

    template<
        typename T, int Size,
        template<typename, int> class LHSOwnershipPolicy,
        template<typename, int> class RHSOwnershipPolicy
    >
    Vector<T, Size> operator+(const Vector<T, Size, LHSOwnershipPolicy>& lhs,
                              const Vector<T, Size, RHSOwnershipPolicy>& rhs) {

        Vector<T, Size> result{lhs};
        result += rhs;
        return result;
    }

    template<
        typename T, int Size,
        template<typename, int> class LHSOwnershipPolicy,
        template<typename, int> class RHSOwnershipPolicy
    >
    Vector<T, Size> operator-(const Vector<T, Size, LHSOwnershipPolicy>& lhs,
                              const Vector<T, Size, RHSOwnershipPolicy>& rhs) {

        Vector<T, Size> result{lhs};
        result -= rhs;
        return result;
    }

    template<typename T, int Size, template<typename, int> class OwnershipPolicy>
    Vector<T, Size> operator*(const Vector<T, Size, OwnershipPolicy>& lhs, const T rhs) {

        Vector<T, Size> result{lhs};
        result *= rhs;
        return result;
    }

    template<typename T, int Size, template<typename, int> class OwnershipPolicy>
    Vector<T, Size> operator*(const T lhs, const Vector<T, Size, OwnershipPolicy>& rhs) {
        return rhs * lhs;
    }

    template<typename T, int Size, template<typename, int> class OwnershipPolicy>
    Vector<T, Size> operator/(const Vector<T, Size, OwnershipPolicy>& lhs, const T rhs) {

        Vector<T, Size> result{lhs};
        result /= rhs;
        return result;
    }

    /**
     * Prints @p vector to @p os, using the current formatting manipulators on @p os.
     */

    template<typename T, int Size, template<typename, int> class OwnershipPolicy>
    std::ostream& operator<<(std::ostream& os, const Vector<T, Size, OwnershipPolicy>& vector) {

        os << '(';
        for (auto i = 0; i < Size; ++i) {
            if (i > 0) {
                os << ", ";
            }
            os << vector[i];
        }

        os << ')';
        return os;
    }

    /**
     * Computes the cross (vector) product of two 3-dimensional vectors. Returns a 3D vector
     * perpendicular to both @p lhs and @p rhs whose magnitude is equal to the area of the
     * parallelogram that they span, oriented "upwards" out of the plane containing @p lhs and
     * @p rhs if the latter lies anticlockwise of the former, or "downwards" in the reverse
     * situation.
     */

    template<
        typename T,
        template<typename, int> class LHSOwnershipPolicy,
        template<typename, int> class RHSOwnershipPolicy
    >
    Vector<T, 3> cross(const Vector<T, 3, LHSOwnershipPolicy>& lhs,
                       const Vector<T, 3, RHSOwnershipPolicy>& rhs) {

        return Vector<T, 3>{
            lhs.y() * rhs.z() - lhs.z() * rhs.y(),
            lhs.z() * rhs.x() - lhs.x() * rhs.z(),
            lhs.x() * rhs.y() - lhs.y() * rhs.x()
        };
    }

    /**
     * Computes the dot (scalar) product of two vectors of arbitrary (but equal) dimension. Returns
     * the product of the norm of @p lhs, the norm of @p rhs, and the cosine of the angle between
     * @p lhs and @p rhs.
     */

    template<
        typename T, int Size,
        template<typename, int> class LHSOwnershipPolicy,
        template<typename, int> class RHSOwnershipPolicy
    >
    T dot(const Vector<T, Size, LHSOwnershipPolicy>& lhs,
          const Vector<T, Size, RHSOwnershipPolicy>& rhs) {

        auto result = T{0};
        for (auto i = 0; i < Size; ++i) {
            result += lhs[i] * rhs[i];
        }
        return result;
    }
    
    template<typename T>
    using Vector2 = Harken::Vector<T, 2>;
    
    template<typename T>
    using Vector3 = Harken::Vector<T, 3>;
    
    template<typename T>
    using Vector4 = Harken::Vector<T, 4>;

    template<typename T, int Size>
    using VectorSpan = Vector<T, Size, SpanVectorPolicy>;
    
    template<typename T>
    using VectorSpan2 = Harken::VectorSpan<T, 2>;
    
    template<typename T>
    using VectorSpan3 = Harken::VectorSpan<T, 3>;
    
    template<typename T>
    using VectorSpan4 = Harken::VectorSpan<T, 4>;
}

#endif
