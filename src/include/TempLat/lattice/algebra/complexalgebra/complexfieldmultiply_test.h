#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDMULTIPLY_TEST_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDMULTIPLY_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::ComplexFieldMultiplyTester::Test(TempLat::TDDAssertion& tdd) {


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
    struct MyStruct2{
        int  ComplexFieldGet(Tag<0> t)
        {
            return 3;
        }
        int  ComplexFieldGet(Tag<1> t)
        {
            return 4;
        }
    };

    auto test = MyStruct() * MyStruct2();
    auto test2 = std::complex<double>(1,2) * MyStruct2();
    
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( test.ComplexFieldGet(0_c) == -5 );
    tdd.verify( test.ComplexFieldGet(1_c) == 10 );
    tdd.verify( test2.ComplexFieldGet(0_c) == -5 );
    tdd.verify( test2.ComplexFieldGet(1_c) == 10 );

}

#endif
