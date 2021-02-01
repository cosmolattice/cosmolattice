#ifndef TEMPLAT_LATTICE_ALGEBRA_HASSTRINGMETHOD_H
#define TEMPLAT_LATTICE_ALGEBRA_HASSTRINGMETHOD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <type_traits>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"
#include "TempLat/util/rangeiteration/tag.h"


namespace TempLat {

    /** \brief A SFINAE-test class which determines at compile time whether an object has a method `get`.
     * See HasGetMethod.
     * Unit test: make test-hasgetmethod
     **/

    // SFINAE test
    // primary template handles types that have no nested ::type member:
    template< class, class = void_t<> >
    struct HasStringMethod : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    template< class T >
    struct HasStringMethod<T, void_t<decltype(std::declval<T>().toString())>> : std::true_type { };

    // SFINAE test
    // primary template handles types that have no nested ::type member:
    template<int, class, class = void_t<> >
    struct HasStringMethodIndexed : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    template<int N, class T >
    struct HasStringMethodIndexed<N,T, void_t<decltype(std::declval<T>().toString(std::declval<Tag<N>>()))>> : std::true_type { };




    /** \brief a mini tester class... */
    struct HasStringMethodTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasstringmethod_test.h"
#endif


#endif
