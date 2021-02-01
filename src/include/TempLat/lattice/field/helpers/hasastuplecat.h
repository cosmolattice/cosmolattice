#ifndef TEMPLAT_LATTICE_FIELD_HELPERS_HASASTUPLECAT_H
#define TEMPLAT_LATTICE_FIELD_HELPERS_HASASTUPLECAT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"

namespace TempLat {


    /** \brief A class which detects whether or not an object has the hasTuplCat method.
     *
     * 
     * Unit test: make test-hasastuplecat
     **/

    template< class, class = void_t<> >
    struct HasAsTupleCat : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    template< class T >
    struct HasAsTupleCat<T, void_t<decltype(std::declval<T>().asTupleCat())>> : std::true_type { };

    struct HasAsTupleCatTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/field/helpers/hasastuplecat_test.h"
#endif


#endif
