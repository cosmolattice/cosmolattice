#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_NEUTDIFF_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_NEUTDIFF_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"


namespace TempLat {


    /** \brief A class which
     * Neutral finite differences.
     * 
     * 
     * Unit test: make test-neutdiff
     **/

    class NeutDiffTester {

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<class R, int N>
    auto neutDiff(R pR, Tag<N> t)
    {
        auto dxInv = 0.5 / GetDx::getDx(pR);

        return dxInv *  (shift(pR,t) - shift(pR,-t));
    }


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/spatialderivatives/neutdiff_test.h"
#endif


#endif
