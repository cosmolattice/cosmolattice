#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASTOOLBOX_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASTOOLBOX_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"

namespace TempLat {

    /** \brief A SFINAE-test class which determines at compile time whether an object has a method `getToolBox`.
     * See HasGetMethod.
     * Unit test: make test-hastoolbox
     **/

    // SFINAE test
    // SFINAE test
    // primary template handles types that have no nested ::type member:
    template< class, class = void_t<> >
    struct HasToolBox : std::false_type { };
    
    // specialization recognizes types that do have a nested ::type member:
    template< class T >
    struct HasToolBox<T, void_t<decltype(std::declval<T>().getToolBox())>> : std::true_type { };

    /** \brief a mini tester class... */
    struct HasToolBoxTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hastoolbox_test.h"
#endif


#endif
