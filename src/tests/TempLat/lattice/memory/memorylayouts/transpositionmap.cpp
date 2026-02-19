
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/memory/memorylayouts/transpositionmap.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  template <size_t NDim> struct TranspositionMapTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> void TranspositionMapTester<NDim>::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(Throws<TranspositionMapOutOfBounds>([]() {
      TranspositionMap<3> tmap;
      tmap.setMap({{1, 2, 5}});
    }));

    static constexpr ptrdiff_t nd = 5;

    TranspositionMap<nd> map;

    bool allRight = true;
    for (ptrdiff_t i = 0; i < nd; ++i) {
      allRight = allRight && map.getForward(i) == i && map.getInverse(i) == i;
    }
    tdd.verify(allRight);
    tdd.verify(!map.isTransposed());

    device::IdxArray<nd> input{};
    for (ptrdiff_t i = 0; i < nd; ++i) {
      input[i] = nd - i - 1;
    }
    map.setMap(input);

    for (ptrdiff_t i = 0; i < nd; ++i) {
      allRight = allRight && map.getForward(map.getInverse(i)) == i;
    }
    tdd.verify(allRight);
    tdd.verify(map.isTransposed());

    auto map2 = map;
    tdd.verify(map2 == map);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::TranspositionMapTester<3>> test;
}
