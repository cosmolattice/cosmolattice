#ifndef TEMPLAT_UTIL_IDEATESTER_H
#define TEMPLAT_UTIL_IDEATESTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"


namespace TempLat {


    /** \brief A class which can be used to test random implementation ideas.
     *
     * 
     * Unit test: make test-ideatester
     **/

    class IdeaTester {
    public:
        /* Put public methods here. These should change very little over time. */
        IdeaTester() {

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
#include "TempLat/util/ideatester_test.h"
#endif


#endif
