
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2024
#include "TempLat/lattice/algebra/gaugealgebra/nonabelianclover.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct NonAbelianCloverTester {
    static void Test(TDDAssertion &tdd);
  };

  void NonAbelianCloverTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(true);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::NonAbelianCloverTester> test;
}
