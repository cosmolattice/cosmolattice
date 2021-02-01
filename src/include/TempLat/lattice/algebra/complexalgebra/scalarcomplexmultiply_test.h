#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_SCALARCOMPLEXFIELDMULTIPLY_TEST_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_SCALARCOMPLEXFIELDMULTIPLY_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::ScalarComplexFieldMultiplyTester::Test(TempLat::TDDAssertion& tdd) {

    struct MyStruct{
        int  ComplexFieldGet(Tag<0> t)
        {
            return 1;
        }
        int  ComplexFieldGet(Tag<1> t)
        {
            return 2;
        }
    };


 //   auto test = MyStruct() * 2;
    /* Default is to fail: to remind yourself to implement something here. */
  //  tdd.verify( test.ComplexFieldGet(0_c) == 2 );
  //  tdd.verify( test.ComplexFieldGet(1_c) == 4 );
}

#endif
