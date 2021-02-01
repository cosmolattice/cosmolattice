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
        HeavisideStepFunction(const R& pR): UnaryOperator<R>(pR){}
        auto get(ptrdiff_t i)
        {
          return (GetValue::get(mR,i) >=0 ? 1. : 0);
        }
        /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
        template <typename U>
        inline auto d(const U& other)  {
            return GetDeriv::get(mR, other) * DiracDelta(mR);
        }

        virtual std::string operatorString() const {
            return "Heaviside";
        }

      };

      template<typename R>
      inline
      typename ConditionalUnaryGetter<HeavisideStepFunction, R>::type
      heaviside(const R& r)
      {
        return HeavisideStepFunction<R>(r);
      }
      /** \brief Specialize for possible zero input! */
      inline
      OneType heaviside( ZeroType a) {
          return OneType();
      }

      /** \brief Specialize for possible unit input! */
      inline
      OneType heaviside( OneType a) {
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
