
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020
#include "TempLat/lattice/algebra/helpers/getngrid.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct GetNGridTester {
    static void Test(TDDAssertion &tdd);
  };

  void GetNGridTester::Test(TDDAssertion &tdd)
  {
    struct MyTestOne {
      MyTestOne() : mToolBox(MemoryToolBox<3>::makeShared(32, 1)) {}
      device::memory::host_ptr<MemoryToolBox<3>> getToolBox() const { return mToolBox; }
      device::memory::host_ptr<MemoryToolBox<3>> mToolBox;
    };
    MyTestOne one;

    struct MyTestTwo {
    };
    MyTestTwo two;

    tdd.verify(GetNGrid::get(one) == 32);
    tdd.verify(GetNGrid::get(two) == 1);
    tdd.verify(GetNGrid::getVec(one)[2] == 32);
    tdd.verify(GetNGrid::getVec(two).size() == 0);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::GetNGridTester> test;
}
