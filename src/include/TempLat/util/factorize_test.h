#ifndef TEMPLAT_UTIL_FACTORIZE_TEST_H
#define TEMPLAT_UTIL_FACTORIZE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::Factorize::Test(TempLat::TDDAssertion& tdd) {
    
    tdd.verify( DoesNotThrow<FactorizationFailed> (
                                                   []() {
                                                       Factorize fac(13);
                                                       Factorize fac1(4);
                                                       Factorize fac2(6);
                                                       Factorize fac3(12);
                                                       Factorize fac4(148);
                                                       if ( TDDRegister::isSingleUnitTest() ) {
                                                           Factorize fac5(1481451232);
                                                           Factorize fac6(1481444);
                                                           Factorize fac7(1482155);
                                                       }
                                                   }
                                                   )
               );
    //    std::cerr << fac4 << "\n";
    
}

#endif
