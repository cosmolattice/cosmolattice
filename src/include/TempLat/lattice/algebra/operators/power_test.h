#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_POWER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_POWER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::PowerTester::Test(TempLat::TDDAssertion &tdd)
{

  class myClass
  {
  public:
    DEVICE_FUNCTION
    myClass(int b) : a(b) {}

    DEVICE_FORCEINLINE_FUNCTION
    auto get(ptrdiff_t i) const { return a; }

    DEVICE_FORCEINLINE_FUNCTION
    auto eval(ptrdiff_t i) const { return a; }

    std::string toString() const { return "aaaaaa"; }

  private:
    int a;
  };

  myClass a(3);
  myClass b(4);

  tdd.verify(pow(a, b).eval(0) == 81);
  tdd.verify(pow<3>(a).eval(0) == 27);

  /* Default is to fail: to remind yourself to implement something here. */
  //    tdd.verify( false );
}

#endif
