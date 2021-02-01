#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTSTATE_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTSTATE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {
    
    /** \brief A class which tracks the three layout states the memory can have. If state is undefined, all the
     *   is___Space() variants return true. So you don't wast time FFT'ing trash etc. 
     *
     * Unit test: make test-memorylayoutstate
     **/
    
    class MemoryLayoutState {
    public:
        /* Put public methods here. These should change very little over time. */
        MemoryLayoutState() :
        mState(undefined)
        {
            
        }
        
        void setToConfigSpace() {
            setState(configSpace);
        }
        void setToFFTConfigSpace() {
            setState(fftConfigSpace);
        }
        void setToFourierSpace() {
            setState(fourierSpace);
        }
        bool isConfigSpace() const {
            return mState == configSpace || mState == undefined;
        }
        bool isFFTConfigSpace() const {
            return mState == fftConfigSpace || mState == undefined;
        }
        bool isFourierSpace() const {
            return mState == fourierSpace || mState == undefined;
        }

        
        friend std::ostream& operator<< ( std::ostream& ostream, const MemoryLayoutState &mls) {
            ostream << "Memory state: " << mls.getStateName(mls.mState);
            return ostream;
        }
        
        
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        
        static constexpr int undefined = 0;
        static constexpr int configSpace = 1;
        static constexpr int fftConfigSpace = 2;
        static constexpr int fourierSpace = 3;
        
        int mState;
        
        void setState(int state) {
            mState = state;
        }
        
        const char * getStateName(const int state) const {
            const char *result = "unknown";
            switch (state) {
                case fourierSpace:
                    result = "Fourier space";
                    break;
                case configSpace:
                    result = "configuration space";
                    break;
                case fftConfigSpace:
                    result = "configuration space for FFT";
                    break;
                case undefined:
                default:
                    result = "undefined";
                    break;
            }
            return result;
        }
        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memorylayoutstate_test.h"
#endif


#endif
