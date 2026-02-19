/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  template <size_t NDim> struct MemoryToolBoxTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> void MemoryToolBoxTester<NDim>::Test(TDDAssertion &tdd)
  {
    MemoryToolBox<NDim> mtb(256, 2);

    // nothing to test: it is a collection of tools, tested elsewhere.
    //    tdd.verify( true );
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::MemoryToolBoxTester<3>> test;
}
