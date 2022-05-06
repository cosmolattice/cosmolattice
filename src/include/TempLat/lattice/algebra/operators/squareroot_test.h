#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SQUAREROOT_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SQUAREROOT_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::SqrtTester::Test(TempLat::TDDAssertion& tdd) {

  class myClass{
  public:
    myClass(int b):a(b){}

    auto get(const double& i)
    {
      return i;
    }

  private:
    double a;
  };

    myClass a(4);
    //myClass b(4);
    say << sqrt(a).get(4) << "\n";
    tdd.verify( AlmostEqual(sqrt(a).get(4),2)  );


    say << safeSqrt(a).get(4) << "\n";
    tdd.verify( AlmostEqual(safeSqrt(a).get(4),2)  );


    say << safeSqrt(a).get(-1) << "\n";
    tdd.verify( AlmostEqual(safeSqrt(a).get(-1),0)  );
}

#endif
