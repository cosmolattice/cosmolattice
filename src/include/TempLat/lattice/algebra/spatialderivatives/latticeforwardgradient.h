#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWARDGRADIENTLOCAL_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWARDGRADIENTLOCAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getjumps.h"
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

namespace TempLat
{

  /** \brief A class which implements the forward gradient.
   *
   *
   * Unit test: make test-forwardgradientlocal
   **/
  template <size_t _NDim, typename R> class LatticeForwardGradient : public UnaryOperator<R>
  {
  public:
    // Put public methods here. These should change very little over time.
    using GetReturnType = typename GetGetReturnType<R>::type;
    using FloatType = typename GetFloatType<GetReturnType>::type;

    static constexpr size_t NDim = _NDim;

    DEVICE_FUNCTION
    LatticeForwardGradient(const R &pR) : mR(pR), dx(GetDx::getDx(mR)) {}

    template <typename... IDX>
      requires requires(R r, IDX... idx) {
        requires IsVariadicNDIndex<NDim, IDX...>;
        GetValue::get(r, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto vectorGet(ptrdiff_t i, const IDX &...idx)
    {
      if constexpr (UnaryOperator<R>::getNDim() == 0)
        return ZeroType();
      else {
        auto other_point = std::array<ptrdiff_t, NDim>{{idx...}};
        other_point[i] += 1;
        auto result = -GetValue::get(mR, idx...);
        device::apply([&](const auto &...shifted_idx) { result += GetValue::get(mR, shifted_idx...); }, other_point);
        return result / dx;
      }
    }

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
        auto other_point = std::array<ptrdiff_t, NDim>{{idx...}};
        other_point[d] += 1;
        device::apply([&](const auto &...shifted_idx) { DoEval::eval(mR, shifted_idx...); }, other_point);
      });
    }

    static constexpr size_t getVectorSize() { return NDim; }

    auto norm2() const { return dot(*this, *this); }
    auto norm() const { return pow(this->norm2(), 0.5); }

    std::string toString() const { return "Grad(" + GetString::get(mR) + ")"; }

    void doWeNeedGhosts() { mR.confirmGhostsUpToDate(); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getDx() const { return dx; }
    DEVICE_FORCEINLINE_FUNCTION
    auto getKIR() const { return GetKIR::getKIR(mR); }

    void confirmSpace(ptrdiff_t i, const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType)
    {
      ConfirmSpace::apply(mR, i, newLayout, spaceType);
    }

    template <int N> ptrdiff_t confirmGhostsUpToDate(Tag<N> i) { return ConfirmGhosts::apply(mR, i); }
    inline JumpsHolder<NDim> getJumps()
    { // Don't need indexing for get jumps.
      return GetJumps::apply(mR);
    }

    /** For measurement objects. */
    inline std::shared_ptr<MemoryToolBox<NDim>> getToolBox() const { return GetToolBox::get(mR); }

    template <typename S> inline auto d(const S &other)
    {
      return LatticeForwardGradient<NDim, R>(GetDeriv::get(mR, other));
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    R mR;
    const FloatType dx;
  };

  class LatticeForwardGradientTester
  {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <int nDimensions = 3, typename R>
  DEVICE_FORCEINLINE_FUNCTION LatticeForwardGradient<nDimensions, R> LatForwardGrad(R pR)
  {
    return LatticeForwardGradient<nDimensions, R>(pR);
  }

} // namespace TempLat

#endif
