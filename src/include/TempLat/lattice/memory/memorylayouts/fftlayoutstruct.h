#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_FFTLAYOUTSTRUCT_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_FFTLAYOUTSTRUCT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"

namespace TempLat {
    
    MakeException(FFTLayoutStructException);
    
    /** \brief A struct which holds two memory layouts, which are unseparable:
     * - the pre-FFT layout in configuration space, no padding or ghosting.
     * - the Fourier space (complex values!) layout.
     *
     *
     * Unit test: make test-fftlayoutstruct
     **/
    
    class FFTLayoutStruct {
    public:
        FFTLayoutStruct(std::vector<ptrdiff_t> nGridPoints, bool isFFTW_, bool isPFFT_) :
        configurationSpace(nGridPoints),
        fourierSpace(LayoutStruct::createGlobalFFTLayout(nGridPoints)),
        mNDimensions(nGridPoints.size()),
        mNGridPoints(nGridPoints),
        mExternalMemoryRequirement(0),
        mIsFFTW(isFFTW_),
        mIsPFFT(isPFFT_)
        {
            
            if ( mIsFFTW == mIsPFFT ) throw FFTLayoutStructException("Must be either FFTW or PFFT, not both");

            /* for FFTW, we manually need to set the size of the last dimension to the r2c setup: 2 * (N/2 + 1). */
            auto configLocalSizes = fourierSpace.getLocalSizes();
            configLocalSizes.back() *= 2;
            configurationSpace.setLocalSizes(configLocalSizes);
            /* but not for configurationSpace.globalSizes: keep the logical layout */

        }
        
        /* yes, public members! That's why its name is "struct". */
        LayoutStruct configurationSpace;
        LayoutStruct fourierSpace;

        /* no, these aren't public members. Just getter methods. */
        const ptrdiff_t& getNDimensions() const { return mNDimensions; }
        const std::vector<ptrdiff_t>& getNGridPoints() const { return mNGridPoints; }
        const bool & isFFTW() const { return mIsFFTW; }
        const bool & isPFFT() const { return mIsPFFT; }
        
        /** \brief Compute on the fly, as our members may be modified by others. That's why OOP... */
        ptrdiff_t getMinimalMemorySize() const {
            ptrdiff_t resultC = 2; /* complex... */
            for (auto&& it : fourierSpace.getLocalSizes()) resultC *= it;

            ptrdiff_t resultR = 1; /* real... */
            for (auto&& it : configurationSpace.getLocalSizes()) resultR *= it;
            //say << "Returning memory size: " << std::max(mExternalMemoryRequirement, std::max(resultR, resultC)) << " from (external)" << mExternalMemoryRequirement << ", (config)" << resultR << ", (fourier)" << resultC << "\n";
            return std::max(mExternalMemoryRequirement, std::max(resultR, resultC)) ;
        }
        
        /** \brief Your library may require more memory than the simple final layout of the result.
         *  Pass that number here.
         *  This is specifically the case for non-transposed FFTW in/outputs:
         *  needs some extra space for the final transposition into normal layout.
         */
        void addExternalMemoryRequest(ptrdiff_t requirement) {
            mExternalMemoryRequirement = std::max(mExternalMemoryRequirement, requirement);
        }

        friend bool operator==(const FFTLayoutStruct &a, const FFTLayoutStruct &b) {
            return a.configurationSpace == b.configurationSpace
            && a.fourierSpace == b.fourierSpace
            && a.mIsPFFT == b.mIsPFFT
            && a.mIsFFTW == b.mIsFFTW;
        }
        
        friend std::ostream& operator<<(std::ostream& ostream, const FFTLayoutStruct& ls) {
            ostream << "fftConfigurationSpace:\n" << ls.configurationSpace << "\n" << "fourierSpace:\n" << ls.fourierSpace << "\n"
                << "\n  Resulting minimal space needed in memory: " << ls.getMinimalMemorySize() << "\n\n";
            return ostream;
        }
    private:
        ptrdiff_t mNDimensions;
        std::vector<ptrdiff_t> mNGridPoints;
        ptrdiff_t mExternalMemoryRequirement;
        
        bool mIsFFTW;
        bool mIsPFFT;
        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
    
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memorylayouts/fftlayoutstruct_test.h"
#endif


#endif
