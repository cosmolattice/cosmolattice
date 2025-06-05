#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_TANH_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_TANH_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/sinh.h"

namespace TempLat {
    namespace Operators {
        /** \brief A class which implements the Tanh.
         *
         * Unit test: make test-tanh
         **/
        template<typename T>
        class Tanh : public UnaryOperator<T> {
        public:
            /* Put public methods here. These should change very little over time. */
            using UnaryOperator<T>::mR;

            KOKKOS_FUNCTION
            Tanh(T a) : UnaryOperator<T>(a) {
            }

            /** \brief Getter for two instances. */
            KOKKOS_FORCEINLINE_FUNCTION
            auto get(ptrdiff_t i) {
                using namespace std; /* not std::exp, but this way, for potential future data types. */
                return tanh(GetValue::get(mR, i));
            }

            /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template<typename U>
            KOKKOS_FORCEINLINE_FUNCTION
            auto d(const U &other) {
                return GetDeriv::get(mR, other) / pow<2>(sinh(*this));
            }

            static std::string operatorString() {
                return "tanh";
            }
        };
    }

    /** \brief A mini struct for instiating the test case. */
    struct TanhTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    /** \brief Exposing our newly define exp operation to the world. */
    template<typename T>
    KOKKOS_FORCEINLINE_FUNCTION
    typename ConditionalUnaryGetter<Operators::Tanh, T>::type
    tanh(T a) {
        return Operators::Tanh<T>(a);
    }

    /** \brief Specialize for possible zero input! */
    KOKKOS_FORCEINLINE_FUNCTION
    ZeroType tanh(ZeroType a) {
        return ZeroType();
    }
} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/tanh_test.h"
#endif

#endif
