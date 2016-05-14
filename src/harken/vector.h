#ifndef HARKEN_VECTOR_H
#define HARKEN_VECTOR_H

#include <array>
#include <ostream>

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
    
    /**
     * Ownership policy for a vector that owns its coordinate data. Provides an implementation of
     * <tt>operator[]</tt> for accessing and mutating these data, and constructors for initialising
     * them.
     */
    
    template<typename T, int Size>
    class OwningVectorPolicy : public NamedVectorAccessPolicy<T, Size, OwningVectorPolicy> {
    public:
        
        /**
         * Constructs the vector and fills its coordinate data with zeros.
         */
        
        OwningVectorPolicy() {
            m_v.fill(0);
        }
        
        /**
         * Constructs the vector and initialises its coordinates to the provided values. This
         * constructor must be called with <tt>Size</tt> arguments, each corresponding to a
         * component of the vector.
         */
        
        template<typename... Args>
        OwningVectorPolicy(Args... args)
            : m_v{args...} {
        
            static_assert(sizeof...(Args) == Size,
                "The number of arguments provided to the Vector constructor does not match its dimension.");
        }
        
        /**
         * Constructs the vector by copying its data from another vector. @p rhs must be of the same
         * dimension as the vector being constructed, but may have a different type or ownership
         * policy.
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
        std::array<T, Size> m_v;
    };
    
    /**
     * Ownership policy for a vector whose coordinate data are stored elsewhere (that is, a vector
     * "span"). Provides an implementation of <tt>operator[]</tt> for accessing and mutating these
     * data, and a constructor for specifying where the data should be observed from and written to.
     */
    
    template<typename T, int Size>
    class SpanVectorPolicy : public NamedVectorAccessPolicy<T, Size, SpanVectorPolicy>  {
    public:
    
        /**
         * Constructs a vector spanning data at @p v. There must be at least @c Size coordinate 
         * items at the location pointed to by @p v, since the vector may attempt to read and write
         * data via this pointer up to an offset of @c Size.
         */
        
        explicit SpanVectorPolicy(T * const v)
            : m_v{v} {
        }
        
        T& operator[](const int i) {
            return m_v[i];
        }
        
        T operator[](const int i) const {
            return m_v[i];
        }
        
    protected:
        ~SpanVectorPolicy() = default;
        
    private:
        T * const m_v;
    };
    
    /**
     * A mathematical vector of arbitrary size that provides common operations for performing 
     * spatial computations. The ownership strategy of the Vector is separated out into a separate
     * policy class. The most common and simple case is when the vector itself owns its coordinate 
     * data, but the policy-based design also trivially allows the case when the vector is a
     * view onto data stored elsewhere (such as in an OpenGL vertex buffer object).
     * 
     * Ownership policies must provide (at a minimum) const and non-const implementations of 
     * <tt>operator[]</tt>, which become the basis for the vector's other operations. They will
     * typically also provide constructors which are inherited by the vector itself. Calling
     * <tt>operator[]</tt> with an index outside the valid range (from <tt>0</tt> to
     * <tt>Size - 1</tt>) is undefined behaviour.
     */
    
    template<typename T, int Size, template<typename, int> class OwnershipPolicy = OwningVectorPolicy>
    class Vector : public OwnershipPolicy<T, Size> {
    public:
        
        using OwnershipPolicy<T, Size>::OwnershipPolicy;
    };
    
    /**
     * Determines whether two vectors are equal; that is, whether all of their components are equal.
     * The comparison is performed component-wise using <tt>T::operator==</tt>; 
     */
    
    template<
        typename T, int Size,
        template<typename, int> class LHSOwnershipPolicy,
        template<typename, int> class RHSOwnershipPolicy
    >
    bool operator==(const Vector<T, Size, LHSOwnershipPolicy>& lhs,
                    const Vector<T, Size, RHSOwnershipPolicy>& rhs) {
        
        auto i = 0;
        while (i < Size && lhs[i] == rhs[i]) {
            ++i;
        }        
        return i == Size;
    }
    
    /**
     * Determines whether two vectors are not equal; that is, whether at least one of their 
     * components is not equal. Equivalent to <tt>!(lhs == rhs)</tt>.
     */
    
    template<
        typename T, int Size,
        template<typename, int> class LHSOwnershipPolicy,
        template<typename, int> class RHSOwnershipPolicy
    >
    bool operator!=(const Vector<T, Size, LHSOwnershipPolicy>& lhs,
                    const Vector<T, Size, RHSOwnershipPolicy>& rhs) {
    
        return !(lhs == rhs);
    }
    
    /**
     * Prints the vector @p v to @p os, using the current formatting manipulators on @p os.
     */
    
    template<typename T, int Size, template<typename, int> class OwnershipPolicy>
    std::ostream& operator<<(std::ostream& os, const Vector<T, Size, OwnershipPolicy>& v) {
        
        os << '(';
        for (auto i = 0; i < Size; ++i) {            
            if (i > 0) {
                os << ", ";
            }
            os << v[i];
        }
        
        os << ')';
        return os;
    }

    
    template<typename T, int Size>
    using VectorSpan = Vector<T, Size, SpanVectorPolicy>;
}

#endif