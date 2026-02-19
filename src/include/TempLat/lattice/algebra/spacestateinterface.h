#ifndef TEMPLAT_LATTICE_ALGEBRA_SPACESTATEINTERFACE_H
#define TEMPLAT_LATTICE_ALGEBRA_SPACESTATEINTERFACE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <string>

#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"

namespace TempLat
{
  /** @brief An enum for passing the right space type, all while having the compiler type check it. */
  enum class SpaceStateType { Configuration, Fourier, undefined };

  /** @brief An interface class which all getter-like objects inherit from, so we can access their
   *confirm(Config/Fourier)Space methods by their virtualness.
   *
   * Unit test: ctest -R test-spacestateinterface
   **/
  template <size_t NDim> class SpaceStateInterface
  {
  public:
    // Put public methods here. These should change very little over time.

    /** @brief A preparation function: pass this call to all the members in the tree / chain, make sure everyone is in
     * configuration or fourier space, and everyone has the actual same layout. */
    virtual void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) = 0;

    /** @brief A preparation function: pass this call to all the members in the tree / chain.
     *   We only want to update ghost cells on those fields where this expression tree actually uses
     *   a spatially shifted view (which needs ghost cells). So only such views have permission
     *   to toggle the value of the detector to true. The Fields on the receiving end of this chain,
     *   then update ghosts.
     */
    // virtual
    // ptrdiff_t confirmGhostsUpToDate(FieldShiftedViewDetection detector) = 0;

    /** For measurement objects. */
    virtual inline device::memory::host_ptr<MemoryToolBox<3>> getToolBox() = 0;

    static inline std::string SpaceTypeString(SpaceStateType st)
    {
      std::string result;
      switch (st) {
      case SpaceStateType::Configuration:
        result = "configuration space";
        break;
      case SpaceStateType::Fourier:
        result = "fourier space";
        break;
      default:
      case SpaceStateType::undefined:
        result = "undefined space type";
        break;
      }
      return result;
    }

    static inline std::string SpaceTypetoCanonicalCharacter(SpaceStateType st)
    {
      std::string result;
      switch (st) {
      case SpaceStateType::Configuration:
        result = "x";
        break;
      case SpaceStateType::Fourier:
        result = "k";
        break;
      default:
      case SpaceStateType::undefined:
        result = "[x or k]";
        break;
      }
      return result;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  template <size_t NDim> inline std::ostream &operator<<(std::ostream &ostream, SpaceStateType st)
  {
    ostream << "SpaceType::" + SpaceStateInterface<NDim>::SpaceTypeString(st);
    return ostream;
  }
} // namespace TempLat

#endif
