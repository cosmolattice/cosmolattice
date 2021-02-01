#ifndef TEMPLAT_FFT_EXTERNAL_PFFT_PFFTTRANSPOSITIONFLAGS_H
#define TEMPLAT_FFT_EXTERNAL_PFFT_PFFTTRANSPOSITIONFLAGS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#ifndef NOFFT
#ifndef NOMPI
#ifndef NOPPFT
#include "pfft.h"
#endif
#endif
#endif

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/fftlibraryinterface.h"

namespace TempLat {
    
    /** \brief A class which determines the transpose-flags for the PFFT planners, corresponding to a given layout.
     *
     * Unit test: make test-pffttranspositionflags
     **/
    
    class PFFTTranspositionFlags {
    public:
        /* Put public methods here. These should change very little over time. */
        PFFTTranspositionFlags(const FFTLayoutStruct& layout) {
            bool transposedC = layout.fourierSpace.isTransposed();
            bool transposedR = layout.configurationSpace.isTransposed();
            
            mFlagC2R =
            (transposedC ? PFFT_TRANSPOSED_IN : 0 )
            |
            (transposedR ? PFFT_TRANSPOSED_OUT : 0 )
            ;
            
            mFlagR2C =
            (transposedC ? PFFT_TRANSPOSED_OUT : 0 )
            |
            (transposedR ? PFFT_TRANSPOSED_IN : 0 )
            ;
            
        }
        
        unsigned int c2r() { return mFlagC2R; }
        unsigned int r2c() { return mFlagR2C; }
        
        friend
        std::ostream& operator<< ( std::ostream& ostream, const PFFTTranspositionFlags& tr) {
            ostream << "C2R : " << ((tr.mFlagC2R & PFFT_TRANSPOSED_IN) ? " transposed in" : (tr.mFlagC2R & PFFT_TRANSPOSED_OUT) ? "transposed out" : "not transposed") << ".\n";
            ostream << "R2C : " << ((tr.mFlagR2C & PFFT_TRANSPOSED_IN) ? " transposed in" : (tr.mFlagR2C & PFFT_TRANSPOSED_OUT) ? "transposed out" : "not transposed") << ".\n";
            return ostream;
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        unsigned int mFlagC2R;
        unsigned int mFlagR2C;
        
        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/external/pfft/pffttranspositionflags_test.h"
#endif


#endif
