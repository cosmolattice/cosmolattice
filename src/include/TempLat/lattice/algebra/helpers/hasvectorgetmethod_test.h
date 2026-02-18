#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETVECTORMETHOD_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETVECTORMETHOD_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

namespace TempLat
{
  struct dummy {
    char a;
  };

  struct MyTestOne {
    dummy vectorGet(Tag<1> t)
    {
      std::cerr << "Hell yeah.\n";
      return dummy();
    }
  };

  struct MyTestTwo {
    void noGet() { std::cerr << "Hell no.\n"; }
  };

  struct MyTestThree {
    template <int N> dummy vectorGet(Tag<N> t)
    {
      std::cerr << "Hell yeah.\n";
      return dummy();
    }
  };
} // namespace TempLat

inline void TempLat::HasVectorGetMethodTester::Test(TempLat::TDDAssertion &tdd)
{
  tdd.verify(HasVectorGetMethod<MyTestOne> == true);
  tdd.verify(HasVectorGetMethod<MyTestTwo> == false);
  tdd.verify(HasVectorGetMethod<MyTestThree> == true);
  tdd.verify(HasVectorGetMethod<int> == false);
}

#endif
