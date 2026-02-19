
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/parameters/multipleparametergetter.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct MultipleParameterGetterTester {
    static void Test(TDDAssertion &tdd);
  };

  void MultipleParameterGetterTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    MultipleParameterGetter<int> mp;
    mp.push_back(ParameterGetter<int>(23, ""));
    mp.push_back(ParameterGetter<int>(45, ""));

    std::cout << "hi" << mp << std::endl;

    std::vector<int> test(mp);

    tdd.verify(test[0] == 23);
    tdd.verify(test[1] == 45);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::MultipleParameterGetterTester> test;
}
