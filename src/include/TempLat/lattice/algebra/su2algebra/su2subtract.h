#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2SUBTRACT_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2SUBTRACT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/lattice/algebra/su2algebra/su2binaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/memory/memorytoolbox.h"

namespace TempLat
{
  /** @brief A class which implements the SU2 Subtraction.
   *
   * Unit test: ctest -R test-su2sum
   **/
  template <typename R, typename T> class SU2Subtraction : public SU2BinaryOperator<R, T>
  {
  public:
    // Put public methods here. These should change very little over time.

    using SU2BinaryOperator<R, T>::mR;
    using SU2BinaryOperator<R, T>::mT;

    using SV = typename SU2GetGetReturnType<R>::type;

    SU2Subtraction(const R &pR, const T &pT) : SU2BinaryOperator<R, T>(pR, pT) {}

    template <int N> DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<N> t) const { return mR.SU2Get(t) - mT.SU2Get(t); }

    template <int N> DEVICE_FORCEINLINE_FUNCTION auto operator()(Tag<N> t) const { return SU2Get(t); }

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
    requires(HasSU2Get<R> && HasSU2Get<T>)
  auto operator-(const R &r, const T &t)
  {
    return SU2Subtraction{r, t};
  }
} // namespace TempLat

#endif
