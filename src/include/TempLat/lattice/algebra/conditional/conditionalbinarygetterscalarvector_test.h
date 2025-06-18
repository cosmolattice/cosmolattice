#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTERSCALARVECTOR_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTERSCALARVECTOR_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::ConditionalBinaryGetterScalarVectorTester::Test(TempLat::TDDAssertion &tdd)
{
  class MyClass
  {
  public:
    MyClass(int b) : a(b) {}

    auto vectorGet(ptrdiff_t i, ptrdiff_t j) { return a; }

  private:
    int a;
  };

  tdd.verify(ConditionalBinaryGetterScalarVector<double, MyClass> == true);

  tdd.verify(ConditionalBinaryGetterScalarVector<MyClass, double> == true);

  tdd.verify(ConditionalBinaryGetterScalarVector<double, double> == false);
}

#endif
