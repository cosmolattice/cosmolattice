#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2SHIFT_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2SHIFT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/operators/shift.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/lattice/algebra/su2algebra/su2operator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/su2algebra/su2unaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"

namespace TempLat
{
  /** @brief A class which applies the shift method to su2 objects.
   *
   * Unit test: ctest -R test-su2shift
   **/
  template <typename R, int... N> class SU2Shifter : public SU2UnaryOperator<R>
  {
  public:
    // Put public methods here. These should change very little over time.
    static constexpr size_t dim = sizeof...(N);
    static constexpr auto shifts = device::make_tuple(N...);

    using SU2UnaryOperator<R>::mR;

    SU2Shifter(const R &pR) : SU2UnaryOperator<R>(pR) {}

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<M> t) const { return shift<N...>(mR.SU2Get(t)); }

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

    virtual std::string operatorString() const override { return shift<N...>(mR.SU2Get(0_c)).getString({N...}); }
  };

  template <typename R, int _N> class SU2ShifterByOne : public SU2UnaryOperator<R>
  {
    static_assert(_N != 0, "_N cannot be 0.");

    static constexpr int N = _N > 0 ? _N : -_N;
    static constexpr int dir = _N > 0 ? 1 : -1;

  public:
    using SU2UnaryOperator<R>::mR;

    // Put public methods here. These should change very little over time.
    SU2ShifterByOne(const R &pR) : SU2UnaryOperator<R>(pR) {}

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<M> t) const { return shift<_N>(mR.SU2Get(t)); }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      return device::apply([&](const auto &...shifted_idx) { return DoEval::eval(mR, shifted_idx...); },
                           tuple_add_to_nth<N - 1, dir>(device::tie(idx...)));
    }

    std::string toString() const { return GetString::get(mR) + "_(->" + std::to_string(N) + ")"; }
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
} // namespace TempLat

#endif
