#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETSUBTRACT_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETSUBTRACT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletbinaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include <Kokkos_Macros.hpp>

namespace TempLat
{
  /** \brief A class which implement the Subtraction between two su2 doublets.
   *
   *
   * Unit test: make test-su2doubletsubtract
   **/
  template <typename R, typename T> class SU2DoubletSubtract : public SU2DoubletBinaryOperator<R, T>
  {
  public:
    using SU2DoubletBinaryOperator<R, T>::mR;
    using SU2DoubletBinaryOperator<R, T>::mT;

    /* Put public methods here. These should change very little over time. */
    SU2DoubletSubtract(const R &pR, const T &pT) : SU2DoubletBinaryOperator<R, T>(pR, pT) {}

    template <int N> KOKKOS_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<N> t) const
    {
      return mR.SU2DoubletGet(t) - mT.SU2DoubletGet(t);
    }
    template <int N, typename... IDX>
      requires requires(R r, T t, IDX... idx) {
        r.SU2DoubletGet(1_c, idx...);
        t.SU2DoubletGet(1_c, idx...);
      }
    KOKKOS_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<N> t, const IDX &...idx) const
    {
      return mR.SU2DoubletGet(t, idx...) - mT.SU2DoubletGet(t, idx...);
    }

    template <typename... IDX>
      requires VariadicIndex<IDX...>
    KOKKOS_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(mR, idx...);
      DoEval::eval(mT, idx...);
    }

    virtual std::string operatorString() const override { return "-"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  struct SU2DoubletSubtractTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename R, typename T>
    requires(HasSU2DoubletGet<R> && HasSU2DoubletGet<T>)
  auto operator-(const R &r, const T &t)
  {
    return SU2DoubletSubtract<R, T>(r, t);
  }
} // namespace TempLat

#endif
