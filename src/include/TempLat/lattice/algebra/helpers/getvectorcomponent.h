#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENT_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/helpers/confirmghosts.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"

namespace TempLat
{
  /** @brief A class which returns one component of a vector as an object, keeping all fourier and co mechanics
   *consistent. Mostly useful for wave numbers.
   *
   * Unit test: ctest -R test-getvectorcomponent
   **/
  template <int N, typename R> class GetVectorComponentHelper
  {
  public:
    // Put public methods here. These should change very little over time.
    GetVectorComponentHelper(const R &pR) : mR(pR) {}

    template <typename... JDX>
      requires requires(std::decay_t<R> mR, JDX... idx) {
        requires IsVariadicIndex<JDX...>;
        DoEval::eval(mR, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const JDX &...jdx) const
    {
      return DoEval::eval(mR, jdx...)[N];
    }

    void doWeNeedGhosts() const { GhostsHunter::apply(mR, N); }

    template <size_t NDim> void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const
    {
      ConfirmSpace::apply(mR, N, newLayout, spaceType);
    }

    ptrdiff_t confirmGhostsUpToDate() const { return ConfirmGhosts::apply(mR, N); }

    inline auto getToolBox() const
    { // just take toolbox from first component
      return GetToolBox::get(mR);
    }

    std::string toString() const { return mR.toString(N); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    R mR;
  };

  template <int N, typename R> GetVectorComponentHelper<N, R> getVectorComponent(const R &pR, Tag<N>)
  {
    return GetVectorComponentHelper<N, R>(pR);
  }
} // namespace TempLat

#endif
