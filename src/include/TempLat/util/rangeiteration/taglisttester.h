#ifndef TEMPLAT_UTIL_RANGEITERATION_TAGLISTTESTER_H
#define TEMPLAT_UTIL_RANGEITERATION_TAGLISTTESTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/taglist.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat {


    /** \brief A class to test the tag list and related operations.
     *
     *
     * Unit test: make test-taglisttester
     **/

    class TagListTester {
    public:
        /* Put public methods here. These should change very little over time. */
        TagListTester() {

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
#include "TempLat/util/rangeiteration/taglisttester_test.h"
#endif


#endif
