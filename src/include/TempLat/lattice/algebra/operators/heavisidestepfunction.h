#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_HEAVISIDESTEPFUNCTION_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_HEAVISIDESTEPFUNCTION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/diracdeltafunction.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"

namespace TempLat
{
  template <typename R> class HeavisideStepFunction : public UnaryOperator<R>
  {
  public:
    using UnaryOperator<R>::mR;

    DEVICE_FUNCTION
    HeavisideStepFunction(const R &pR) : UnaryOperator<R>(pR) {}

    template <typename... IDX>
      requires requires(IDX... idx) { GetValue::get(mR, idx...); }
    DEVICE_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
    {
      return (GetValue::get(mR, idx...) >= 0 ? 1 : 0);
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      return (DoEval::eval(mR, idx...) >= 0 ? 1 : 0);
    }

    /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
    template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
    {
      return GetDeriv::get(mR, other) * DiracDelta(mR);
    }

    virtual std::string operatorString() const override { return "Heaviside"; }
  };

  template <typename R>
    requires ConditionalUnaryGetter<R>
  DEVICE_FORCEINLINE_FUNCTION auto heaviside(const R &r)
  {
    return HeavisideStepFunction<R>(r);
  }

  /** @brief Specialize for possible zero input! */
  DEVICE_FORCEINLINE_FUNCTION
  OneType heaviside(ZeroType a) { return OneType(); }

  /** @brief Specialize for possible unit input! */
  DEVICE_FORCEINLINE_FUNCTION
  OneType heaviside(OneType a) { return OneType(); }

#ifdef TEMPLATTEST
  struct HeavisideStepFunctionTester {
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
