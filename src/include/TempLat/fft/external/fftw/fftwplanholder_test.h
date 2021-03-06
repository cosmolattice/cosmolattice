#ifndef TEMPLAT_FFT_EXTERNAL_FFTW_FFTWPLANHOLDER_TEST_H
#define TEMPLAT_FFT_EXTERNAL_FFTW_FFTWPLANHOLDER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

template <typename T>
inline void TempLat::FFTWPlanHolder<T>::Test(TempLat::TDDAssertion& tdd) {

    tdd.verify( true );

}

#endif
