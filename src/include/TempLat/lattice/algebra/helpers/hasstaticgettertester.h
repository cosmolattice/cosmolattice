#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTERTESTER_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTERTESTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A SFINAE test to see if a iven class has a static getter. Useful to distinguish between "scalar" and "composite" objects.
     *
     * 
     * Unit test: make test-hasstaticgettertester
     **/

    class HasStaticGetterTester {
    public:
        /* Put public methods here. These should change very little over time. */
        HasStaticGetterTester() {

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
#include "TempLat/lattice/algebra/helpers/hasstaticgettertester_test.h"
#endif


#endif
