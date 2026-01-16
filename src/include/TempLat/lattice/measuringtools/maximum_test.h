#ifndef TEMPLAT_LATTICE_MEASUREMENTS_MAXIMUM_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_MAXIMUM_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2021

#include "TempLat/util/almostequal.h"

namespace TempLat
{
  struct myTmpStruct {
    static constexpr size_t NDim = 3;
    myTmpStruct() : mt(MemoryToolBox<3>::makeShared(64, 0)) {}
    template <typename... IDX>
      requires IsVariadicNDIndex<3, IDX...>
    DEVICE_FORCEINLINE_FUNCTION double get(const IDX &...idx) const
    {
      return std::get<0>(std::tie(idx...)) > 40 ? 0 : std::get<0>(std::tie(idx...));
    }
    auto getToolBox() const { return mt; }
    void confirmSpace(const LayoutStruct<3> &newLayout, const SpaceStateType &spaceType) const {}
    device::memory::host_ptr<MemoryToolBox<3>> mt;
    std::string toString() const { return "myTmpStruct"; }
  };
} // namespace TempLat

inline void TempLat::MaximumTester::Test(TempLat::TDDAssertion &tdd)
{
  myTmpStruct myInstance;

  auto aget = max(myInstance);
  say << "result of max : " << aget << "\n";
  tdd.verify(aget == 40);
}

#endif
