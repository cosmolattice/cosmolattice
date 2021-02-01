#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETGETRETURNTYPE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETGETRETURNTYPE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::GetGetReturnTypeTester::Test(TempLat::TDDAssertion& tdd) {

  struct tmp {
        double get(const ptrdiff_t&) { return 0.; }
    };

    struct tmp2 {
        std::complex<double> get(const ptrdiff_t&) { return std::complex<double>(0., 0.); }
    };

  //  /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( std::is_same<GetGetReturnType<tmp>::type, double>::value );
    tdd.verify( ! GetGetReturnType<tmp>::isComplex );

    tdd.verify( !std::is_same<GetGetReturnType<tmp2>::type, double>::value );
    tdd.verify( std::is_same<GetGetReturnType<tmp2>::type, std::complex<double>>::value );
    tdd.verify( GetGetReturnType<tmp2>::isComplex );


}

#endif
