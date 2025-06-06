#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_HEAVISIDESTEPFUNCTION_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_HEAVISIDESTEPFUNCTION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/operators/diracdeltafunction.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"

#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"

namespace TempLat {
    template<typename R>
    class HeavisideStepFunction : public UnaryOperator<R> {
    public:
        using UnaryOperator<R>::mR;

        KOKKOS_FUNCTION
        HeavisideStepFunction(const R &pR): UnaryOperator<R>(pR) {
        }

        KOKKOS_FORCEINLINE_FUNCTION
        auto get(ptrdiff_t i) const {
            return (GetValue::get(mR, i) >= 0 ? 1. : 0);
        }

        /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
        template<typename U>
        KOKKOS_FORCEINLINE_FUNCTION
        auto d(const U &other) const {
            return GetDeriv::get(mR, other) * DiracDelta(mR);
        }

        static std::string operatorString() {
            return "Heaviside";
        }
    };

    template<typename R>
    KOKKOS_FORCEINLINE_FUNCTION
    typename ConditionalUnaryGetter<HeavisideStepFunction, R>::type
    heaviside(const R &r) {
        return HeavisideStepFunction<R>(r);
    }

    /** \brief Specialize for possible zero input! */
    KOKKOS_FORCEINLINE_FUNCTION
    OneType heaviside(ZeroType a) {
        return OneType();
    }

    /** \brief Specialize for possible unit input! */
    KOKKOS_FORCEINLINE_FUNCTION
    OneType heaviside(OneType a) {
        return OneType();
    }

    struct HeavisideStepFunctionTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/heavisidestepfunction_test.h"
#endif

#endif
