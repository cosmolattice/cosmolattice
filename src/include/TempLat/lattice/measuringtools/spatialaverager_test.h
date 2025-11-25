#ifndef TEMPLAT_LATTICE_MEASUREMENTS_SPATIALAVERAGER_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_SPATIALAVERAGER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"

namespace TempLat
{
  template <size_t _NDim> struct myTmpStruct {
    static constexpr size_t NDim = _NDim;

    myTmpStruct() : mt(MemoryToolBox<NDim>::makeShared(16, 0)), mLayout(mt->mLayouts.getConfigSpaceLayout()) {}

    template <typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION double get(const IDX &...idx) const
    {
      Kokkos::Array<ptrdiff_t, NDim> ii;
      mLayout.putSpatialLocationFromMemoryIndexInto0N(ii, idx...);

      return ii[0] * pow(16, 3) + ii[1] * pow(16, 2) + ii[2] * 16 + ii[3];
    }

    double expectedAnswer(int l)
    {
      return (0.5 * (15) * 16 * (pow(16, 2) + 16 + 1)) + l; // sum_0^N-1 = (N-1)N/2
    }

    auto getToolBox() const { return mt; }
    void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) {}
    device::memory::host_ptr<MemoryToolBox<NDim>> mt;
    std::string toString() const { return "myTmpStruct"; }

    LayoutStruct<NDim> mLayout;
  };
} // namespace TempLat

inline void TempLat::SpatialAveragerTester::Test(TempLat::TDDAssertion &tdd)
{
  static constexpr size_t NDim = 2;
  /* Default is to fail: to remind yourself to implement something here. */
  myTmpStruct<NDim> myInstance;

  auto aget = spatialAverage(myInstance);

  for (int i = 0; i < 15; ++i) {
    //  tdd.verify(AlmostEqual(aget[i], myInstance.expectedAnswer(i)));
    sayShort << "i = " << i << ", aget[i] = " << aget[i] << ", expected = " << myInstance.expectedAnswer(i) << "\n";
  }
  tdd.verify(true); // TODO FRANZ
}

#endif
