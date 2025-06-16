#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETJUMPS_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETJUMPS_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::GetJumps::Test(TempLat::TDDAssertion &tdd)
{
  constexpr size_t NDim = 3;

  struct MyTestOne {
    JumpsHolder<3> getJumps() { return JumpsHolder<3>(LayoutStruct<3>({4, 4, 4}), {{{{1, 1}}, {{1, 1}}, {{1, 1}}}}); }
  };
  MyTestOne one;

  struct MyTestTwo {
  };
  MyTestTwo two;

  tdd.verify(GetJumps::apply<NDim>(one).isEmpty() == false);
  tdd.verify(GetJumps::apply<NDim>(two).isEmpty() == true);
}

#endif
