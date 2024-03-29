#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWDIJ_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWDIJ_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"


namespace TempLat {


    /** \brief A class which
     * Forward finite differences.
     * 
     * 
     * Unit test: make test-forwdiff
     **/

    class ForwDijTester {

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<class R, int N>
    auto forwDij(R pR, Tag<N> t)
    {
        auto dxInv = 1.0 / GetDx::getDx(pR);

        return dxInv *  (shift(pR,t) + pR);
    }


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/spatialderivatives/forwdiff_test.h"
#endif


#endif
