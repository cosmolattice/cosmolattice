#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_LAPLACIANLOCAL_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_LAPLACIANLOCAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/util/constexpr_for.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/operators/operators.h"

#include "TempLat/parallel/device.h"

#include "TempLat/util/tuple_tools.h"

namespace TempLat
{
  /** \brief A class which implements the laplacian.
   *
   *
   * Unit test: make test-laplacianlocal
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

    void doWeNeedGhosts() { mR.confirmGhostsUpToDate(); }

    template <typename... IDX>
      requires requires(IDX... idx) {
        requires IsVariadicNDIndex<NDim, IDX...>;
        GetValue::get(mR, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
    {
      if constexpr (UnaryOperator<R>::getNDim() == 0)
        return ZeroType();
      else {
        auto result = (-2 * NDim * GetValue::get(mR, idx...));
        constexpr_for<0, NDim, 1>([&](const auto _d) {
          constexpr size_t d = decltype(_d)::value;
          device::apply([&](const auto &...shifted_idx) { result += GetValue::get(mR, shifted_idx...); },
                        tuple_add_to_nth<d>(device::tie(idx...), 1));
          device::apply([&](const auto &...shifted_idx) { result += GetValue::get(mR, shifted_idx...); },
                        tuple_add_to_nth<d>(device::tie(idx...), -1));
        });
        return result / dx2;
      }
    }

    virtual std::string operatorString() const override { return "Laplacian"; }

    template <typename... IDX>
      requires requires(R r, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(mR, idx...);
      constexpr_for<0, NDim, 1>([&](const auto _d) {
        constexpr size_t d = decltype(_d)::value;
        device::apply([&](const auto &...shifted_idx) { DoEval::eval(mR, shifted_idx...); },
                      tuple_add_to_nth<d>(device::tie(idx...), 1));
      });
    }

    /** \brief Symbolic derivatives. */
    template <typename S> DEVICE_FORCEINLINE_FUNCTION auto d(const S &other)
    {
      return LatticeLaplacian<NDim, R>(GetDeriv::get(mR, other));
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    const FloatType dx2;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <size_t NDim> class LatticeLaplacianTester
  {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <int NDim, typename R>
    requires HasGetMethod<R>
  DEVICE_FORCEINLINE_FUNCTION auto LatLapl(R pR)
  {
    return LatticeLaplacian<NDim, R>(pR);
  }

  template <int NDim, typename R>
    requires(!HasGetMethod<R>)
  DEVICE_FORCEINLINE_FUNCTION auto LatLapl(R pR)
  {
    return ZeroType();
  }
} // namespace TempLat

#endif
