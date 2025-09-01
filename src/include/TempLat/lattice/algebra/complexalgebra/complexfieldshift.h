#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDSHIFT_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDSHIFT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/shift.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldunaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"

namespace TempLat
{
  /** \brief A class which implements spatial shifts for complex algebra.
   *
   *
   * Unit test: make test-complexfieldshift
   **/
  template <typename R, int... N> class ComplexFieldShifter : public ComplexFieldUnaryOperator<R>
  {
  public:
    /* Put public methods here. These should change very little over time. */

    using ComplexFieldUnaryOperator<R>::mR;

    ComplexFieldShifter(const R &pR) : ComplexFieldUnaryOperator<R>(pR)
    {
      shiftString = shift<N...>(mR.ComplexFieldGet(0_c)).getString({N...});
    }

    template <int M> KOKKOS_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<M> t) const
    {
      return shift<N...>(mR.ComplexFieldGet(t));
    }

    template <int M, typename... IDX>
      requires requires(R mR, IDX... idx) {
        requires VariadicIndex<IDX...>;
        mR.ComplexFieldGet(Tag<M>());
      }
    KOKKOS_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<M> t, const IDX &...idx) const
    {
      return GetValue::get(shift<N...>(mR.ComplexFieldGet(t)), idx...);
    }

    template <int M, typename... IDX>
      requires VariadicIndex<IDX...>
    KOKKOS_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(ComplexFieldGet(0_c), idx...);
      DoEval::eval(ComplexFieldGet(1_c), idx...);
    }

    std::string operatorString() const { return shiftString; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::string shiftString;
  };
  template <typename R, int N> class ComplexFieldShifterByOne : public ComplexFieldUnaryOperator<R>
  {
  public:
    /* Put public methods here. These should change very little over time. */

    using ComplexFieldUnaryOperator<R>::mR;

    ComplexFieldShifterByOne(const R &pR) : ComplexFieldUnaryOperator<R>(pR) {}

    template <int M> KOKKOS_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<M> t) const
    {
      return shift<N>(mR.ComplexFieldGet(t));
    }

    template <int M, typename... IDX>
      requires VariadicIndex<IDX...>
    KOKKOS_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<M> t, const IDX &...idx) const
    {
      return shift<N>(mR.ComplexFieldGet(t)).get(idx...);
    }

    template <int M, typename... IDX>
      requires VariadicIndex<IDX...>
    KOKKOS_FORCEINLINE_FUNCTION void eval(const IDX &...idx)
    {
      DoEval::eval(ComplexFieldGet(0_c), idx...);
      DoEval::eval(ComplexFieldGet(1_c), idx...);
    }

    std::string toString() const { return GetString::get(mR) + "(->" + std::to_string(N) + ")"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  template <int... shifts, class R>
    requires((sizeof...(shifts) > 1) && HasComplexFieldGet<R>)
  auto shift(const R &pR)
  {
    return ComplexFieldShifter<R, shifts...>(pR);
  }

  template <int N, class R>
    requires HasComplexFieldGet<R>
  auto shift(const R &pR)
  {
    return ComplexFieldShifterByOne<R, N>(pR);
  }

  template <class R, int N>
    requires HasComplexFieldGet<R>
  auto shift(const R &pR, Tag<N> t)
  {
    return ComplexFieldShifterByOne<R, N>(pR);
  }

  struct ComplexFieldShiftTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif