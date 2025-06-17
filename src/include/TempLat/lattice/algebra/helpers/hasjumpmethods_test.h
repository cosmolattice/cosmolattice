#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASJUMPMETHODS_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASJUMPMETHODS_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::HasJumpMethodsTester::Test(TempLat::TDDAssertion &tdd)
{
  static constexpr size_t NDim = 3;

  struct MyTestOne {
    JumpsHolder<NDim> getJumps()
    {
      std::cerr << "Hell yeah.\n";
      return JumpsHolder<NDim>();
    }
  };

  struct MyTestTwo {
    JumpsHolder<NDim> getNOConfigSpaceJumps()
    {
      std::cerr << "Hell no.\n";
      return JumpsHolder<NDim>();
    }
  };

  tdd.verify(HasJumpMethods<NDim, MyTestOne> == true);
  tdd.verify(HasJumpMethods<NDim, MyTestTwo> == false);
  tdd.verify(HasJumpMethods<NDim, int> == false);
}

#endif
