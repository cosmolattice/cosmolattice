#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2WRAPPER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2WRAPPER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2operator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/parallel/kokkos/kokkos.h"

namespace TempLat
{
  /** \brief A class which construct a SU2-like object.
   *
   *
   * Unit test: make test-su2wrapper
   **/
  template <class A, class B, class C, class D> class SU2Wrapper : public SU2Operator
  {
  public:
    using SV = typename GetGetReturnType<A>::type;

    /* Put public methods here. These should change very little over time. */
    SU2Wrapper() {}

    SU2Wrapper(const A &pA, const B &pB, const C &pC, const D &pD) : data(pA, pB, pC, pD) {}

    template <int N> KOKKOS_FORCEINLINE_FUNCTION auto SU2Get(Tag<N> t) const { return device::get<N>(data); }
    template <int N> KOKKOS_FORCEINLINE_FUNCTION auto operator()(Tag<N> t) const { return SU2Get(t); }

    template <int N, typename... IDX> struct RightIndices {
      static constexpr bool value =
          requires(device::tuple<A, B, C, D> t, IDX... idx) { GetValue::get(device::get<N>(t), idx...); };
    };

    template <int N, typename... IDX>
      requires RightIndices<N, IDX...>::value
    KOKKOS_FORCEINLINE_FUNCTION auto SU2Get(Tag<N> t, const IDX &...idx) const
    {
      return GetValue::get(device::get<N>(data), idx...);
    }

    template <typename... IDX>
      requires(RightIndices<0, IDX...>::value && RightIndices<1, IDX...>::value && RightIndices<2, IDX...>::value &&
               RightIndices<3, IDX...>::value)
    KOKKOS_FORCEINLINE_FUNCTION device::array<SV, 4> SU2Get(const IDX &...idx) const
    {
      return {SU2Get(0_c, idx...), SU2Get(1_c, idx...), SU2Get(2_c, idx...), SU2Get(3_c, idx...)};
    }

    std::string toString() const
    {
      return "SU2(" + GetString::get(device::get<0>(data)) + "," + GetString::get(device::get<1>(data)) + "," +
             GetString::get(device::get<2>(data)) + "," + GetString::get(device::get<3>(data)) + ")";
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    device::tuple<A, B, C, D> data;
  };

  struct SU2WrapperTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <class A, class B, class C, class D> auto SU2Wrap(A &&pA, B &&pB, C &&pC, D &&pD)
  {
    return SU2Wrapper<A, B, C, D>(pA, pB, pC, pD);
  }

  template <typename F> auto SU2Wrap(F &&f) { return SU2Wrap(f(0_c), f(1_c), f(2_c), f(3_c)); }

#define MakeSU2(a, expr) SU2Wrap([&](auto a) { return expr; })
} // namespace TempLat

#endif
