
#ifndef TEMPLAT_LATTICE_ALGEBRA_SHIFT_H
#define TEMPLAT_LATTICE_ALGEBRA_SHIFT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"

#include "TempLat/util/tuple_size.h"
#include "TempLat/util/tuple_tools.h"

namespace TempLat
{
  /** @brief A class which implements shifts in coordinates.
   *
   * Unit test: ctest -R test-shift
   **/
  template <typename R, int... SHIFTS> class ExpressionShifter : public UnaryOperator<R>
  {
  public:
    // Put public methods here. These should change very little over time.
    using UnaryOperator<R>::mR;

    static constexpr size_t dim = sizeof...(SHIFTS);
    static constexpr auto shifts = device::make_tuple(SHIFTS...);

    DEVICE_FUNCTION
    ExpressionShifter(const R &pR) : UnaryOperator<R>(pR) {}

    template <typename... IDX>
      requires requires(std::decay_t<R> r, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      auto tup = device::tie(idx...);
      constexpr_for<0, dim>([&](const auto _d) {
        constexpr size_t d = decltype(_d)::value;
        tup = tuple_add_to_nth<d, device::get<d>(shifts)>(tup);
      });
      return device::apply([&](const auto &...shifted_idx) { return DoEval::eval(mR, shifted_idx...); }, tup);
    }

    void doWeNeedGhosts() const { mR.confirmGhostsUpToDate(); }

    virtual std::string operatorString() const override
    {
      std::string res = "_(";
      for (auto x : device::tie(SHIFTS...))
        res += std::to_string(x) + ",";
      res.pop_back();
      return res + ")";
    }
  };

  template <typename R, int _N> class ExpressionShifterByOne : public UnaryOperator<R>
  {
  public:
    static_assert(_N != 0, "_N cannot be 0.");

    static constexpr int N = _N > 0 ? _N : -_N;
    static constexpr int dir = _N > 0 ? 1 : -1;

    // Put public methods here. These should change very little over time.
    using UnaryOperator<R>::mR;

    DEVICE_FUNCTION
    ExpressionShifterByOne(const R &pR) : UnaryOperator<R>(pR) {}

    template <typename... IDX>
      requires requires(R r, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(mR, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      return device::apply([&](const auto &...shifted_idx) { return DoEval::eval(mR, shifted_idx...); },
                           tuple_add_to_nth<N - 1, dir>(device::tie(idx...)));
    }

    void doWeNeedGhosts() const { mR.confirmGhostsUpToDate(); }

    std::string toString() const { return GetString::get(mR) + "_(->" + std::to_string(N) + ")"; }
  };

  template <int... shifts, class R>
    requires((sizeof...(shifts) > 1) && tuple_size<R>::value == 1)
  DEVICE_FORCEINLINE_FUNCTION auto shift(const R &pR)
  {
    return ExpressionShifter<R, shifts...>(pR);
  }

  template <int N, class R>
    requires(tuple_size<R>::value == 1)
  DEVICE_FORCEINLINE_FUNCTION auto shift(const R &pR)
  {
    return ExpressionShifterByOne<R, N>(pR);
  }

  template <class R, int N>
    requires(tuple_size<R>::value == 1)
  DEVICE_FORCEINLINE_FUNCTION auto shift(const R &pR, Tag<N> t)
  {
    return ExpressionShifterByOne<R, N>(pR);
  }

  template <int N> DEVICE_FORCEINLINE_FUNCTION OneType shift(OneType) { return OneType(); }
  template <int N> DEVICE_FORCEINLINE_FUNCTION OneType shift(OneType, Tag<N>) { return OneType(); }

  template <int N> ZeroType shift(ZeroType) { return ZeroType(); }
  template <int N> ZeroType shift(ZeroType, Tag<N>) { return ZeroType(); }
} // namespace TempLat

#endif
