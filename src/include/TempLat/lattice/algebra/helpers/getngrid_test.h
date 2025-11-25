#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETNGRID_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETNGRID_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

inline void TempLat::GetNGrid::Test(TempLat::TDDAssertion &tdd)
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

#endif
