#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALUNARYGETTER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALUNARYGETTER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

inline void TempLat::ConditionalUnaryGetterTester::Test(TempLat::TDDAssertion &tdd)
{
  class MyClass
  {
  public:
    MyClass(int b) : a(b) {}

    auto get(ptrdiff_t i) { return a; }

  private:
    int a;
  };

  tdd.verify(ConditionalUnaryGetter<MyClass> == true);
  tdd.verify(ConditionalUnaryGetter<double> == false);
}

#endif
