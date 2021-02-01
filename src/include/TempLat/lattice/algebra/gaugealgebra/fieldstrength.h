#ifndef TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_FIELDSTRENGTH_H
#define TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_FIELDSTRENGTH_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/lattice/algebra/spatialderivatives/forwdiff.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class which implements fieldstrength tensor as finite differences. Useful for non-compact U(1) mostly.
     * 
     * 
     * Unit test: make test-fieldstrength
     **/

    class FieldStrengthTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template <typename R, int Mu, int Nu>
    auto fieldStrength(R A, Tag<Mu> mu, Tag<Nu> nu)
    {
        return forwDiff(A(nu),mu) - forwDiff(A(mu), nu);
    }


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/gaugealgebra/fieldstrength_test.h"
#endif


#endif
