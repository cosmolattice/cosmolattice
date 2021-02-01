#ifndef TEMPLAT_UTIL_TUPLE_SIZE_TESTER_H
#define TEMPLAT_UTIL_TUPLE_SIZE_TESTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class which tests the tuple_size class.
     *
     * 
     * Unit test: make test-tuple_size_tester
     **/

    class tuple_size_Tester {
    public:
        /* Put public methods here. These should change very little over time. */
        tuple_size_Tester() {

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
#include "TempLat/util/tuple_size_tester_test.h"
#endif


#endif
