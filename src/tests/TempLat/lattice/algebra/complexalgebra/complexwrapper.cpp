
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/complexalgebra/complexwrapper.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ComplexFieldWrapperTester {
    static void Test(TDDAssertion &tdd);
  };

  void ComplexFieldWrapperTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(Complexify(1, 2).ComplexFieldGet(0_c) == 1);
    tdd.verify(Complexify(1, 2).ComplexFieldGet(1_c) == 2);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ComplexFieldWrapperTester> test;
}
