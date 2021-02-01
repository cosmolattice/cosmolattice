#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2ALGEBRATESTER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2ALGEBRATESTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/su2algebra.h"
#include "TempLat/lattice/memory/memorytoolbox_test.h"

namespace TempLat {


    /** \brief A class to test the su2 algebra.
     *
     * 
     * Unit test: make test-su2algebratester
     **/

    class SU2AlgebraTester {
    public:
        /* Put public methods here. These should change very little over time. */
        SU2AlgebraTester() {

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
#include "su2algebratester_test.h"
#endif


#endif
