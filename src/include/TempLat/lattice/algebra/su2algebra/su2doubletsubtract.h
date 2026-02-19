#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETSUBTRACT_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETSUBTRACT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletbinaryoperator.h"
#include "TempLat/lattice/algebra/su2algebra/su2subtract.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

namespace TempLat
{
  /** @brief A class which implement the Subtraction between two su2 doublets.
   *
   *
   * Unit test: ctest -R test-su2doubletsubtract
   **/
  template <typename R, typename T> class SU2DoubletSubtract : public SU2DoubletBinaryOperator<R, T>
  {
  public:
    using SU2DoubletBinaryOperator<R, T>::mR;
    using SU2DoubletBinaryOperator<R, T>::mT;

    // Put public methods here. These should change very little over time.
    SU2DoubletSubtract(const R &pR, const T &pT) : SU2DoubletBinaryOperator<R, T>(pR, pT) {}

    template <int N> DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<N> t) const
    {
      return mR.SU2DoubletGet(t) - mT.SU2DoubletGet(t);
    }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
        DoEval::eval(t, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      auto cL = DoEval::eval(mR, idx...);
      const auto cR = DoEval::eval(mT, idx...);
      cL[0] -= cR[0];
      cL[1] -= cR[1];
      cL[2] -= cR[2];
      cL[3] -= cR[3];
      return cL;
    }

    virtual std::string operatorString() const override { return "-"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  template <typename R, typename T>
    requires(HasSU2DoubletGet<R> && HasSU2DoubletGet<T>)
  auto operator-(const R &r, const T &t)
  {
    return SU2DoubletSubtract<R, T>(r, t);
  }
} // namespace TempLat

#endif
