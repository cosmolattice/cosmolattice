#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2OPERATOR_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2OPERATOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2get.h"


namespace TempLat {


    /** \brief A class which contains basic info about su2 algebra.
     *
     * 
     * Unit test: make test-su2operator
     **/

    class SU2Operator {
    public:
        /* Put public methods here. These should change very little over time. */
        SU2Operator() {

        }

        static constexpr size_t size = 4;
        using Getter = SU2Getter;

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "su2operator_test.h"
#endif


#endif
