#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETDOTTER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETDOTTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"

#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldoperator.h"
#include "TempLat/lattice/algebra/operators/squareroot.h"

#include "TempLat/lattice/algebra/complexalgebra/complexfieldbinaryoperator.h"

namespace TempLat
{

  /** \brief A class which computes the scalar product of two doublets.
   *
   *
   * Unit test: make test-su2doubletdotter
   **/
  template <typename R, typename T> class SU2DoubletDotter : public ComplexFieldBinaryOperator<R, T>
  {
  public:
    /* Put public methods here. These should change very little over time. */

    using ComplexFieldBinaryOperator<R, T>::mR;
    using ComplexFieldBinaryOperator<R, T>::mT;

    SU2DoubletDotter(const R &pR, const T &pT) : ComplexFieldBinaryOperator<R, T>(pR, pT) {}

    auto ComplexFieldGet(Tag<0> t)
    {
      return mR.SU2DoubletGet(0_c) * mT.SU2DoubletGet(0_c) + mR.SU2DoubletGet(1_c) * mT.SU2DoubletGet(1_c) +
             mR.SU2DoubletGet(2_c) * mT.SU2DoubletGet(2_c) + mR.SU2DoubletGet(3_c) * mT.SU2DoubletGet(3_c);
    }
    auto ComplexFieldGet(Tag<1> t)
    {
      return mR.SU2DoubletGet(0_c) * mT.SU2DoubletGet(1_c) - mR.SU2DoubletGet(1_c) * mT.SU2DoubletGet(0_c) +
             mR.SU2DoubletGet(2_c) * mT.SU2DoubletGet(3_c) - mR.SU2DoubletGet(3_c) * mT.SU2DoubletGet(2_c);
    }

    template <typename... IDX>
      requires VariadicIndex<IDX...>
    KOKKOS_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<0> t, const IDX &...idx) const
    {
      return mR.SU2DoubletGet(0_c, idx...) * mT.SU2DoubletGet(0_c, idx...) +
             mR.SU2DoubletGet(1_c, idx...) * mT.SU2DoubletGet(1_c, idx...) +
             mR.SU2DoubletGet(2_c, idx...) * mT.SU2DoubletGet(2_c, idx...) +
             mR.SU2DoubletGet(3_c, idx...) * mT.SU2DoubletGet(3_c, idx...);
    }

    template <typename... IDX>
      requires VariadicIndex<IDX...>
    KOKKOS_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<1> t, const IDX &...idx) const
    {
      return mR.SU2DoubletGet(0_c, idx...) * mT.SU2DoubletGet(1_c, idx...) -
             mR.SU2DoubletGet(1_c, idx...) * mT.SU2DoubletGet(0_c, idx...) +
             mR.SU2DoubletGet(2_c, idx...) * mT.SU2DoubletGet(3_c, idx...) -
             mR.SU2DoubletGet(3_c, idx...) * mT.SU2DoubletGet(2_c, idx...);
    }

    template <typename... IDX>
      requires VariadicIndex<IDX...>
    KOKKOS_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(mR, idx...);
      DoEval::eval(mT, idx...);
    }

    std::string toString() const
    {
      std::string tt = GetString::get(mR);

      if (ContainsSpace::test(tt)) tt = "(" + tt + ")";

      std::string ss = GetString::get(mT);

      if (ContainsSpace::test(ss)) ss = "(" + ss + ")";

      std::string result = "(" + tt + "," + ss + ")";

      return result;
    }
  };

  struct SU2DoubletDotterTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <class R, class T>
    requires(HasSU2DoubletGet<R> && HasSU2DoubletGet<T>)
  auto scalar_prod(const R &r, const T &t)
  {
    return SU2DoubletDotter{r, t};
  }

  template <class R>
    requires HasSU2DoubletGet<R>
  auto scalar_prod(const R &r, ZeroType t)
  {
    return t;
  }

  template <class R>
    requires HasSU2DoubletGet<R>
  auto scalar_prod(ZeroType t, const R &r)
  {
    return t;
  }

  template <typename R>
    requires HasSU2DoubletGet<R>
  auto norm2(const R &r)
  {
    return Real(scalar_prod(r, r));
  }
} // namespace TempLat

#endif
