#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGHOSTMETHOD_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGHOSTMETHOD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"
#include "TempLat/util/rangeiteration/tag.h"

//#include "TempLat/lattice/field/fieldshiftedviewdetection.h"
namespace TempLat {

    /** \brief A SFINAE-test class which determines at compile time whether an object has a method `get`.
     * See HasGetMethod.
     * Unit test: make test-hasgetmethod
     **/

    // SFINAE test
    // primary template handles types that have no nested ::type member:
    //template< class, class = void_t<> >
    //struct HasGhostMethod : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    //template< class T >
    //struct HasGhostMethod<T, void_t<decltype(std::declval<T>().confirmGhostsUpToDate(FieldShiftedViewDetection()))>> : std::true_type { };

    template< class, class = void_t<> >
    struct HasGhostMethod : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    template< class T >
    struct HasGhostMethod<T, void_t<decltype(std::declval<T>().confirmGhostsUpToDate())>> : std::true_type { };

    template<int, class, class = void_t<> >
    struct HasGhostMethodIndexed : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    template<int N, class T >
    struct HasGhostMethodIndexed<N,T, void_t<decltype(std::declval<T>().confirmGhostsUpToDate(std::declval<Tag<N>>()))>> : std::true_type { };



    /** \brief a mini tester class... */
    struct HasGhostMethodTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasghostmethod_test.h"
#endif


#endif
