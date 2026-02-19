#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_LAPLACIANLOCAL_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_LAPLACIANLOCAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/util/constexpr_for.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/util/tuple_tools.h"
#include "TempLat/lattice/algebra/helpers/haseval.h"

#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A class which implements the laplacian.
   *
   *
   * Unit test: ctest -R test-laplacianlocal
   **/
  template <ptrdiff_t NDim, typename R> class LatticeLaplacian : public UnaryOperator<R>
  {
  private:
    using UnaryOperator<R>::mR;

  public:
    using GetReturnType = typename GetGetReturnType<R>::type;
    using FloatType = typename GetFloatType<GetReturnType>::type;

    DEVICE_FUNCTION
    LatticeLaplacian(R pR) : UnaryOperator<R>(pR), dx2(pow(GetDx::getDx(pR), 2)) {}

    void doWeNeedGhosts() const { mR.confirmGhostsUpToDate(); }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, IDX... idx) {
        requires IsVariadicNDIndex<NDim, IDX...>;
        DoEval::eval(r, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      if constexpr (UnaryOperator<R>::getNDim() == 0)
        return ZeroType();
      else {
        auto result = (-2 * NDim * DoEval::eval(mR, idx...));
        constexpr_for<0, NDim>([&](const auto _d) {
          constexpr size_t d = decltype(_d)::value;
          device::apply([&](const auto &...shifted_idx) { result += DoEval::eval(mR, shifted_idx...); },
                        tuple_add_to_nth<d, 1>(device::tie(idx...)));
          device::apply([&](const auto &...shifted_idx) { result += DoEval::eval(mR, shifted_idx...); },
                        tuple_add_to_nth<d, -1>(device::tie(idx...)));
        });
        return result / dx2;
      }
    }

    virtual std::string operatorString() const override { return "Laplacian"; }

    /** @brief Symbolic derivatives. */
    template <typename S> DEVICE_FORCEINLINE_FUNCTION auto d(const S &other)
    {
      return LatticeLaplacian<NDim, R>(GetDeriv::get(mR, other));
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    const FloatType dx2;
  };

  template <int NDim, typename R>
    requires HasEvalMethod<R>
  DEVICE_FORCEINLINE_FUNCTION auto LatLapl(R pR)
  {
    return LatticeLaplacian<NDim, R>(pR);
  }

  template <int NDim, typename R>
    requires(!HasEvalMethod<R>)
  DEVICE_FORCEINLINE_FUNCTION auto LatLapl(R pR)
  {
    return ZeroType();
  }
} // namespace TempLat

#endif
