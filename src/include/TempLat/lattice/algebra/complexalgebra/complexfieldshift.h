#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDSHIFT_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDSHIFT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/shift.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldunaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"

namespace TempLat
{
  /** @brief A class which implements spatial shifts for complex algebra.
   *
   *
   * Unit test: ctest -R test-complexfieldshift
   **/
  template <typename R, int... N> class ComplexFieldShifter : public ComplexFieldUnaryOperator<R>
  {
  public:
    // Put public methods here. These should change very little over time.
    static constexpr size_t dim = sizeof...(N);
    static constexpr auto shifts = device::make_tuple(N...);

    using ComplexFieldUnaryOperator<R>::mR;

    ComplexFieldShifter(const R &pR) : ComplexFieldUnaryOperator<R>(pR)
    {
      shiftString = shift<N...>(mR.ComplexFieldGet(0_c)).getString({N...});
    }

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<M> t) const
    {
      return shift<N...>(mR.ComplexFieldGet(t));
    }

    template <int M, typename... IDX>
      requires requires(R mR, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        mR.ComplexFieldGet(Tag<M>());
      }
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<M> t, const IDX &...idx) const
    {
      auto tup = device::tie(idx...);
      constexpr_for<0, dim, 1>([&](const auto _d) {
        constexpr size_t d = decltype(_d)::value;
        tup = tuple_add_to_nth<d, device::get<d>(shifts)>(tup);
      });
      return device::apply([&](const auto &...shifted_idx) { return mR.ComplexFieldGet(t, shifted_idx...); }, tup);
    }

    template <int M, typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      auto tup = device::tie(idx...);
      constexpr_for<0, dim, 1>([&](const auto _d) {
        constexpr size_t d = decltype(_d)::value;
        tup = tuple_add_to_nth<d, device::get<d>(shifts)>(tup);
      });
      device::apply([&](const auto &...shifted_idx) { DoEval::eval(mR, shifted_idx...); }, tup);
    }

    std::string operatorString() const { return shiftString; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::string shiftString;
  };
  template <typename R, int _N> class ComplexFieldShifterByOne : public ComplexFieldUnaryOperator<R>
  {
    static_assert(_N != 0, "_N cannot be 0.");

    static constexpr int N = _N > 0 ? _N : -_N;
    static constexpr int dir = _N > 0 ? 1 : -1;

  public:
    // Put public methods here. These should change very little over time.

    using ComplexFieldUnaryOperator<R>::mR;

    ComplexFieldShifterByOne(const R &pR) : ComplexFieldUnaryOperator<R>(pR) {}

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<M> t) const
    {
      return shift<N>(mR.ComplexFieldGet(t));
    }

    template <int M, typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<M> t, const IDX &...idx) const
    {
      return device::apply([&](const auto &...shifted_idx) { return mR.ComplexFieldGet(t, shifted_idx...); },
                           tuple_add_to_nth<N - 1, dir>(device::tie(idx...)));
    }

    template <int M, typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx)
    {
      device::apply([&](const auto &...shifted_idx) { DoEval::eval(mR, shifted_idx...); },
                    tuple_add_to_nth<N - 1, 1>(device::tie(idx...)));
    }

    std::string toString() const { return GetString::get(mR) + "(->" + std::to_string(N) + ")"; }
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

#ifdef TEMPLATTEST
  struct ComplexFieldShiftTester {
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif