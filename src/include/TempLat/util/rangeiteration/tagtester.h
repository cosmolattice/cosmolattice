#ifndef TEMPLAT_UTIL_RANGEITERATION_TAGTESTER_H
#define TEMPLAT_UTIL_RANGEITERATION_TAGTESTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class to test the tags and related operations.
     * 
     * 
     * Unit test: make test-tagtester
     **/

    class TagTester {
    public:
        /* Put public methods here. These should change very little over time. */
        TagTester() {

        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/rangeiteration/tagtester_test.h"
#endif


#endif
