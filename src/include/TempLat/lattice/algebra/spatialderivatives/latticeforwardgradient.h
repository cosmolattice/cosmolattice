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

namespace TempLat
{

  /** \brief A class which implements the forward gradient.
   *
   *
   * Unit test: make test-forwardgradientlocal
   **/
  template <int NDim, typename R> class LatticeForwardGradient
  {
  public:
    /* Put public methods here. These should change very little over time. */
    using GetReturnType = typename GetGetReturnType<R>::type;
    using FloatType = typename GetFloatType<GetReturnType>::type;

    KOKKOS_FUNCTION
    LatticeForwardGradient(const R &pR) : mR(pR), dx(GetDx::getDx(mR)) { fixGradientMap(GetJumps::apply(mR)); }

    template <std::integral... IDX> KOKKOS_FORCEINLINE_FUNCTION auto vectorGet(ptrdiff_t i, const IDX &...idx)
    {
      auto other_point = std::array<ptrdiff_t, NDim>{{idx...}};
      other_point[i] += 1;
      auto result = -GetValue::get(mR, idx...);
      std::apply([&](const auto &...shifted_idx) { result += GetValue::get(mR, shifted_idx...); }, other_point);
      return result / dx;
    }

    void eval(ptrdiff_t i)
    {
      // TODO (Franz)
      // DoEval::eval(mR, i);
      // for (size_t j = 0; j < nDimensions; ++j)
      //  DoEval::eval(mR, i + jumps[j]);
    }

    ptrdiff_t getVectorSize() { return NDim; }
    auto norm2() { return dot(*this, *this); }
    auto norm() { return pow(this->norm2(), 0.5); }

    std::string toString() const { return "Grad(" + GetString::get(mR) + ")"; }

    void doWeNeedGhosts(ptrdiff_t i) { mR.confirmGhostsUpToDate(); }
    void doWeNeedGhosts() { mR.confirmGhostsUpToDate(); }

    KOKKOS_FORCEINLINE_FUNCTION
    auto getDx() const { return dx; }
    KOKKOS_FORCEINLINE_FUNCTION
    auto getKIR() const { return GetKIR::getKIR(mR); }

    void confirmSpace(ptrdiff_t i, const LayoutStruct<NDim> &newLayout,
                      const SpaceStateInterface<NDim>::SpaceType &spaceType)
    {
      ConfirmSpace::apply(mR, i, newLayout, spaceType);
    }
    ptrdiff_t confirmGhostsUpToDate(ptrdiff_t i) { return ConfirmGhosts::apply(mR, i); }
    inline JumpsHolder<NDim> getJumps()
    { // Don't need indexing for get jumps.
      return GetJumps::apply(mR);
    }

    /** For measurement objects: need the toolbox for easiest access to loopers and whatever else. */
    virtual inline std::shared_ptr<MemoryToolBox<NDim>> getToolBox() { return GetToolBox::get(mR); }

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

  template <int nDimensions = 3, typename R> LatticeForwardGradient<nDimensions, R> LatForwardGrad(R pR)
  {
    return LatticeForwardGradient<nDimensions, R>(pR);
  }

} // namespace TempLat

#endif
