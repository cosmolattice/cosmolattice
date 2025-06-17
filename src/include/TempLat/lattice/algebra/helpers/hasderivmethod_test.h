#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDERIVMETHOD_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDERIVMETHOD_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

namespace DummyTest
{
  struct dummy {
    char a;
  };
  struct MyTestOne {
    template <typename U> auto d(const U &i)
    {
      std::cerr << "Hell yeah.\n";
      return dummy();
    }
  };
}; // namespace DummyTest

inline void TempLat::HasDerivMethodTester::Test(TempLat::TDDAssertion &tdd)
{
  struct MyTestTwo {
    void noD() { std::cerr << "Hell no.\n"; }
  };

  tdd.verify(HasDerivMethod<DummyTest::MyTestOne> == true);
  tdd.verify(HasDerivMethod<MyTestTwo> == false);
  tdd.verify(HasDerivMethod<int> == false);
}

#endif
