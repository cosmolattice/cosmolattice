#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_TRANSPOSITIONMAP_TEST_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_TRANSPOSITIONMAP_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
template <size_t NDim> inline void TempLat::TranspositionMapTester<NDim>::Test(TempLat::TDDAssertion &tdd)
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

#endif
