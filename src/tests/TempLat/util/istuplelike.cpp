
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/istuplelike.h"
#include "TempLat/util/tdd/tdd.h"
#include <array>

namespace TempLat
{

  struct IsTupleLikeTester {
    static void Test(TDDAssertion &tdd);
  };

  void IsTupleLikeTester::Test(TDDAssertion &tdd)
  {

    struct IAmNotATupleAintI {
    };

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(IsTupleLike<std::array<double, 4>>::value == true);
    tdd.verify(IsTupleLike<IAmNotATupleAintI>::value == false);
    tdd.verify(IsTupleLike<int>::value == false);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::IsTupleLikeTester> test;
}
