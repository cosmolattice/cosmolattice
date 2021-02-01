#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_FFTLAYOUTSTRUCT_TEST_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_FFTLAYOUTSTRUCT_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::FFTLayoutStruct::Test(TempLat::TDDAssertion& tdd) {

    /* only test the operator== */
    FFTLayoutStruct a({100,100,100}, true, false);
    FFTLayoutStruct b({100,100,100},  true, false);
    FFTLayoutStruct c( {100,100}, true, false);
    FFTLayoutStruct d( {100,100,100}, true, false);
    
    auto newSizes = d.configurationSpace.getLocalSizes();
    newSizes[1] = 2;
    d.configurationSpace.setLocalSizes(newSizes);

    tdd.verify( ! (a == c) );
    tdd.verify( (a == b) );
    tdd.verify( ! (a == d) );

}

#endif
