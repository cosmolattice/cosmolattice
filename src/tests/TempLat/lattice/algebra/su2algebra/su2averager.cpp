
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/su2algebra/su2averager.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/su2field.h"

namespace TempLat
{

  struct SU2AveragerTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2AveragerTester::Test(TDDAssertion &tdd)
  {
    auto toolbox = MemoryToolBox<3>::makeShared(16, 1);

    SU2Field<3, double> A("A", toolbox);

    A(1_c) = 0.2;
    A(2_c) = 0.3;
    A(3_c) = 0.4;
    A.unitarize();

    auto average = su2average(A);

    tdd.verify(AlmostEqual(average.getComp(0_c), sqrt(1.0 - 0.2 * 0.2 - 0.3 * 0.3 - 0.4 * 0.4)));
    tdd.verify(AlmostEqual(average.getComp(1_c), 0.2));
    tdd.verify(AlmostEqual(average.getComp(2_c), 0.3));
    tdd.verify(AlmostEqual(average.getComp(3_c), 0.4));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2AveragerTester> test;
}
