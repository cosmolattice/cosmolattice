#ifndef TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_GRADIENTCOMPONENTTEST_H
#define TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_GRADIENTCOMPONENTTEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief A class which tests getting vector components, specifically on gradients.
     *
     * Unit test: make test-gradientcomponenttest
     **/

    class GradientComponentTest {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/field/integrationtesting/gradientcomponenttest_test.h"
#endif


#endif
