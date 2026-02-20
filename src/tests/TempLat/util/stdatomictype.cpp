/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Year: 2025
#include "TempLat/util/stdatomictype.h"
#include "TempLat/util/tdd/tdd.h"

#include <array>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <vector>

namespace TempLat
{

  struct StdAtomicTypeTester {
    static void Test(TDDAssertion &tdd);
  };

  void StdAtomicTypeTester::Test(TDDAssertion &tdd)
  {
    using T1 = std::vector<std::vector<int>>;
    using T2 = std::list<std::deque<std::array<float, 3>>>;
    using T3 = std::set<std::unordered_set<double>>;

    tdd.verify(std::is_same<std_atomic_type<T1>::type, int>::value);
    tdd.verify(std::is_same<std_atomic_type<T2>::type, float>::value);
    tdd.verify(std::is_same<std_atomic_type<T3>::type, double>::value);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::StdAtomicTypeTester> test;
}
