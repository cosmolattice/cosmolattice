#ifndef TEMPLAT_FFT_EXTERNAL_FFTW_FFTWHERMITIANPARTNERS_H
#define TEMPLAT_FFT_EXTERNAL_FFTW_FFTWHERMITIANPARTNERS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <memory>

#include "TempLat/util/exception.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorylayouts/hermitianredundancy.h"
#include "TempLat/lattice/memory/memorylayouts/hermitianpartners.h"

namespace TempLat {

    MakeException(FFTWHermitianPartnersWrongSizeException);
    
    /** \brief A class which holds all information about the redundancy in
     *  the complex values from a r2c fft. The redundancy comes from
     *  hermitian symmetry, so certain elements must be each other's
     *  complex conjugate. This class tells you which elements are each
     *  other's
     * 
     * 
     * Unit test: make test-fftwhermitianpartners
     **/

    class FFTWHermitianPartners : public HermitianPartners {
    public:

        /** \brief Construct a std::shared_ptr to a new instance. */
        static std::shared_ptr<FFTWHermitianPartners> create(const std::vector<ptrdiff_t>& globalSizes) {
            return std::make_shared<FFTWHermitianPartners>(globalSizes);
        }
        
        FFTWHermitianPartners(const std::vector<ptrdiff_t>& globalSizes) :
        HermitianPartners(globalSizes),
        mGlobalSizes(globalSizes),
        mNDimensions(mGlobalSizes.size()),
        mSignConversionMidpoint(mNDimensions)
        {
            for ( ptrdiff_t i = 0; i < mNDimensions; ++i )  {
                mSignConversionMidpoint[i] = mGlobalSizes[i] / 2;
            }
        }

        /** \brief For testing purposes: track which entries in the layout carry redundant information, and if so, what information.
         */
        HermitianRedundancy qualify(const std::vector<ptrdiff_t>& globalCoordinate) {
            if ( globalCoordinate.size() != mGlobalSizes.size() ) throw FFTWHermitianPartnersWrongSizeException("You called qualify with other dimensionality than this layout.", globalCoordinate.size(), " != ", mGlobalSizes.size());

            ptrdiff_t lastDim = ((ptrdiff_t) mGlobalSizes.size()) - 1;

            HermitianRedundancy result = HermitianRedundancy::none;
            if ( (globalCoordinate[lastDim] == 0) || globalCoordinate[lastDim] == mSignConversionMidpoint[lastDim] ) {
                bool isRealValued = true;
                bool leadingZeros = true;
                bool isNegativePartner = false;
                for ( ptrdiff_t i = 0; i < lastDim; ++i) {
                    isRealValued = isRealValued && ( globalCoordinate[i] == 0 || globalCoordinate[i] == mSignConversionMidpoint[i] );
                    /* if the first non-zero coordinate is negative, we are a negative partner. */
                    if ( leadingZeros && globalCoordinate[i] < 0 ) isNegativePartner = true;
                    /* after testing for negative partnership, update leadingZeros. */
                    leadingZeros = leadingZeros && (globalCoordinate[i] == 0 || globalCoordinate[i] == mSignConversionMidpoint[i]);
                }
                result = isRealValued ? HermitianRedundancy::realValued :
                isNegativePartner ? HermitianRedundancy::negativePartner :
                HermitianRedundancy::positivePartner;
            }
            return result;
        }

        /** \brief If the entry at your input globalCoordinate has a partner which is its hermitian conjugate,
         *  then return the coordinates to that partner. Otherwise return the input. No bounds checking!
         */
        HermitianRedundancy putHermitianPartner(const std::vector<ptrdiff_t>& globalCoordinate, std::vector<ptrdiff_t> &target) {
            target.resize(globalCoordinate.size());
            auto q = qualify(globalCoordinate);
            if ( q != HermitianRedundancy::negativePartner ) {
                for ( ptrdiff_t i = 0; i < mNDimensions; ++i ) target[i] = globalCoordinate[i];
            } else {
                for ( ptrdiff_t i = 0; i < mNDimensions; ++i ) {
                    target[i] = globalCoordinate[i] == 0 || globalCoordinate[i] == mSignConversionMidpoint[i] ? globalCoordinate[i] : - globalCoordinate[i];
                }
            }
            return q;
        }

        /** \brief Compute the number of unique / independent real and imaginary floating point values in the FFTW r2c complex memory layout. */
        virtual HermitianValueAccounting getNumberOfIndependentValues() {
            /* How do we get here? Well, hermitian redundant layout in FFTW is put in
             * N x N x ... x N/2+1 complex values -> naively 1 real and 1 imaginary for each.
             * Half of the entries in the last dim's [N/2] are redundant,
             * as are half of the entries in the last dim's [0].
             * N x N x ... x 2 are the last dim's [N/2] and [0].
             * Moreover, out of the independent half, all values whose coordinate consists
             * of only 0's and N/2's are real-valued, so they don't go into the imaginary count.
             */
            
            ptrdiff_t nGridVertices = 1;
            
            /* mGlobalSizes always has the shape of the real-valued input problem,
             * not the resulting r2c complex layout.
             */
            for ( auto&& it : mGlobalSizes ) nGridVertices *= it;

            ptrdiff_t nComplexVerticesAfterR2C = nGridVertices / mGlobalSizes.back() * (mGlobalSizes.back() / 2 + 1);

say << "mGlobalSizes: " << mGlobalSizes << "\n";
            ptrdiff_t hermitianSymmetricEntries = nGridVertices / mGlobalSizes.back();

            ptrdiff_t imaginary = nComplexVerticesAfterR2C - hermitianSymmetricEntries - std::pow(2, mNDimensions - 1);
            
            /* 2^ND-1 is the number of real-valued entries, at {{ 0, 0, N/2, N/2, ...}}. */
            ptrdiff_t real = imaginary + std::pow(2, mNDimensions);

            return HermitianValueAccounting(real, imaginary);
        }

        virtual std::string toString() const {
            return "FFTW HermitianRedundancy describer.";
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        
        std::vector<ptrdiff_t> mGlobalSizes;
        ptrdiff_t mNDimensions;
        std::vector<ptrdiff_t> mSignConversionMidpoint;

        


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/external/fftw/fftwhermitianpartners_test.h"
#endif


#endif
