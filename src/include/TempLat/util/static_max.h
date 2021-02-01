#ifndef TEMPLAT_UTIL_STATIC_MAX_H
#define TEMPLAT_UTIL_STATIC_MAX_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class which compute the maximum at compile time.
     *
     * 
     * Unit test: make test-static_max
     **/

    template<size_t N, size_t M>
    struct static_max {
        static const size_t value = N < M ? M : N;
    };

    struct static_max_Tester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion &tdd);
#endif
    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/static_max_test.h"
#endif


#endif
