#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETSUM_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETSUM_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletbinaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

namespace TempLat
{
  /** @brief A class which implements the sum of 2 su2 doublets.
   *
   *
   * Unit test: ctest -R test-su2doubletsum
   **/
  template <typename R, typename T> class SU2DoubletAddition : public SU2DoubletBinaryOperator<R, T>
  {
  public:
    // Put public methods here. These should change very little over time.

    using SU2DoubletBinaryOperator<R, T>::mR;
    using SU2DoubletBinaryOperator<R, T>::mT;

    SU2DoubletAddition(const R &pR, const T &pT) : SU2DoubletBinaryOperator<R, T>(pR, pT) {}

    template <int N> DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<N> t) const
    {
      return mT.SU2DoubletGet(t) + mR.SU2DoubletGet(t);
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      auto cL = DoEval::eval(mR, idx...);
      auto cR = DoEval::eval(mT, idx...);
      device::array<decltype(cL[0] + cR[0]), 4> result;
      result[0] = cL[0] + cR[0];
      result[1] = cL[1] + cR[1];
      result[2] = cL[2] + cR[2];
      result[3] = cL[3] + cR[3];
      return result;
    }

    virtual std::string operatorString() const override { return "+"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  template <typename R, typename T>
    requires(HasSU2DoubletGet<R> && HasSU2DoubletGet<T>)
  auto operator+(const R &r, const T &t)
  {
    return SU2DoubletAddition<R, T>(r, t);
  }

#ifdef TEMPLATTEST
  struct SU2DoubletSumTester {
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
