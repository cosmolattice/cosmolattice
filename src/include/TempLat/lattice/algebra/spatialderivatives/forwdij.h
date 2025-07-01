#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWDIJ_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWDIJ_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "templat/util/rangeiteration/tag.h"
#include "templat/util/tdd/tdd.h"
#include "templat/lattice/algebra/helpers/getdx.h"
#include "templat/util/constexpr_for.h"
#include "templat/lattice/algebra/operators/unaryoperator.h"
#include "templat/lattice/algebra/helpers/getderiv.h"
#include "templat/lattice/algebra/helpers/getgetreturntype.h"
#include "templat/lattice/algebra/helpers/getfloattype.h"
#include "templat/lattice/algebra/operators/operators.h"

namespace TempLat
{
  /** \brief A class which implements forward finite sums.
   *
   *
   * Unit test: make test-forwdij
   **/
  template <int dir, typename R> class ForwDij
  {
  public:
    using GetReturnType = typename GetGetReturnType<R>::type;
    using FloatType = typename GetFloatType<GetReturnType>::type;

    using UnaryOperator<R>::mR;

    KOKKOS_FUNCTION
    ForwDij(R pR) : UnaryOperator<R>(pR), dx(GetDx::getDx(pR)) {}

    void doWeNeedGhosts() { mR.confirmGhostsUpToDate(); }

    template <std::integral... IDX> KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
    {
      static_assert(dir > 0);
      constexpr size_t d = static_cast<size_t>(dir) - 1;
      auto result = GetValue::get(mR, idx...);
      std::apply([&](const auto &...shifted_idx) { result += GetValue::get(mR, shifted_idx...); },
                 tuple_add_to_nth<d>(std::tie(idx...), 1));
      return result / dx;
    }

    static std::string operatorString() { return "ForwDij"; }

    KOKKOS_FORCEINLINE_FUNCTION
    void eval(ptrdiff_t i)
    {
      // TODO (Franz)
      // DoEval::eval(mR, i);
      // for (size_t j = 0; ^ < 2 * NDim; ++j)
      //   DoEval::eval(mR, i + mShiftAccessors[j]);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    const FloatType dx;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  class ForwDijfTester
  {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <class R, int N>
    requires HasGetMethod<R>
  auto forwDij(R pR, Tag<N> t)
  {
    return ForwDij<N, R>(pR);
  }

  template <int NDim, typename R>
    requires(!HasGetMethod<R>)
  KOKKOS_FORCEINLINE_FUNCTION auto forwDij(R pR)
  {
    return ZeroType();
  }
} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/spatialderivatives/forwdiff_test.h"
#endif

#endif
