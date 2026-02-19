
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020
#include "TempLat/util/isincontainer.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  struct IsInContainerTester {
    static void Test(TDDAssertion &tdd);
  };

  void IsInContainerTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(IsInContainer::check(1, std::vector<int>{1, 2, 3, 4}));
    tdd.verify(!IsInContainer::check(1, std::vector<int>{2, 3, 4}));
    tdd.verify(IsInContainer::check("E_K", std::vector<std::string>{"a", "E_K", "b"}));
  }
} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::IsInContainerTester> test;
}
