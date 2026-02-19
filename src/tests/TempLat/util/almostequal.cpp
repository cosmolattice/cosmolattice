
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/almostequal.h"
#include "TempLat/util/tdd/tdd.h"
#include <array>

namespace TempLat
{

  struct AlmostEqualTester {
    static void Test(TDDAssertion &tdd);
  };

  void AlmostEqualTester::Test(TDDAssertion &tdd)
  {
    std::array<double, 5> list{{3., 3.4, 72435.62, 234., 123.}};

    for (auto &&it : list) {

      double test1 = 1. / std::sqrt(it);
      double test2 = std::sqrt(it) / it;

      tdd.verify(AlmostEqual(test1, test2));
      tdd.verify(!AlmostEqual(test1, 1.001 * test2));
      tdd.verify(AlmostEqual(test1, 1.001 * test2, 0.01));
      tdd.verify(!AlmostEqual(test1, 1.001 * test2, 0.0001));
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::AlmostEqualTester> test;
}
