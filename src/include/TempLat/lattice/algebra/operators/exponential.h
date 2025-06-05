#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_EXPONENTIAL_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_EXPONENTIAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"

namespace TempLat {
    /** \brief Enable use of this operator without prefixing std:: or TempLat::. The compiler can distinguish between them. */
    using std::exp;

    /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
    namespace Operators {
        /** \brief A class which exponentiate a field.
         *
         * Unit test: make test-multiply
         **/
        template<typename T>
        class Exponential : public UnaryOperator<T> {
        public:
            /* Put public methods here. These should change very little over time. */
            using UnaryOperator<T>::mR;

            KOKKOS_FUNCTION
            Exponential(T a) : UnaryOperator<T>(a) {
            }

            /** \brief Getter for two instances. */
            KOKKOS_FORCEINLINE_FUNCTION
            auto get(ptrdiff_t i) {
                using namespace std; /* not std::exp, but this way, for potential future data types. */
                return exp(GetValue::get(mR, i));
            }

            /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template<typename U>
            KOKKOS_FORCEINLINE_FUNCTION
            auto d(const U &other) {
                return GetDeriv::get(mR, other) * *this;
            }

            static std::string operatorString() {
                return "exp";
            }
        };
    }

    /** \brief A mini struct for instiating the test case. */
    struct ExponentialTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    /** \brief Exposing our newly define exp operation to the world. */
    template<typename T>
    KOKKOS_FORCEINLINE_FUNCTION
    typename ConditionalUnaryGetter<Operators::Exponential, T>::type
    exp(T a) {
        return Operators::Exponential<T>(a);
    }

    /** \brief Specialize for possible zero input! */
    KOKKOS_FORCEINLINE_FUNCTION
    OneType exp(ZeroType a) {
        return OneType();
    }
} //

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/exponential_test.h"
#endif

#endif
