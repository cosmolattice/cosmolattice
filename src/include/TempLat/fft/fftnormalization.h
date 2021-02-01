#ifndef TEMPLAT_FFT_FFTNORMALIZATION_H
#define TEMPLAT_FFT_FFTNORMALIZATION_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <cmath>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorylayouts/fftlayoutstruct.h"
#include "TempLat/lattice/memory/memoryblock.h"
namespace TempLat {

    /** \brief A class which applies the normalization to a block of memory after C2R and R2C fft's.
     * You can choose between:
     * fourier: C2R: 1/N, R2C: 1,
     * mixed:   C2R: 1/sqrt(N), R2C: 1/sqrt(N),
     * config:  C2R: 1, R2C: 1/N,
     *
     * Unit test: make test-fftnormalization
     **/

    class FFTNormalization {
    public:
    /* Put public methods here. These should change very little over time. */
        FFTNormalization(FFTLayoutStruct layout) :
        mLayout(layout),
        mN(1),
        mNumberOfDoublesToChange(2),
        mUnrollLevel(2),
        mNorm(1),
        mType(0)
        {
            for ( auto&& it : mLayout.configurationSpace.getGlobalSizes()) mN *= it;
            /* we always normalize the entire memory, including padding. */
//            for ( auto&& it : mLayout.fourierSpace.getLocalSizes()) mNumberOfDoublesToChange *= it;

            mNumberOfDoublesToChange = mLayout.getMinimalMemorySize();

            while (mNumberOfDoublesToChange % (mUnrollLevel * 2) == 0 && (mUnrollLevel * 2) < mNumberOfDoublesToChange && mUnrollLevel * 2 < cMaxUnroll) mUnrollLevel *= 2;
            mUnrollRepetitions = mNumberOfDoublesToChange / mUnrollLevel;
            setToConfigType();

        }
        
        static constexpr int cConfigType = 0;
        static constexpr int cFourierType = 1;
        static constexpr int cMixedType = 2;
        static constexpr int cMaxUnroll = 32; /* change this if you have a processor with insanely many vector lanes... */

        void setToConfigType() {
            mNorm = 1./(double)mN;
            mType = cConfigType;
        }
        void setToFourierType() {
            mNorm = 1./(double)mN;
            mType = cFourierType;
        }
        void setToMixedType() {
            mNorm = 1./std::sqrt((double)mN);
            mType = cMixedType;
        }
        
        template <typename T>
        inline void c2r(MemoryBlock<T> mBlock, T scale) {
            switch (mType) {
                case cConfigType:
                case cMixedType:
                    apply(mBlock, (T) (scale * mNorm));
                    break;
                case cFourierType:
                default:
                    break;
            }
        }

        template <typename T>
        inline void r2c(MemoryBlock<T> mBlock, T scale) {
            switch (mType) {
                case cFourierType:
                case cMixedType:
                    apply(mBlock, (T) (scale * mNorm));
                    break;
                case cConfigType:
                default:
                    break;
            }
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        FFTLayoutStruct mLayout;
        ptrdiff_t mN;
        ptrdiff_t mNumberOfDoublesToChange;
        ptrdiff_t mUnrollLevel;
        ptrdiff_t mUnrollRepetitions;
        double mNorm;
        int mType;


        template <typename T>
        inline void apply(MemoryBlock<T> &mBlock, T norm) {
            /* manually unrolled... */

            switch (mUnrollLevel) {
                case 32:
                    apply_unrolled<32>(mBlock, norm);
                    break;
                case 16:
                    apply_unrolled<16>(mBlock, norm);
                    break;
                case 8:
                    apply_unrolled<8>(mBlock, norm);
                    break;
                case 4:
                    apply_unrolled<4>(mBlock, norm);
                    break;
                case 2:
                default:
                    apply_unrolled<2>(mBlock, norm);
                    break;
            }
        }
        
        template <ptrdiff_t BLOCKSIZE, typename T>
        inline void apply_unrolled(MemoryBlock<T> &mBlock, T norm) {
            for ( ptrdiff_t i = 0, iEnd = mNumberOfDoublesToChange; i < iEnd; i += BLOCKSIZE ) {
                multiplyInPlace<BLOCKSIZE - 1, T>::apply(mBlock, i, norm);
            }
        }
        
        /** \brief is this a nested class? Yes, but well, it's really a wrapper around a function so that we can partially specialize. */
        template <ptrdiff_t OFFSET, typename T>
        struct multiplyInPlace {
            static inline void apply( MemoryBlock<T> &mBlock, ptrdiff_t i, T norm ) {
                mBlock[i] *= norm;
                multiplyInPlace<OFFSET - 1, T>::apply(mBlock, i + 1, norm);
            }
        };

        template <typename T>
        struct multiplyInPlace<(ptrdiff_t) -1, T> {
            static inline void apply( MemoryBlock<T> &mBlock, ptrdiff_t i, T norm ) {
                /* finished unrolling */
            }
        };


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/fftnormalization_test.h"
#endif


#endif
