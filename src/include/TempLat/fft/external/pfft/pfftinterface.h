#if !defined(TempLat_FFT_EXTERNAL_PFFT_PFFTINTERFACE_H) && !defined(NOPFFT)
#define TEMPLAT_FFT_EXTERNAL_PFFT_PFFTINTERFACE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/external/pfft/pfftmemorylayout.h"
#include "TempLat/fft/external/pfft/pfftguard.h"

namespace TempLat {

    /** \brief A class which implements all of FFTLibraryInterface. The larger methods are implemented in classes from which we inherit, in a linear chain: PFFTMemoryLayout and PFFTPlanner.
     *
     * 
     * Unit test: make test-pfftinterface
     **/

    class PFFTInterface : public PFFTMemoryLayout {
    public:
    /* Put public methods here. These should change very little over time. */
        PFFTInterface() {

        }
        
        virtual ptrdiff_t getMaximumNumberOfDimensionsToDivide(ptrdiff_t nDimensions)
        { return std::max((ptrdiff_t)1, nDimensions - 1); };

        virtual IntrinsicScales getIntrinsicRescaleToGetUnnormalizedFFT(ptrdiff_t nDimensions, ptrdiff_t nGridPoints) {
            return IntrinsicScales();
        }

        virtual std::shared_ptr<SessionGuard> getSessionGuard(bool pVerbose = true) {
            return std::make_shared<PFFTGuard>(pVerbose);
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
#include "TempLat/fft/external/pfft/pfftinterface_test.h"
#endif
#ifdef TEMPLATTEST
#include "TempLat/fft/external/pfft/pfftmemorylayout_test.h"
#endif


#endif
