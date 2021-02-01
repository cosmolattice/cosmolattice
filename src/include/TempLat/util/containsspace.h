#ifndef TEMPLAT_UTIL_CONTAINSSPACE_H
#define TEMPLAT_UTIL_CONTAINSSPACE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <cctype>

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief A class which checks if a string contains a space.
     *
     * Unit test: make test-containsspace
     **/

    class ContainsSpace {
    public:
    /* Put public methods here. These should change very little over time. */

        static inline bool test(const std::string &input) {
            bool result = false;
            for ( auto&& it : input) result = result || isspace(it);
            return result;
        }
        
    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        ContainsSpace() {

        }



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/containsspace_test.h"
#endif


#endif
