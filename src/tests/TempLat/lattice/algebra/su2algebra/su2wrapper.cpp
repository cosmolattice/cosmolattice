
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/su2algebra/su2wrapper.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/parallel/device_memory.h"

namespace TempLat
{

  struct SU2WrapperTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2WrapperTester::Test(TDDAssertion &tdd)
  {
    // Just to check if all compiles
    SU2Wrapper<double, double, double, double> w1(0., 0., 0., 0.);
    tdd.verify(w1.toString() == "SU2(0,0,0,0)");

    SU2Wrapper<double, double, double, double> w2(2.0, 3.0, 4.0, 4.0);
    tdd.verify(w2.toString() == "SU2(2,3,4,4)");

    auto toolbox = MemoryToolBox<3>::makeShared(16, 0);

    SU2Field<3, double> f1("f", toolbox);
    f1 = w2;

    // First one will be off, because the SU2Get(0_c) is not used in the assignment, but the others should be correct.
    // tdd.verify(device::memory::getAtOnePoint(f1.SU2Get(0_c), device::IdxArray<3>{0, 0, 0}) == 2.0);
    tdd.verify(device::memory::getAtOnePoint(f1.SU2Get(1_c), device::IdxArray<3>{0, 0, 0}) == 3.0);
    tdd.verify(device::memory::getAtOnePoint(f1.SU2Get(2_c), device::IdxArray<3>{0, 0, 0}) == 4.0);
    tdd.verify(device::memory::getAtOnePoint(f1.SU2Get(3_c), device::IdxArray<3>{0, 0, 0}) == 4.0);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2WrapperTester> test;
}
