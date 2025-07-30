#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_NORMGRADIENTSQUARE_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_NORMGRADIENTSQUARE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/spatialderivatives/latticeforwardgradient.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

namespace TempLat
{
  /** \brief A class which computes the gradient square norm.
   *
   *
   * Unit test: make test-normgradientsquare
   **/
  template <int NDim, typename R> class NormGradientSquare : public UnaryOperator<R>
  {
  public:
    /* Put public methods here. These should change very little over time. */
    using GetReturnType = typename GetGetReturnType<R>::type;
    using FloatType = typename GetFloatType<GetReturnType>::type;

    using UnaryOperator<R>::mR;

    KOKKOS_FUNCTION
    NormGradientSquare(const R &pR) : UnaryOperator<R>(pR), dx2(pow<2>(GetDx::getDx(pR)))
    {
      fixGradientMap(GetJumps::apply(mR));
    }

    template <std::integral... IDX> KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
    {
      const auto midval = GetValue::get(mR, idx...);
      FloatType result{};
      constexpr_for<0, NDim, 1>([&](const auto _d) {
        constexpr size_t d = decltype(_d)::value;
        std::apply([&](const auto &...shifted_idx) { result += pow<2>(GetValue::get(mR, shifted_idx...) - midval); },
                   tuple_add_to_nth<d>(std::tie(idx...), 1));
      });
      return result / dx2;
    }

    std::string toString() const { return "|Grad(" + GetString::get(mR) + ")|^2"; }

    void doWeNeedGhosts() { mR.confirmGhostsUpToDate(); }

    KOKKOS_FORCEINLINE_FUNCTION
    void eval(ptrdiff_t i)
    {
      // TODO (Franz)
      // DoEval::eval(mR, i);
      // for (size_t j = 0; j < nDimensions; ++j)
      //  DoEval::eval(mR, i + jumps[j]);
    }

    template <typename S> KOKKOS_FORCEINLINE_FUNCTION auto d(const S &other)
    {
      return 2 * LatForwardGrad(mR) * LatForwardGrad(mR.d(other));
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    FloatType dx2;
  };

  class NormGradientSquareTester
  {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <int nDimensions = 3, typename R>
    requires HasGetMethod<R>
  auto Grad2(R pR)
  {
    return NormGradientSquare<nDimensions, R>(pR);
  }

  template <int nDimensions = 3, typename R>
    requires(!HasGetMethod<R>)
  auto Grad2(R pR)
  {
    return ZeroType();
  }
} // namespace TempLat

#endif
