#ifndef TEMPLAT_LATTICE_ALGEBRA_HASGETMETHOD_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HASGETMETHOD_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::HasGetMethodTester::Test(TempLat::TDDAssertion &tdd)
{
  struct dummy {
    char a;
  };

  struct MyTestOne {
    dummy get(const ptrdiff_t &i) const
    {
      std::cerr << "Hell yeah.\n";
      return dummy();
    }
  };

  struct MyTestTwo {
    void noGet() { std::cerr << "Hell no.\n"; }
  };

  struct MyTestThree {
    dummy get(const ptrdiff_t &, const ptrdiff_t &, const ptrdiff_t &)
    {
      std::cerr << "Hell yeah.\n";
      return dummy();
    }
  };

  struct MyTestFour {
    dummy get(const ptrdiff_t &, const ptrdiff_t &, const ptrdiff_t &, const ptrdiff_t &)
    {
      std::cerr << "Hell yeah.\n";
      return dummy();
    }
  };

  tdd.verify(HasGetMethod<MyTestOne> == true);
  tdd.verify(HasGetMethod<MyTestTwo> == false);
  tdd.verify(HasGetMethod<MyTestThree> == true);
  tdd.verify(HasGetMethod<MyTestFour> == true);
  tdd.verify(HasGetMethod<int> == false);
  tdd.verify(HasGetMethod<bool> == false);
}

#endif
