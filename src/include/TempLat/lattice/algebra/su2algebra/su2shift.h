#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2SHIFT_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2SHIFT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/shift.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/lattice/algebra/su2algebra/su2operator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/su2algebra/su2unaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"

namespace TempLat
{
  /** \brief A class which applies the shift method to su2 objects.
   *
   * Unit test: make test-su2shift
   **/
  template <typename R, int... N> class SU2Shifter : public SU2UnaryOperator<R>
  {
  public:
    // Put public methods here. These should change very little over time.

    using SV = typename SU2GetGetReturnType<R>::type;
    using SU2UnaryOperator<R>::mR;

    SU2Shifter(const R &pR) : SU2UnaryOperator<R>(pR) { shiftString = shift<N...>(mR.SU2Get(0_c)).getString({N...}); }

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<M> t) const { return shift<N...>(mR.SU2Get(t)); }

    template <int M, typename... IDX> struct RightIndices {
      static constexpr bool value = requires(R r, IDX... idx) {
        GetValue::get(r.SU2Get(0_c), idx...);
        GetValue::get(r.SU2Get(1_c), idx...);
        GetValue::get(r.SU2Get(2_c), idx...);
        GetValue::get(r.SU2Get(3_c), idx...);
      };
    };

    template <int M, typename... IDX>
      requires RightIndices<M, IDX...>::value
    DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<M> t, const IDX &...idx) const
    {
      return GetValue::get(shift<N...>(mR.SU2Get(t)), idx...);
    }

    template <typename... IDX>
      requires(RightIndices<0, IDX...>::value && RightIndices<1, IDX...>::value && RightIndices<2, IDX...>::value &&
               RightIndices<3, IDX...>::value)
    DEVICE_FORCEINLINE_FUNCTION device::array<SV, 4> SU2Get(const IDX &...idx) const
    {
      return {{SU2Get(0_c, idx...), SU2Get(1_c, idx...), SU2Get(2_c, idx...), SU2Get(3_c, idx...)}};
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(shift<N...>(mR.SU2Get(1_c)), idx...);
      DoEval::eval(shift<N...>(mR.SU2Get(2_c)), idx...);
      DoEval::eval(shift<N...>(mR.SU2Get(3_c)), idx...);
    }

    virtual std::string operatorString() const override { return shiftString; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::string shiftString;
  };

  template <typename R, int N> class SU2ShifterByOne : public SU2UnaryOperator<R>
  {
  public:
    typedef typename SU2GetGetReturnType<R>::type SV;
    using SU2UnaryOperator<R>::mR;

    // Put public methods here. These should change very little over time.
    SU2ShifterByOne(const R &pR) : SU2UnaryOperator<R>(pR) {}

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<M> t) const { return shift<N>(mR.SU2Get(t)); }

    template <int M, typename... IDX> struct RightIndices {
      static constexpr bool value = requires(R r, IDX... idx) { GetValue::get(r.SU2Get(Tag<M>()), idx...); };
    };

    template <int M, typename... IDX>
      requires RightIndices<M, IDX...>::value
    DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<M> t, const IDX &...idx) const
    {
      return GetValue::get(shift<N>(mR.SU2Get(t)), idx...);
    }

    template <int Q> DEVICE_FORCEINLINE_FUNCTION const auto &operator()(Tag<Q> t) const { return SU2Get(t); }

    template <typename... IDX>
      requires(RightIndices<0, IDX...>::value && RightIndices<1, IDX...>::value && RightIndices<2, IDX...>::value &&
               RightIndices<3, IDX...>::value)
    DEVICE_FORCEINLINE_FUNCTION device::array<SV, 4> SU2Get(const IDX &...idx) const
    {
      return {SU2Get(0_c, idx...), SU2Get(1_c, idx...), SU2Get(2_c, idx...), SU2Get(3_c, idx...)};
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(shift<N>(mR.SU2Get(1_c)), idx...);
      DoEval::eval(shift<N>(mR.SU2Get(2_c)), idx...);
      DoEval::eval(shift<N>(mR.SU2Get(3_c)), idx...);
    }

    std::string toString() const { return GetString::get(mR) + "_(->" + std::to_string(N) + ")"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  template <int... shifts, class R>
    requires((sizeof...(shifts) > 1) && HasSU2Get<R>)
  auto shift(const R &pR)
  {
    return SU2Shifter<R, shifts...>(pR);
  }

  template <int N, class R>
    requires HasSU2Get<R>
  auto shift(const R &pR)
  {
    return SU2ShifterByOne<R, N>(pR);
  }

  template <class R, int N>
    requires(HasSU2Get<R>)
  auto shift(const R &pR, Tag<N> t)
  {
    return SU2ShifterByOne<R, N>(pR);
  }

  template <class R> auto sh1(const R &pR) { return pR; }

  template <int I, int... J, class R> auto sh1(const R &pR) { return shift<I>(sh1<J...>(pR)); }

#ifdef TEMPLATTEST
  struct SU2ShiftTester {
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
