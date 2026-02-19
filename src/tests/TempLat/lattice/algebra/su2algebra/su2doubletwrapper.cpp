
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/su2algebra/su2doubletwrapper.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct SU2DoubletWrapperTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2DoubletWrapperTester::Test(TDDAssertion &tdd)
  {
    // Just to check if all compiles

    SU2DoubletWrapper<double, double, double, double> w1;
    tdd.verify(w1.toString() == "SU2(0,0,0,0)");

    SU2DoubletWrapper<double, double, double, double> w2(1.0, 2.0, 3.0, 4.0);
    tdd.verify(w2.toString() == "SU2(1,2,3,4)");
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2DoubletWrapperTester> test;
}
