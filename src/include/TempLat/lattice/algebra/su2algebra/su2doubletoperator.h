#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETOPERATOR_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETOPERATOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2doubletget.h"


namespace TempLat {


    /** \brief A class which groups some features of the SU2Doublet ops.
     *
     * 
     * Unit test: make test-su2doubletoperator
     **/

    class SU2DoubletOperator {
    public:
        /* Put public methods here. These should change very little over time. */
        SU2DoubletOperator() {

        }
        static constexpr size_t size = 4;
        using Getter = SU2DoubletGetter;

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "su2doubletoperator_test.h"
#endif


#endif
