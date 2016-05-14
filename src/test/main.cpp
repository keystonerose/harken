#include "tester.h"
#include "harken/stringbuilder.h"
#include "harken/vector.h"

#include <array>
#include <cstdlib>
#include <iostream>

using Harken::OwningVectorPolicy;
using Harken::SpanVectorPolicy;
using Harken::StringBuilder;
using Harken::Vector;
using Harken::VectorSpan;

using Vector3i = Vector<int, 3>;
using VectorSpan3i = VectorSpan<int, 3>;

namespace VectorTest {

    namespace {
        std::array<int, 3> externalData;
    }
    
    /**
     * Constructs a <tt>Vector&lt;int, 3&gt;</tt> object for testing. For owning vectors, performs
     * the construction directly; for non-owning vectors, sets up some external data to contain the
     * specified coordinates and then constructs a @c VectorSpan pointing to those external data.
     */

    template<template<typename, int> class OwnershipPolicy>
    Vector<int, 3, OwnershipPolicy> createVector3i(const int x, const int y, const int z) {
        return {x, y, z};
    }

    template<>
    VectorSpan3i createVector3i<SpanVectorPolicy>(const int x, const int y, const int z) {
        externalData[0] = x;
        externalData[1] = y;
        externalData[2] = z;
        return VectorSpan3i{externalData.data()};
    }
    
    // Most of the vector test functions are templated by the vector ownership policy. This means
    // that the same tests can be run for both vectors and vector spans while keeping the
    // implementation differences isolated to template specialisations where necessary.
    
    /**
     * Tests that the components of an owning or non-owning vector can be accessed via the subscript
     * operator, and that doing so yields the expected values.
     */
    
    template<template<typename, int> class OwnershipPolicy>
    void testAccessorSubscript(Tester& tester) {
        
        const std::array<int, 3> values{1, 2, 3};
        const auto v = createVector3i<OwnershipPolicy>(values[0], values[1], values[2]);
        
        for (auto i = 0; i < 3; ++i) {
            tester.assertEqual(StringBuilder{} << "Accessing operator[](" << i << ")", v[i], values[i]);
        }
    }
    
    /**
     * Tests that the equality operation for owning and non-owning vectors is implemented so that
     * vectors are equal to themselves.
     */

    template<template<typename, int> class OwnershipPolicy>
    void testEqualityReflexivity(Tester& tester) {
        
        const auto v = createVector3i<OwnershipPolicy>(1, 2, 3);
        tester.assertEqual("operator== reflexivity", v, v);
    }
    
    /**
     * Tests that owning and non-owning vectors provide mutator methods (set{X,Y,Z}) that can be
     * used to modify either the corresponding components of the vector (in the case of an owning
     * vector) or the elements of the underlying array (in the case of a non-owning vector).
     */
    
    template<template<typename, int> class OwnershipPolicy>
    void testMutatorMethods(Tester& tester) {
        
        auto v = createVector3i<OwnershipPolicy>(0, 0, 0);
        v.setX(1);
        v.setY(2);
        v.setZ(3);
        tester.assertEqual("Mutator methods", v, Vector3i{1, 2, 3});
    }
    
    template<>
    void testMutatorMethods<SpanVectorPolicy>(Tester& tester) {
        
        const std::array<int, 3> values{1, 2, 3};
        auto v = createVector3i<SpanVectorPolicy>(0, 0, 0);
        
        v.setX(values[0]);
        v.setY(values[1]);
        v.setZ(values[2]);
        
        tester.assertEqual("Mutator methods", v, Vector3i{1, 2, 3});
        
        for (auto i = 0; i < 3; ++i) {
            tester.assertEqual(StringBuilder{} << "Mutator methods: span target " << i, values[i], externalData[i]);
        }
    }
    
    /**
     * Tests that owning and non-owning vectors provide a subscript operator that can be used to
     * modify elements in the vector (in the case of an owning vector) or in the underlying array
     * (in the case of a non-owning vector).
     */
    
    template<template<typename, int> class OwnershipPolicy>
    void testMutatorSubscript(Tester& tester) {
        
        auto v = createVector3i<OwnershipPolicy>(0, 0, 0);
        v[0] = 1;
        v[1] = 2;
        v[2] = 3;
        tester.assertEqual("Mutating operator[]", v, Vector3i{1, 2, 3});
    }
    
    template<>
    void testMutatorSubscript<SpanVectorPolicy>(Tester& tester) {
        
        const std::array<int, 3> values{1, 2, 3};
        auto v = createVector3i<SpanVectorPolicy>(0, 0, 0);
        
        for (auto i = 0; i < 3; ++i) {
            v[i] = values[i];
        }
        
        tester.assertEqual("Mutating operator[]", v, Vector3i{1, 2, 3});
        
        for (auto i = 0; i < 3; ++i) {
            tester.assertEqual(StringBuilder{} << "Mutating operator[]: span target " << i, values[i], externalData[i]);
        }
    }
    
    /**
     * Tests that owning vectors are initialised to zero when constructed.
     */
    
    void testZeroInitialisation(Tester& tester) {
        Vector3i v;
        tester.assertEqual("Zero initialisation", v, Vector3i{0, 0, 0});
    }

    template<template<typename, int> class OwnershipPolicy>
    void runTests(Tester& tester) {
        
        testAccessorSubscript<OwnershipPolicy>(tester);
        testEqualityReflexivity<OwnershipPolicy>(tester);
        testMutatorMethods<OwnershipPolicy>(tester);
        testMutatorSubscript<OwnershipPolicy>(tester);
    }
}

int main() {
    
    {
        Tester tester{"Vector3"};        
        VectorTest::testZeroInitialisation(tester);
        VectorTest::runTests<OwningVectorPolicy>(tester);
        tester.printReport(std::cout);
    }
    
    std::cout << std::endl;
    
    {
        Tester tester{"VectorSpan3"};
        VectorTest::runTests<SpanVectorPolicy>(tester);
        tester.printReport(std::cout);
    }
    
    return EXIT_SUCCESS;
}