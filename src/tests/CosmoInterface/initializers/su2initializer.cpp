
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/initializers/su2initializer.h"
#include "TempLat/lattice/field/collections/vectorfield.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  struct SU2InitializerTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2InitializerTester::Test(TDDAssertion &tdd) { tdd.verify(true); }
} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2InitializerTester> test;
}
