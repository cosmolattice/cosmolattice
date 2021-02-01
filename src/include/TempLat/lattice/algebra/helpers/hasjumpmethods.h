#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASJUMPMETHODS_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASJUMPMETHODS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/jumpsholder.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"

namespace TempLat {

    /** \brief A SFINAE-test class which determines at compile time whether an object has a method `get`.
     * See HasGetMethod.
     * Unit test: make test-hasgetmethod
     **/

    // SFINAE test
    // SFINAE test
    // primary template handles types that have no nested ::type member:


    template< class, class = void_t<> >
    struct HasJumpMethods : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    template< class T >
    struct HasJumpMethods<T, void_t<decltype(std::declval<std::remove_cv_t <T>>().getJumps())>> : std::true_type { };


    /** \brief a mini tester class... */
    struct HasJumpMethodsTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}


#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasjumpmethods_test.h"
#endif


#endif
