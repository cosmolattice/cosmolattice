#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDCONJUGATE_TEST_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDCONJUGATE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::ComplexFieldConjugateTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
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
    tdd.verify( Real(conj(MyStruct())) == 1 );
    tdd.verify( Imag(conj(MyStruct())) == -2 );

}

#endif
