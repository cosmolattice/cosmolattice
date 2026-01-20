#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2SU2DOUBLETMULTIPLY_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2SU2DOUBLETMULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletbinaryoperator.h"
#include "TempLat/util/rangeiteration/for_in_range.h"

namespace TempLat
{
  /** \brief A class which implemetns SU2 SU2Doublet multiplication.
   *
   * Unit test: make test-su2su2doubletmultiply
   **/
  template <class R, class T> class SU2SU2DoubletMultiplication : public SU2DoubletBinaryOperator<R, T>
  {
  public:
    // Put public methods here. These should change very little over time.

    using SU2DoubletBinaryOperator<R, T>::mR;
    using SU2DoubletBinaryOperator<R, T>::mT;

    using SV = typename SU2GetGetReturnType<R>::type;

    SU2SU2DoubletMultiplication(const R &pR, const T &pT) : SU2DoubletBinaryOperator<R, T>(pR, pT) {}

    DEVICE_FORCEINLINE_FUNCTION
    auto SU2DoubletGet(Tag<0> t) const
    {
      return mR.SU2Get(0_c) * mT.SU2DoubletGet(0_c) + mR.SU2Get(2_c) * mT.SU2DoubletGet(2_c) -
             mR.SU2Get(3_c) * mT.SU2DoubletGet(1_c) - mR.SU2Get(1_c) * mT.SU2DoubletGet(3_c);
    }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2DoubletGet(Tag<1> t) const
    {
      return mR.SU2Get(0_c) * mT.SU2DoubletGet(1_c) + mR.SU2Get(3_c) * mT.SU2DoubletGet(0_c) +
             mR.SU2Get(2_c) * mT.SU2DoubletGet(3_c) + mR.SU2Get(1_c) * mT.SU2DoubletGet(2_c);
    }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2DoubletGet(Tag<2> t) const
    {
      return -mR.SU2Get(1_c) * mT.SU2DoubletGet(1_c) + mR.SU2Get(3_c) * mT.SU2DoubletGet(3_c) -
             mR.SU2Get(2_c) * mT.SU2DoubletGet(0_c) + mR.SU2Get(0_c) * mT.SU2DoubletGet(2_c);
    }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2DoubletGet(Tag<3> t) const
    {
      return -mR.SU2Get(2_c) * mT.SU2DoubletGet(1_c) + mR.SU2Get(1_c) * mT.SU2DoubletGet(0_c) +
             mR.SU2Get(0_c) * mT.SU2DoubletGet(3_c) - mR.SU2Get(3_c) * mT.SU2DoubletGet(2_c);
    }

    template <typename... IDX> struct RightIndices {
      static constexpr bool value = requires(R r, T t, IDX... idx) {
        r.SU2Get(0_c, idx...);
        r.SU2Get(1_c, idx...);
        r.SU2Get(2_c, idx...);
        r.SU2Get(3_c, idx...);

        t.SU2DoubletGet(0_c, idx...);
        t.SU2DoubletGet(1_c, idx...);
        t.SU2DoubletGet(2_c, idx...);
        t.SU2DoubletGet(3_c, idx...);
      };
    };

    template <int N, typename... IDX>
      requires RightIndices<IDX...>::value
    DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<N> t, const IDX &...idx) const
    {
      return cache[N];
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(mR, idx...);
      DoEval::eval(mT, idx...);

      device::array<SV, 4> cL;
      device::array<SV, 4> cR;

      constexpr_for<0, 4, 1>([&](auto j) {
        cL[j] = mR.SU2Get(j, idx...);
        cR[j] = mT.SU2DoubletGet(j, idx...);
      });

      cache[0] = cL[0] * cR[0] + cL[2] * cR[2] - cL[3] * cR[1] - cL[1] * cR[3];
      cache[1] = cL[0] * cR[1] + cL[3] * cR[0] + cL[2] * cR[3] + cL[1] * cR[2];
      cache[2] = -cL[1] * cR[1] + cL[3] * cR[3] - cL[2] * cR[0] + cL[0] * cR[2];
      cache[3] = -cL[2] * cR[1] + cL[1] * cR[0] + cL[0] * cR[3] - cL[3] * cR[2];
    }

    virtual std::string operatorString() const override { return "*"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    mutable device::array<SV, 4> cache;
  };

  template <typename R, typename T>
    requires(HasSU2Get<R> && HasSU2DoubletGet<T>)
  auto operator*(const R &r, const T &t)
  {
    return SU2SU2DoubletMultiplication<R, T>(r, t);
  }

#ifdef TEMPLATTEST
  struct SU2SU2DoubletMultiplyTester {
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
