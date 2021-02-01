#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_HERMITIANPARTNERS_TEST_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_HERMITIANPARTNERS_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::HermitianPartners::Test(TempLat::TDDAssertion& tdd) {

    HermitianPartners hp({16,16,16,16});
    /* simply test the default implementation */
    tdd.verify( hp.qualify({{}}) == HermitianRedundancy::none );

    std::vector<ptrdiff_t> global(4, 16), hpvec(4, 0);
    hp.putHermitianPartner(global, hpvec);
    bool correct = true;
    for ( ptrdiff_t i = 0; i < 4; ++i) correct = correct && global[i] == hpvec[i];
    tdd.verify( correct );

}

#endif
