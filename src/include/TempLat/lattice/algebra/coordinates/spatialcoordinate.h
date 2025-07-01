#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_SPATIALCOORDINATE_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_SPATIALCOORDINATE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/coordinates/coordinatevector.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include <Kokkos_Macros.hpp>

namespace TempLat
{

  /** \brief A class which implements spatial coordinates.
   *
   * Unit test: make test-spatialcoordinate
   **/
  MakeException(SpatialCoordinateConfigWrongSpaceConfirmation);

  template <size_t NDim> class SpatialCoordinate : public CoordinateVector<NDim>
  {
  public:
    /* Put public methods here. These should change very little over time. */
    SpatialCoordinate(std::shared_ptr<MemoryToolBox<NDim>> toolBox) : mToolBox(toolBox) {}

    ptrdiff_t getVectorSize() { return mToolBox->mNDimensions; }

    template <std::integral IDX1, std::integral... IDX>
    KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX1 component, const IDX &...idx) const
    {
      using type = decltype(component * (idx * ...));
      return (Kokkos::Array<type, sizeof...(IDX)>{{idx...}}[component]);
    }

    auto operator[](const ptrdiff_t &i) { return get(*this, i); }

    JumpsHolder<NDim> getJumps() { return mToolBox->mLayouts.getConfigSpaceJumps(); }
    void confirmSpace(ptrdiff_t i, const LayoutStruct<NDim> &newLayout,
                      const SpaceStateInterface<NDim>::SpaceType &spaceType)
    {
      switch (spaceType) {
      case SpaceStateInterface<NDim>::SpaceType::Fourier:
        throw SpatialCoordinateConfigWrongSpaceConfirmation(
            "SpatialCoordinate explicitly only can be used in configuration space. Abort.");
        break;
      case SpaceStateInterface<NDim>::SpaceType::Configuration:
      default:
        break;
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::shared_ptr<MemoryToolBox<NDim>> mToolBox;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate_test.h"
#endif

#endif
