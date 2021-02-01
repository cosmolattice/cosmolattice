#ifndef TEMPLAT_FFT_FFTNORMALIZATION_TEST_H
#define TEMPLAT_FFT_FFTNORMALIZATION_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/almostequal.h"
#include "TempLat/lattice/memory/memoryblock.h"
#include "TempLat/fft/fftlibraryselector.h"

inline void TempLat::FFTNormalization::Test(TempLat::TDDAssertion& tdd) {

    ptrdiff_t nDim = 3;
    std::vector<ptrdiff_t> nGrid (nDim,256);


    double baseNorm = std::pow(nGrid[0], nDim);
    
    auto split = FFTMPIDomainSplit::makeDomainDecomposition(MPICommReference().size(), nDim);
    FFTLibrarySelector ffter(MPICartesianGroup(nDim, split),  nGrid);

    FFTLayoutStruct layout = ffter.getLayout();

    FFTNormalization normalizer(layout);
    
    ptrdiff_t iEnd = layout.getMinimalMemorySize();
    
    MemoryBlock<double> mem(iEnd);
    
    auto&& doTest = [&](auto expectedNormC2R, auto expectedNormR2C) {
    
        for ( ptrdiff_t i = 0; i < iEnd; ++i) {
            mem[i] = 1;
        }
        normalizer.c2r(mem, 1.);
        bool c2rAllgood = true;
        for ( ptrdiff_t i = 0; i < iEnd; ++i) {
            c2rAllgood = c2rAllgood && AlmostEqual(mem[i], expectedNormC2R);
            if ( ! c2rAllgood ) {
                sayShort << "at " << i << ": " << mem[i] << " != " << expectedNormC2R << "\n";
                break;
            }
        }
        
        tdd.verify(c2rAllgood);

        normalizer.r2c(mem, 1.);
        bool r2cAllgood = true;
        for ( ptrdiff_t i = 0; i < iEnd; ++i) {
            r2cAllgood = r2cAllgood && AlmostEqual(mem[i], expectedNormR2C);
            if ( ! r2cAllgood ) {
                sayShort  << "at " << i << ": " << mem[i] << " != " << expectedNormR2C << "\n";
                break;
            }
        }

        tdd.verify(r2cAllgood);

    };
    
    normalizer.setToConfigType();
    doTest(1. / baseNorm, 1. / baseNorm);

    normalizer.setToFourierType();
    doTest(1., 1. / baseNorm);

    normalizer.setToMixedType();
    doTest(1. / std::sqrt(baseNorm), 1. / baseNorm);


}

#endif
