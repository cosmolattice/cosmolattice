#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETKIR_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETKIR_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::GetKIR::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    struct myTest{
      double getKIR()
      {
        return 0.42;
      }
    };
    struct myTest1{
      double getKIR()
      {
        return 0.42;
      }

      double get(ptrdiff_t i)
      {
        return 76;
      }
    };

    struct myTest2{
      double getIR()
      {
        return 0.42;
      }

      double get(ptrdiff_t i)
      {
        return 76;
      }
    };

    myTest1 t1;
    myTest2 t2;

    //myTest t;
    //tdd.verify(GetKIR::getKIR(t1) == 0.42); //does not compile as we don't have get method and is needed for automatic type deducing.
    tdd.verify(GetKIR::getKIR(t1) == 0.42);
    tdd.verify(GetKIR::getKIR(t2) == 1);

}

#endif
