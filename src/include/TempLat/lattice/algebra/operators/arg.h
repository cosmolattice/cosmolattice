#ifndef TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_ARG_H
#define TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_ARG_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/constants.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"

#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexfieldget.h"


namespace TempLat {
    namespace Operators {
        /** \brief A class to compute the argument of a complex field.
         *
         * Unit test: make test-arg
         **/
        template<typename R, typename T>
        class Arg : public BinaryOperator<R, T> {
        public:
            /* Put public methods here. These should change very little over time. */
            using BinaryOperator<R, T>::mR;
            using BinaryOperator<R, T>::mT;

            KOKKOS_FUNCTION
            Arg(R r, T t) : BinaryOperator<R, T>(r, t) {
            }

            KOKKOS_FUNCTION
            Arg() : BinaryOperator<R, T>(R(), T()) {
            }

            /** \brief Getter for two instances. */
            KOKKOS_FORCEINLINE_FUNCTION
            auto get(ptrdiff_t i) const {
#ifndef NOKOKKOS
                auto res = Kokkos::atan2(GetValue::get(mR, i), GetValue::get(mR, i));
                return AlmostEqual(res, 0) ? 0 : ((res > 0) ? res : res + 2 * Constants::pi<double>);
#else
                using namespace std; /* this way, for potential future data types. */
                auto res = atan2(GetValue::get(mR, i), GetValue::get(mR, i));
                return AlmostEqual(res, 0) ? 0 : ((res > 0) ? res : res + 2 * Constants::pi<double>);
#endif
            }

            /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template<typename U>
            void d(const U &other) = delete;

            static std::string operatorString() {
                return "arg";
            }
        };
    }

    struct ArgTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<typename R, typename T>
    KOKKOS_FORCEINLINE_FUNCTION
    typename std::enable_if<HasGetMethod<R>::value && HasGetMethod<T>::value, Operators::Arg<R, T> >::type
    arg(R r, T t) {
        return {r, t};
    }
} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/arg_test.h"
#endif

#endif
