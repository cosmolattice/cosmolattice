#ifndef TEMPLAT_FFT_EXTERNAL_FFTW_FFTWINTERFACE_H
#define TEMPLAT_FFT_EXTERNAL_FFTW_FFTWINTERFACE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/external/fftw/fftwguard.h"
#include "TempLat/fft/external/fftw/fftwmemorylayout.h"

namespace TempLat {

    /** \brief A class which implements all of FFTLibraryInterface. The larger methods are implemented in classes from which we inherit, in a linear chain: FFTWMemoryLayout and FFTWPlanner.
     *
     * 
     * Unit test: make test-fftwinterface
     **/

    class FFTWInterface : public FFTWMemoryLayout {
    public:
    /* Put public methods here. These should change very little over time. */
        FFTWInterface() {

        }

        virtual ptrdiff_t getMaximumNumberOfDimensionsToDivide(ptrdiff_t nDimensions)
        { return 1; };

        virtual IntrinsicScales getIntrinsicRescaleToGetUnnormalizedFFT(ptrdiff_t nDimensions, ptrdiff_t nGridPoints) {
            return IntrinsicScales();
        }

        virtual std::shared_ptr<SessionGuard> getSessionGuard(bool pVerbose = true) {
            return std::make_shared<FFTWGuard>(pVerbose);
        }
        

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/external/fftw/fftwmemorylayout_test.h"
#endif

#ifdef TEMPLATTEST
#include "TempLat/fft/external/fftw/fftwinterface_test.h"
#endif


#endif
