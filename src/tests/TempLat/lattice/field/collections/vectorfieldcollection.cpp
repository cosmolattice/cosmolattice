
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020
#include "TempLat/lattice/field/collections/vectorfieldcollection.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct VectorFieldCollectionTester {
    static void Test(TDDAssertion &tdd);
  };

  void VectorFieldCollectionTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(true);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::VectorFieldCollectionTester> test;
}
