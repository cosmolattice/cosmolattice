#ifndef TEMPLAT_FFT_FFTLIBRARYSELECTOR_H
#define TEMPLAT_FFT_FFTLIBRARYSELECTOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <functional>

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/fft/external/fftw/fftwinterface.h"

#ifndef NOFFT
#ifndef NOMPI
#ifndef NOPFFT
#include "TempLat/fft/external/pfft/pfftinterface.h"
#endif
#endif
#endif


namespace TempLat {

    MakeException(FFTLibraryDoubleInitializationException);

    /** \brief A class which sets up the interface with the appropriate FFT library.
     * Once you have implemented the FFTLibraryInterface for your library, add it to the logic here.
     *
     * Unit test: make test-fftlibraryselector
     **/

    class FFTLibrarySelector {
    public:
    /* Put public methods here. These should change very little over time. */
        FFTLibrarySelector(MPICartesianGroup group,  std::vector<ptrdiff_t> nGridPoints, bool forbidTransposition = false) :
        mGroup(group),
        mNDimensions(nGridPoints.size()),
        mNGridPoints(nGridPoints),
        mLayout(mNGridPoints, true, false),
        madePlansFloat(false),
        madePlansDouble(false),
        verbose(false)
        {
            /* here we take the decisions, although the decision to split the group has been made already. */
            ptrdiff_t nDimSplit = group.getNumberOfDividedDimensions();
            bool havePFFT = false;
#ifndef NOMPI
#ifndef NOPFFT
            havePFFT = true;
#endif
#endif
            if ( havePFFT && nDimSplit > 1 ) {
#ifndef NOMPI
#ifndef NOPFFT
                theLibrary = std::make_shared<PFFTInterface>();
#endif
#endif
            } else {
                theLibrary = std::make_shared<FFTWInterface>();
            }
            mLayout = theLibrary->computeLocalSizes(mGroup,  mNGridPoints, forbidTransposition);
        }

        const auto& getLayout() {
            return mLayout;
        }
        
        void setVerbose() {
            verbose = true;
        }

        /* forward all the library calls */
        static ptrdiff_t getMaximumNumberOfDimensionsToDivide(ptrdiff_t nDimensions) {
            ptrdiff_t result = FFTWInterface().getMaximumNumberOfDimensionsToDivide(nDimensions);
#ifndef NOMPI
#ifndef NOPFFT
            result = PFFTInterface().getMaximumNumberOfDimensionsToDivide(nDimensions);
#endif
#endif
            return result;
        };
        
        static std::vector<std::shared_ptr<FFTLibraryInterface::SessionGuard>> getSessionGuards(bool pVerbose = true) {
            if ( getSessionGuardsWasCalledOnce() ) throw FFTLibraryDoubleInitializationException("You can only call getSessionGuards once.");
            
            std::vector<std::shared_ptr<FFTLibraryInterface::SessionGuard>> result;

            /* add your guards here. */
            /* Note: the standard guarantees that the destructors are called in the
             inverse order of appearance in the vector. So if you must construct after FFTW and
             destruct before FFTW, you should be safe if you add your thing after FFTW.
             */
#ifndef NOFFT
            
            result.push_back(FFTWInterface().getSessionGuard(pVerbose));
#ifndef NOMPI
#ifndef NOPFFT
            result.push_back(PFFTInterface().getSessionGuard(pVerbose));
#endif
#endif

#endif

            return result;
        }
        
        void r2c(MemoryBlock<double> &mBlock) {
            getPlans_double();
            if ( verbose ) sayMPI << "Going to perform double r2c.\n";
            mPlansDouble->r2c(mBlock);
        }

        void r2c(MemoryBlock<float> &mBlock) {
            getPlans_float();
            if ( verbose ) sayMPI << "Going to perform float r2c.\n";
            mPlansFloat->r2c(mBlock);
        }

        void c2r(MemoryBlock<double> &mBlock) {
            getPlans_double();
            if ( verbose ) sayMPI << "Going to perform double c2r.\n";
            mPlansDouble->c2r(mBlock);
        }

        void c2r(MemoryBlock<float> &mBlock) {
            getPlans_float();
            if ( verbose ) sayMPI << "Going to perform float c2r.\n";
            mPlansFloat->c2r(mBlock);
        }


        void getPlans_float() {
            if ( ! madePlansFloat ) {
                if ( verbose ) sayMPI << "Going to prepare float FFT plans.\n";
                madePlansFloat = true;
                mPlansFloat = theLibrary->getPlans_float(mGroup, mLayout);
            }
        }
        void getPlans_double() {
            if ( ! madePlansDouble ) {
                if ( verbose ) sayMPI << "Going to prepare double FFT plans.\n";
                madePlansDouble = true;
                mPlansDouble = theLibrary->getPlans_double(mGroup, mLayout);
            }
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        MPICartesianGroup mGroup;
        ptrdiff_t mNDimensions;
        std::vector<ptrdiff_t> mNGridPoints;
        std::shared_ptr<FFTLibraryInterface> theLibrary;
        FFTLayoutStruct mLayout;
        
        bool madePlansFloat;
        bool madePlansDouble;
        std::shared_ptr<FFTLibraryInterface::PlanInterface<float>> mPlansFloat;
        std::shared_ptr<FFTLibraryInterface::PlanInterface<double>> mPlansDouble;

        bool verbose;


        
        /** \brief An inline function for storing a static global variable in a header. A lock that
            verifies that we do not accidentally call the FFT initi/fin-alizations twice. */
        static inline bool getSessionGuardsWasCalledOnce() {
            static bool wasOnce = false;
            bool result = wasOnce;
            wasOnce = true;
            return result;
        }

    public:
        template <typename T>
        static inline void TestBody(TDDAssertion& tdd);

#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif

    };
}


#include "TempLat/fft/fftmpidomainsplit.h"
// include this one after fftmpidomainsplit.h finishes, spaghetti includes...
#ifdef TEMPLATTEST
//#include "TempLat/fft/fftlibraryselector_test.h"
#endif


#endif
