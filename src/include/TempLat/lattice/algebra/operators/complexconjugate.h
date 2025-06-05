#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COMPLEXCONJUGATE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COMPLEXCONJUGATE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <complex>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"

namespace TempLat {
    /** \brief Enable use of this operator without prefixing std:: or TempLat::.
     * The compiler can distinguish between them. */
#ifndef NOKOKKOS
    using Kokkos::conj;
#else
    using std::conj;
#endif

    namespace Operators {
        /** \brief A class which applies complex conjugation to a complex number.
         * Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
         *
         * Unit test: make test-multiply
         **/
        template<typename R>
        class ComplexConjugate : public UnaryOperator<R> {
        public:
            /* Put public methods here. These should change very little over time. */
            using UnaryOperator<R>::mR;

            KOKKOS_FUNCTION
            ComplexConjugate(const R &a) : UnaryOperator<R>(a) {
            }

            /** \brief Getter for two instances. */
            KOKKOS_FORCEINLINE_FUNCTION
            auto get(ptrdiff_t i) {
#ifndef NOKOKKOS
                return Kokkos::conj(GetValue::get(mR, i));
#else
                return conj(GetValue::get(mR, i));
#endif
            }

            /** \brief Complex conjugation and copmlex differentiation aren't friends. */
            template<typename U>
            KOKKOS_FORCEINLINE_FUNCTION
            auto d(const U &other) = delete;
        };
    }

    /** \brief A mini struct for instiating the test case. */
    struct ComplexConjugateTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    /** \brief Exposing our newly define multiplication operation to the world. */
    template<typename T>
    KOKKOS_FORCEINLINE_FUNCTION
    typename ConditionalUnaryGetter<Operators::ComplexConjugate, T>::type
    conj(const T &a) {
        return Operators::ComplexConjugate<T>(a);
    }
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/complexconjugate_test.h"
#endif

#endif
