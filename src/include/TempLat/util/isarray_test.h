#ifndef TEMPLAT_UTIL_ISCOMPOSITE_TEST_H
#define TEMPLAT_UTIL_ISCOMPOSITE_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

inline void TempLat::IsArrayTester::Test(TempLat::TDDAssertion &tdd)
{
  tdd.verify(IsArray<std::array<int, 3>, 3> == true);
  tdd.verify(IsArray<Kokkos::Array<int, 3>, 3> == true);

  tdd.verify(IsArray<std::array<int, 3>, 2> == false);
  tdd.verify(IsArray<std::array<int, 3>, 4> == false);
  tdd.verify(IsArray<Kokkos::Array<int, 3>, 2> == false);
  tdd.verify(IsArray<Kokkos::Array<int, 3>, 4> == false);

  tdd.verify(IsArray<int, 3> == false);
  tdd.verify(IsArray<double, 5> == false);
}

#endif
