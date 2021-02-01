#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_HERMITIANVALUEACCOUNTING_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_HERMITIANVALUEACCOUNTING_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief A number of unique / independent real and imaginary floating point values in a memory layout.
     *
     * 
     * Unit test: make test-hermitianvalueaccounting
     **/

    class HermitianValueAccounting {
    public:
        /* Put public methods here. These should change very little over time. */
        HermitianValueAccounting(ptrdiff_t realCount, ptrdiff_t imaginaryCount):
        mRealCount(realCount),
        mImaginaryCount(imaginaryCount)
        {

        }
        
        ptrdiff_t getRealValueCount() {
            return mRealCount;
        }

        ptrdiff_t getImaginaryValueCount() {
            return mImaginaryCount;
        }

        friend std::ostream& operator<<(std::ostream& ostream, const HermitianValueAccounting& hva) {
            ostream << hva.mRealCount << " independent real values,\n" << hva.mImaginaryCount << " imaginary values.";
            return ostream;
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        ptrdiff_t mRealCount;
        ptrdiff_t mImaginaryCount;
        

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memorylayouts/hermitianvalueaccounting_test.h"
#endif


#endif
