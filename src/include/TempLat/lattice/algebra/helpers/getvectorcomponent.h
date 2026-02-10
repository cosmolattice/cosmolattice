#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENT_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/algebra/helpers/confirmghosts.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"
#include "TempLat/lattice/algebra/helpers/getjumps.h"
#include "TempLat/lattice/algebra/helpers/getvectorvalue.h"

namespace TempLat
{
  /** @brief A class which returns one component of a vector as an object, keeping all fourier and co mechanics
   *consistent. Mostly useful for wave numbers.
   *
   * Unit test: make test-getvectorcomponent
   **/
  template <typename R> class GetVectorComponentHelper
  {
  public:
    // Put public methods here. These should change very little over time.
    GetVectorComponentHelper(const R &pR, ptrdiff_t pJ) : mR(pR), mJ(pJ) {}

    template <typename... JDX>
      requires requires(std::decay_t<R> mR, ptrdiff_t mJ, JDX... idx) {
        requires IsVariadicIndex<JDX...>;
        mR.vectorGet(mJ, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto get(const JDX &...jdx) const
    {
      return mR.vectorGet(mJ, jdx...);
    }

    void doWeNeedGhosts() const { GhostsHunter::apply(mR, mJ); }

    template <size_t NDim> void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const
    {
      ConfirmSpace::apply(mR, mJ, newLayout, spaceType);
    }

    ptrdiff_t confirmGhostsUpToDate() const { return ConfirmGhosts::apply(mR, mJ); }

    template <size_t NDim> inline JumpsHolder<NDim> getJumps() const
    { // Just take jumps from the first component
      return GetJumps::apply(mR);
    }

    inline auto getToolBox() const
    { // just take toolbox from first component
      return GetToolBox::get(mR);
    }

    std::string toString() const { return mR.toString(mJ); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    R mR;
    const int mJ;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename R> GetVectorComponentHelper<R> getVectorComponent(const R &pR, const ptrdiff_t i)
  {
    return GetVectorComponentHelper<R>(pR, i);
  }
} // namespace TempLat

#endif
