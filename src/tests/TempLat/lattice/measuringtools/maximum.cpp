
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2021
#include "TempLat/lattice/measuringtools/maximum.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/device.h"
#include "TempLat/util/almostequal.h"

namespace TempLat
{

  struct MaximumTester {
    static void Test(TDDAssertion &tdd);
  };

  struct myTmpStruct {
    static constexpr size_t NDim = 3;
    myTmpStruct() : mt(MemoryToolBox<3>::makeShared(64, 0)), mLayout(mt->mLayouts.getConfigSpaceLayout()) {}
    template <typename... IDX>
      requires IsVariadicNDIndex<3, IDX...>
    DEVICE_FORCEINLINE_FUNCTION double eval(const IDX &...idx) const
    {
      // get local position
      device::IdxArray<3> global_idx;
      mLayout.putSpatialLocationFromMemoryIndexInto(global_idx, idx...);
      return global_idx[0] > 15 ? 0 : global_idx[0];
    }
    auto getToolBox() const { return mt; }
    void confirmSpace(const LayoutStruct<3> &newLayout, const SpaceStateType &spaceType) const {}
    device::memory::host_ptr<MemoryToolBox<3>> mt;
    std::string toString() const { return "myTmpStruct"; }
    LayoutStruct<NDim> mLayout;
  };

  void MaximumTester::Test(TDDAssertion &tdd)
  {
    myTmpStruct myInstance;

    auto aget = max(myInstance);
    say << "result of max : " << aget << "\n";
    tdd.verify(aget == 15);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::MaximumTester> test;
}
