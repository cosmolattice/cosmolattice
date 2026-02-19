
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/su2algebra/su2doubletaverager.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/su2doublet.h"

namespace TempLat
{

  struct SU2DoubletAveragerTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2DoubletAveragerTester::Test(TDDAssertion &tdd)
  {
    auto toolbox = MemoryToolBox<3>::makeShared(16, 1);

    SU2Doublet<3, double> A("A", toolbox);

    A(0_c) = 0.1;
    A(1_c) = 0.2;
    A(2_c) = 0.3;
    A(3_c) = 0.4;

    auto average = su2doubletaverage(A);

    tdd.verify(AlmostEqual(average.getComp(0_c), 0.1));
    tdd.verify(AlmostEqual(average.getComp(1_c), 0.2));
    tdd.verify(AlmostEqual(average.getComp(2_c), 0.3));
    tdd.verify(AlmostEqual(average.getComp(3_c), 0.4));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2DoubletAveragerTester> test;
}
