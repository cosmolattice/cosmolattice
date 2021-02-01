#ifndef TEMPLAT_FFT_EXTERNAL_FFTW_FFTWTRANSPOSITIONFLAGS_H
#define TEMPLAT_FFT_EXTERNAL_FFTW_FFTWTRANSPOSITIONFLAGS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#ifndef NOFFT
#ifndef NOMPI
#include "fftw3-mpi.h"
#endif
#endif

#ifndef NOFFT
#include "fftw3.h"
#endif

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/fftlibraryinterface.h"

#ifndef NOMPI
#define FFTWTRANSPOSITIONFLAGS_TRANSPOSED_IN FFTW_MPI_TRANSPOSED_IN
#define FFTWTRANSPOSITIONFLAGS_TRANSPOSED_OUT FFTW_MPI_TRANSPOSED_OUT
#else
#define FFTWTRANSPOSITIONFLAGS_TRANSPOSED_IN 0u
#define FFTWTRANSPOSITIONFLAGS_TRANSPOSED_OUT 0u
#endif

namespace TempLat {

    /** \brief A class which creates the transposition flags for FFTW, based on a layout.
     * 
     * Unit test: make test-fftwtranspositionflags
     **/

    class FFTWTranspositionFlags {
    public:
    /* Put public methods here. These should change very little over time. */
        FFTWTranspositionFlags(const FFTLayoutStruct& layout) {
            bool transposedC = layout.fourierSpace.isTransposed();
            bool transposedR = layout.configurationSpace.isTransposed();
            
            mFlagC2R =
            (transposedC ? FFTWTRANSPOSITIONFLAGS_TRANSPOSED_IN : 0 )
            |
            (transposedR ? FFTWTRANSPOSITIONFLAGS_TRANSPOSED_OUT : 0 )
            ;

            mFlagR2C =
            (transposedC ? FFTWTRANSPOSITIONFLAGS_TRANSPOSED_OUT : 0 )
            |
            (transposedR ? FFTWTRANSPOSITIONFLAGS_TRANSPOSED_IN : 0 )
            ;

        }

        unsigned int c2r() { return mFlagC2R; }
        unsigned int r2c() { return mFlagR2C; }
        
        friend
        std::ostream& operator<< ( std::ostream& ostream, const FFTWTranspositionFlags& tr) {
            ostream << "C2R : " << ((tr.mFlagC2R & FFTWTRANSPOSITIONFLAGS_TRANSPOSED_IN) ? " transposed in" : (tr.mFlagC2R & FFTWTRANSPOSITIONFLAGS_TRANSPOSED_OUT) ? "transposed out" : "not transposed") << ".\n";
            ostream << "R2C : " << ((tr.mFlagR2C & FFTWTRANSPOSITIONFLAGS_TRANSPOSED_IN) ? " transposed in" : (tr.mFlagR2C & FFTWTRANSPOSITIONFLAGS_TRANSPOSED_OUT) ? "transposed out" : "not transposed") << ".\n";
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
#include "TempLat/fft/external/fftw/fftwtranspositionflags_test.h"
#endif

#undef FFTWTRANSPOSITIONFLAGS_TRANSPOSED_IN
#undef FFTWTRANSPOSITIONFLAGS_TRANSPOSED_OUT

#endif
