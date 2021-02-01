#ifndef TEMPLAT_UTIL_ENDIANNESS_H
#define TEMPLAT_UTIL_ENDIANNESS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <cstdint>

#include "TempLat/util/tdd/tdd.h"


namespace TempLat {

    /** \brief A class which detects, at runtime, the endianness of the system.
     *
     * Unit test: make test-endianness
     **/

    class Endianness {
    public:
    /* Put public methods here. These should change very little over time. */
        Endianness() : mIsLittleEndian(true) {
            /* see https://stackoverflow.com/a/1001326/2295722 and the comment about int16_t */
            int16_t temp = 0x1234;
            char* tempChar = (char*)&temp;
            mIsLittleEndian = *tempChar == 0x34;
            mIsBigEndian = *tempChar == 0x12;
        }
        const bool& isLittle  () const {
            return mIsLittleEndian;
        }
        const bool& isBig  () const {
            return mIsBigEndian;
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        bool mIsLittleEndian, mIsBigEndian;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/endianness_test.h"
#endif


#endif
