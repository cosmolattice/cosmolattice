#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SINE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SINE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/cosine.h"

namespace TempLat {
    using std::sin;

    /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
    namespace Operators {
        /** \brief A class which applies cosine.
         *
         * Unit test: make test-multiply
         **/
        template<typename T>
        class Sine : public UnaryOperator<T> {
        public:
            /* Put public methods here. These should change very little over time. */
            using UnaryOperator<T>::mR;

            KOKKOS_FUNCTION
            Sine(const T& a) : UnaryOperator<T>(a) {
            }

            /** \brief Getter for two instances. */
            KOKKOS_FORCEINLINE_FUNCTION
            auto get(ptrdiff_t i) const {
#ifndef NOKOKKOS
                return Kokkos::sin(GetValue::get(mR, i));
#else
                using namespace std;
                return sin(GetValue::get(mR, i));
#endif
            }

            /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template<typename U>
            KOKKOS_FORCEINLINE_FUNCTION
            auto d(const U &other) const {
                return GetDeriv::get(mR, other) * cos(mR);
            }

            static std::string operatorString() {
                return "sin";
            }
        };
    }

    /** \brief A mini struct for instiating the test case. */
    struct SineTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    /** \brief Exposing our newly define exp operation to the world. */
    template<typename T>
    KOKKOS_FORCEINLINE_FUNCTION
    typename ConditionalUnaryGetter<Operators::Sine, T>::type
    sin(T a) {
        return Operators::Sine<T>(a);
    }

    /** \brief Specialize for possible zero input! */
    KOKKOS_FORCEINLINE_FUNCTION
    ZeroType sin(ZeroType a) {
        return ZeroType();
    }
} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/sine_test.h"
#endif

#endif
