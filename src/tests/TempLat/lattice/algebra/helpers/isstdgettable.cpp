
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct IsSTDGettableTester {
    static void Test(TDDAssertion &tdd);
  };

  void IsSTDGettableTester::Test(TDDAssertion &tdd)
  {
    struct myDummyStruct {

      int get(Tag<0> t) { return 24; }
    };

    tdd.verify(IsSTDGettable<0, std::tuple<int, int>> == true);
    tdd.verify(IsSTDGettable<0, myDummyStruct> == false);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::IsSTDGettableTester> test;
}
