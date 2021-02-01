#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENTTESTER_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORCOMPONENTTESTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"


namespace TempLat {


    /** \brief Just to cut a Spaghetti of include, a tester for get vector component,
     *
     *
     * Unit test: make test-getvectorcomponenttester
     **/

    class GetVectorComponentTester {
    public:
        /* Put public methods here. These should change very little over time. */
        GetVectorComponentTester() {

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
#include "TempLat/lattice/algebra/helpers/getvectorcomponenttester_test.h"
#endif


#endif
