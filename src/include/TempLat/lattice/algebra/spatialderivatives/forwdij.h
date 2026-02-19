#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWDIJ_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWDIJ_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/util/constexpr_for.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/util/tuple_tools.h"
#include "TempLat/lattice/algebra/helpers/haseval.h"

namespace TempLat
{
  /** @brief A class which implements forward finite sums.
   *
   *
   * Unit test: ctest -R test-forwdij
   **/
  template <int dir, typename R> class ForwDij : public UnaryOperator<R>
  {
  public:
    using GetReturnType = typename GetGetReturnType<R>::type;
    using FloatType = typename GetFloatType<GetReturnType>::type;

    using UnaryOperator<R>::mR;

    DEVICE_FUNCTION
    ForwDij(R pR) : UnaryOperator<R>(pR), dx(GetDx::getDx(pR)) {}

    void doWeNeedGhosts() const { mR.confirmGhostsUpToDate(); }

    virtual std::string operatorString() const override { return "ForwDij"; }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      if constexpr (UnaryOperator<R>::getNDim() == 0)
        return ZeroType();
      else {
        static_assert(dir > 0);
        constexpr size_t d = static_cast<size_t>(dir) - 1;
        auto result = DoEval::eval(mR, idx...);
        device::apply([&](const auto &...shifted_idx) { result += DoEval::eval(mR, shifted_idx...); },
                      tuple_add_to_nth<d, 1>(device::tie(idx...)));
        return result / dx;
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    const FloatType dx;
  };

  template <class R, int N>
    requires HasEvalMethod<R>
  DEVICE_FORCEINLINE_FUNCTION auto forwDij(R pR, Tag<N> t)
  {
    return ForwDij<N, R>(pR);
  }

  template <int NDim, typename R>
    requires(!HasEvalMethod<R>)
  DEVICE_FORCEINLINE_FUNCTION auto forwDij(R pR)
  {
    return ZeroType();
  }
} // namespace TempLat

#endif
