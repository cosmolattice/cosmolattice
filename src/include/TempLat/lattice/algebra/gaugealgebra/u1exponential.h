#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_U1WRAPPER_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_U1WRAPPER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldunaryoperator.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexgetgetreturntype.h"
#include <Kokkos_Macros.hpp>

namespace TempLat
{
  /** \brief Exponential map for U(1).
   *
   *
   * Unit test: make test-u1wrapper
   **/
  template <typename R> class U1Exponential : public ComplexFieldUnaryOperator<R>
  {
  public:
    using SV = typename GetGetReturnType<R>::type;
    // Put public methods here. These should change very little over time.
    using ComplexFieldUnaryOperator<R>::mR;

    U1Exponential(const R &pR) : ComplexFieldUnaryOperator<R>(pR) {}

    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<0> t) const { return cos(mR); }
    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<1> t) const { return sin(mR); }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<0> t, const IDX &...idx) const
    {
      return mCacheRe;
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<1> t, const IDX &...idx) const
    {
      return mCacheIm;
      // return sin(GetEval::getEval(mR, idx...));
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(mR, idx...);
      const SV tmp = GetEval::getEval(mR, idx...);
      mCacheRe = cos(tmp);
      mCacheIm = sin(tmp);
    }

    std::string toString() const { return "U1(" + GetString::get(mR) + ")"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    mutable SV mCacheRe;
    mutable SV mCacheIm;
  };

  struct U1ExponentialTester {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename R> auto complexPhase(R &&r) { return U1Exponential<R>(std::forward<R>(r)); }
} // namespace TempLat

#endif
