#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_HASCOMPLEXFIELDGET_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_HASCOMPLEXFIELDGET_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat {


    /** \brief A class which checks if instance is a complex field.
     *
     * 
     * Unit test: make test-hascomplexfieldget
     **/

    struct HasComplexFieldGetTester {

#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    template< class , class = void_t<> >
    struct HasComplexFieldGet : std::false_type {
    };

    template < class T >
    struct HasComplexFieldGet<T, void_t<decltype(std::declval<T>().ComplexFieldGet(std::declval<Tag<0>>()))>> : std::true_type {
};



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget_test.h"
#endif


#endif
