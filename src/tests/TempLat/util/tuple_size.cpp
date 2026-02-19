/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/tuple_size.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct tuple_sizeTester {
    static void Test(TDDAssertion &tdd);
  };

  void tuple_sizeTester::Test(TDDAssertion &tdd) {}

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::tuple_sizeTester> test;
}
