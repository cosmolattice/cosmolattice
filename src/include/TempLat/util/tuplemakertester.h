#ifndef TEMPLAT_UTIL_TUPLEMAKERTESTER_H
#define TEMPLAT_UTIL_TUPLEMAKERTESTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class which tests the tuple makker class.
     *
     * 
     * Unit test: make test-tuplemakertester
     **/

    class TupleMakerTester {
    public:
        /* Put public methods here. These should change very little over time. */
        TupleMakerTester() {

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
#include "TempLat/util/tuplemakertester_test.h"
#endif


#endif
