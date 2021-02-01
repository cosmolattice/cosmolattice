#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORVALUE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORVALUE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::GetVectorValue::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    struct dummy {
        char a;
    };

    struct MyTestOne {
        double vectorGet(const ptrdiff_t& i,const ptrdiff_t& j) {
            std::cerr << "Hell yeah.\n";
            return i+j;
        }
    };

    MyTestOne t1;


    tdd.verify( GetVectorValue::vectorGet(t1,1,2) == 3 );
    tdd.verify( GetVectorValue::vectorGet(t1,5,2) == 7 );
    tdd.verify( GetVectorValue::vectorGet(4,0,0) == 4 );

}

#endif
