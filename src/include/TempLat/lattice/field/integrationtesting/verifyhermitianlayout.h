#ifndef TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_VERIFYHERMITIANLAYOUT_H
#define TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_VERIFYHERMITIANLAYOUT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief A class which takes an r2c fft, and verifies that the symmetries in the
     *  result correspond to what your HermitianPartners prescribes.
     *
     * Unit test: make test-verifyhermitianlayout
     **/

    class VerifyHermitianLayout {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/field/integrationtesting/verifyhermitianlayout_test.h"
#endif


#endif
