#ifndef TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_RANDOMFIELD_H
#define TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_RANDOMFIELD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {
    
    /** \brief A namespace for integration tests of the field class, since many of the building blocks are actually only fully testable using the final product, the field. Note: Integration testing has nothing to do with mathematical integrals, but everything with 'testing the integral(= entire) software product'.
    */
    namespace IntegrationTesting {
        
        /** \brief A class which tests the gaussian random field.
         *
         * Unit test: make test-randomfield
         **/
        
        class RandomField {
        public:
#ifdef TEMPLATTEST
            static inline void Test(TDDAssertion& tdd);
#endif
        };
        
    }
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/field/integrationtesting/randomfield_test.h"
#endif


#endif
