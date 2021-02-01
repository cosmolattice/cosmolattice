#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETFLOATTYPE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETFLOATTYPE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <complex>

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief A template which extracts the float type from complexes.
     *
     * Unit test: make test-getfloattype
     **/

    template <typename T>
    struct GetFloatType  {
        typedef T type;
    };
    
    template <typename S>
    struct GetFloatType<std::complex<S>> {
        typedef S type;
    };
    
    struct GetFloatTypeTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/getfloattype_test.h"
#endif


#endif
