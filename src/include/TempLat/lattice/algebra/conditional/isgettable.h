#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_ISGETTABLE_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_ISGETTABLE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasgetmethod.h"

namespace TempLat {

    /** \brief An alias for HasGetMethod, just for the structure. Has ::value == true if the
     *  class has a method get.
     *
     * Unit test: make test-isgettable
     **/
    template <typename T>
    using IsGettable = HasGetMethod<T>;
    
    struct IsGettableTester {
        static void Test(TempLat::TDDAssertion&);
    };
    
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/conditional/isgettable_test.h"
#endif


#endif
