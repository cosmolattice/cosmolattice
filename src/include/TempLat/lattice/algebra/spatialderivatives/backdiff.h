#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_BACKDIFF_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_BACKDIFF_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/util/constexpr_for.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/operators/operators.h"

namespace TempLat
{
  /** \brief A class which implements backward finite differences.
   *
   *
   * Unit test: make test-backdiff
   **/
  template <int dir, typename R> class BackDiff
  {
  public:
    using GetReturnType = typename GetGetReturnType<R>::type;
    using FloatType = typename GetFloatType<GetReturnType>::type;

    using UnaryOperator<R>::mR;

    KOKKOS_FUNCTION
    BackDiff(R pR) : UnaryOperator<R>(pR), dx(GetDx::getDx(pR)) {}

    void doWeNeedGhosts() { mR.confirmGhostsUpToDate(); }

    template <typename... IDX>
      requires VariadicIndex<IDX...>
    KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
    {
      static_assert(dir > 0);
      constexpr size_t d = static_cast<size_t>(dir) - 1;
      auto result = GetValue::get(mR, idx...);
      std::apply([&](const auto &...shifted_idx) { result -= GetValue::get(mR, shifted_idx...); },
                 tuple_add_to_nth<d>(std::tie(idx...), -1));
      return result / dx;
    }

    static std::string operatorString() { return "BackDiff"; }

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

  class BackDiffTester
  {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <class R, int N>
    requires HasGetMethod<R>
  auto backDiff(R pR, Tag<N> t)
  {
    return BackDiff<N, R>(pR);
  }

  template <int NDim, typename R>
    requires(!HasGetMethod<R>)
  KOKKOS_FORCEINLINE_FUNCTION auto backDiff(R pR)
  {
    return ZeroType();
  }
} // namespace TempLat

#endif
