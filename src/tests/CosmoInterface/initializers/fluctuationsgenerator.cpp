
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/initializers/fluctuationsgenerator.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct FluctuationsGeneratorTester {
    static void Test(TDDAssertion &tdd);
  };

  void FluctuationsGeneratorTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(true);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FluctuationsGeneratorTester> test;
}
