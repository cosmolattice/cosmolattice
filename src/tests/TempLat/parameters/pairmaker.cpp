
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/parameters/pairmaker.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct PairMakerTester {
    static void Test(TDDAssertion &tdd);
  };

  void PairMakerTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    std::string w1("mouahaha"), w2("shampoo");
    std::vector<std::string> v;

    v.push_back(w1 + "=" + w2);
    v.push_back("   " + w1 + "=" + w2);
    v.push_back("   " + w1 + "=" + w2 + "      ");
    v.push_back("   " + w1 + "  = " + w2 + "      ");
    v.push_back("   " + w1 + "  = " + w2 + "     #this is a comment");
    PairMaker test;
    for (auto t : v) {
      test(t);
      std::cout << test << std::endl;
      std::cout << test.getKey() << std::endl;
      std::cout << test.getValue() << std::endl;
      tdd.verify(test.getKey() == w1 && test.getValue() == w2);
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::PairMakerTester> test;
}
