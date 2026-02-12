#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_DOEVAL_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_DOEVAL_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::DoEval::Test(TempLat::TDDAssertion &tdd)
{
  struct myTest {
    DEVICE_FORCEINLINE_FUNCTION
    void eval(ptrdiff_t i)
    {
      ev = true;
      m = 76;
    }
    double m;

    bool ev = false;
  } mT1;
  struct myTest2 {
    DEVICE_FORCEINLINE_FUNCTION
    double eval()
    {
      ev = true;
      return 777;
    }

    bool ev = false;
  } mT2;
  struct myTest3 {
    DEVICE_FORCEINLINE_FUNCTION
    double eval(int i, int j, int x)
    {
      ev = true;
      return i + j + x;
    }

    bool ev = false;
  } mT3;

  DoEval::eval(mT1, 0);
  DoEval::eval(mT2);
  DoEval::eval(mT3, 1, 2, 3);

  tdd.verify(mT1.ev);
  tdd.verify(mT2.ev);
  tdd.verify(mT3.ev);
}

#endif
