#ifndef TEMPLAT_UTIL_FLATTENINGTESTER_H
#define TEMPLAT_UTIL_FLATTENINGTESTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/iscomposite.h"
#include "TempLat/util/flattentuple.h"
#include "TempLat/util/tuplemaker.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2get.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"

namespace TempLat {


    /** \brief A class which tests the tuple flattener.
     *
     * 
     * Unit test: make test-flatteningtester
     **/

    class FlatteningTester {
    public:
        /* Put public methods here. These should change very little over time. */
        FlatteningTester() {

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
#include "TempLat/util/flatteningtester_test.h"
#endif


#endif
