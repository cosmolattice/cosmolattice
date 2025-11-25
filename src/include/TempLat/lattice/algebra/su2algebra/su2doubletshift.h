#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETSHIFT_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETSHIFT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/shift.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletunaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

namespace TempLat
{
  /** \brief A class which shifts a doublet in space.
   *
   *
   * Unit test: make test-su2shift
   **/
  template <typename R, int... N> class SU2DoubletShifter : public SU2DoubletUnaryOperator<R>
  {
  public:
    using SU2DoubletUnaryOperator<R>::mR;
    // Put public methods here. These should change very little over time.
    SU2DoubletShifter(const R &pR) : SU2DoubletUnaryOperator<R>(pR)
    {
      shiftString = shift<N...>(mR.SU2DoubletGet(0_c)).getString({N...});
    }

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<M> t) const
    {
      return shift<N...>(mR.SU2DoubletGet(t));
    }

    template <int M, typename... IDX>
      requires requires(R r, IDX... idx) { r.SU2DoubletGet(1_c, idx...); }
    DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<M> t, const IDX &...idx) const
    {
      return GetValue::get(shift<N...>(mR.SU2DoubletGet(t)), idx...);
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(SU2DoubletGet(0_c), idx...);
      DoEval::eval(SU2DoubletGet(1_c), idx...);
      DoEval::eval(SU2DoubletGet(2_c), idx...);
      DoEval::eval(SU2DoubletGet(3_c), idx...);
    }

    virtual std::string operatorString() const override { return shiftString; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::string shiftString;
  };

  template <typename R, int N> class SU2DoubletShifterByOne : public SU2DoubletUnaryOperator<R>
  {
    decltype(SU2DoubletWrap(
        shift<N>(std::declval<R>().SU2DoubletGet(0_c)), shift<N>(std::declval<R>().SU2DoubletGet(1_c)),
        shift<N>(std::declval<R>().SU2DoubletGet(2_c)), shift<N>(std::declval<R>().SU2DoubletGet(3_c)))) expr;

  public:
    using SU2DoubletUnaryOperator<R>::mR;
    // Put public methods here. These should change very little over time.
    SU2DoubletShifterByOne(const R &pR)
        : SU2DoubletUnaryOperator<R>(pR),
          expr(SU2DoubletWrap(shift<N>(pR.SU2DoubletGet(0_c)), shift<N>(pR.SU2DoubletGet(1_c)),
                              shift<N>(pR.SU2DoubletGet(2_c)), shift<N>(pR.SU2DoubletGet(3_c))))
    {
    }

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<M> t) const { return expr.SU2DoubletGet(t); }

    template <int M, typename... IDX>
      requires requires(R r, IDX... idx) { r.SU2DoubletGet(Tag<M>(), idx...); }
    DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<M> t, const IDX &...idx) const
    {
      return GetValue::get(expr.SU2DoubletGet(t), idx...);
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(SU2DoubletGet(0_c), idx...);
      DoEval::eval(SU2DoubletGet(1_c), idx...);
      DoEval::eval(SU2DoubletGet(2_c), idx...);
      DoEval::eval(SU2DoubletGet(3_c), idx...);
    }

    std::string toString() const { return GetString::get(mR) + "_(->" + std::to_string(N) + ")"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  template <int... shifts, class R>
    requires((sizeof...(shifts) > 1) && HasSU2DoubletGet<R>)
  auto shift(const R &pR)
  {
    return SU2DoubletShifter<R, shifts...>(pR);
  }

  template <int N, class R>
    requires HasSU2DoubletGet<R>
  auto shift(const R &pR)
  {
    return SU2DoubletShifterByOne<R, N>(pR);
  }
  template <int N, class R>
    requires(HasSU2DoubletGet<R>)
  auto shift(const R &pR, Tag<N> t)
  {
    return SU2DoubletShifterByOne<R, N>(pR);
  }

  struct SU2DoubletShiftTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif
