/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Year: 2025
#include "TempLat/util/flattenstd.h"
#include "TempLat/util/tdd/tdd.h"

#include <vector>

namespace TempLat
{

  struct FlattenStdTester {
    static void Test(TDDAssertion &tdd);
  };

  void FlattenStdTester::Test(TDDAssertion &tdd)
  {
    std::vector<std::vector<std::vector<int>>> nested = {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}};

    auto flat = flatten(nested);

    for (int i = 0; i < 8; ++i)
      tdd.verify(flat[i] == i + 1);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FlattenStdTester> test;
}
