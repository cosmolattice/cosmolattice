
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2021
#include "TempLat/lattice/measuringtools/maximum.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/almostequal.h"

namespace TempLat {

struct MaximumTester {
  static void Test(TDDAssertion &tdd);
};

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

void MaximumTester::Test(TDDAssertion &tdd)
{
  myTmpStruct myInstance;

  auto aget = max(myInstance);
  say << "result of max : " << aget << "\n";
  tdd.verify(aget == 40);
}

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::MaximumTester> test;
}
