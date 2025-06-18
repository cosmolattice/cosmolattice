#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::HasStaticGetterDummy::Test(TempLat::TDDAssertion &tdd)
{
  class MyClass
  {
  public:
    static auto get(ptrdiff_t i, Tag<0> tag) { return 1; }
  };

  /* Default is to fail: to remind yourself to implement something here. */
  tdd.verify(HasStaticGet<MyClass> == true);
  tdd.verify(HasStaticGet<double> == false);
}

#endif
