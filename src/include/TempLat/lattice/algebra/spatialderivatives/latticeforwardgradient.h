#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWARDGRADIENTLOCAL_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWARDGRADIENTLOCAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/listoperators/vectordotter.h"
#include "TempLat/util/tuple_tools.h"
#include "TempLat/lattice/algebra/spatialderivatives/forwdiff.h"

namespace TempLat
{

  /** @brief A class which implements the forward gradient.
   *
   *
   * Unit test: ctest -R test-forwardgradientlocal
   **/
  template <size_t _NDim, typename R> class LatticeForwardGradient : public UnaryOperator<R>
  {
  private:
    using UnaryOperator<R>::mR;

  public:
    // Put public methods here. These should change very little over time.
    using GetReturnType = typename GetGetReturnType<R>::type;
    using FloatType = typename GetFloatType<GetReturnType>::type;

    static constexpr size_t NDim = _NDim;

    DEVICE_FUNCTION
    LatticeForwardGradient(const R &pR) : UnaryOperator<R>(pR), dx(GetDx::getDx(mR)) {}

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
        device::array<GetReturnType, NDim> result{};
        constexpr_for<0, NDim>([&](const auto _i) {
          constexpr int i = decltype(_i)::value;

          result[i] = -DoEval::eval(mR, idx...);
          device::apply([&](const auto &...shifted_idx) { result[i] += DoEval::eval(mR, shifted_idx...); },
                        tuple_add_to_nth<i, 1>(device::tie(idx...)));
          result[i] /= dx;
        });
        return result;
      }
    }

    template <int N>
      requires(N > 0)
    auto vectorGet(Tag<N> t) const
    {
      return ForwDiff<N, R>(mR);
    }

    template <int N>
      requires(N > 0)
    auto operator()(Tag<N> t) const
    {
      return vectorGet(t);
    }

    static constexpr size_t getVectorSize() { return NDim; }

    auto norm2() { return dot(*this, *this); }
    auto norm() { return pow(this->norm2(), 0.5); }

    std::string toString() const { return "Grad(" + GetString::get(mR) + ")"; }

    void doWeNeedGhosts() const { mR.confirmGhostsUpToDate(); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getDx() const { return dx; }
    DEVICE_FORCEINLINE_FUNCTION
    auto getKIR() const { return GetKIR::getKIR(mR); }

    void confirmSpace(ptrdiff_t i, const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const
    {
      ConfirmSpace::apply(mR, i, newLayout, spaceType);
    }

    template <int N> ptrdiff_t confirmGhostsUpToDate(Tag<N> i) const { return ConfirmGhosts::apply(mR, i); }

    /** For measurement objects. */
    inline device::memory::host_ptr<MemoryToolBox<NDim>> getToolBox() const { return GetToolBox::get(mR); }

    template <typename S> inline auto d(const S &other)
    {
      return LatticeForwardGradient<NDim, R>(GetDeriv::get(mR, other));
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    const FloatType dx;
  };

  template <int nDimensions, typename R>
  DEVICE_FORCEINLINE_FUNCTION LatticeForwardGradient<nDimensions, R> LatForwardGrad(R pR)
  {
    return LatticeForwardGradient<nDimensions, R>(pR);
  }

} // namespace TempLat

#endif
