#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSPACECONFIRMATIONMETHODS_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSPACECONFIRMATIONMETHODS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/util/rangeiteration/tag.h"


namespace TempLat {

    /** \brief A SFINAE-test class which determines at compile time whether an object has a method `get`.
     * See HasGetMethod.
     * Unit test: make test-hasgetmethod
     **/

    // SFINAE test
    // primary template handles types that have no nested ::type member:
    template< class, class = void_t<> >
    struct HasSpaceConfirmationMethods : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    template< class T >
    struct HasSpaceConfirmationMethods<T, void_t<decltype(std::declval<T>().confirmSpace(std::declval<LayoutStruct>(), std::declval<SpaceStateInterface::SpaceType>()))>> : std::true_type { };

    // SFINAE test
    // primary template handles types that have no nested ::type member:
    template<int, class, class = void_t<> >
    struct HasSpaceConfirmationMethodsIndexed : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    template<int N, class T >
    struct HasSpaceConfirmationMethodsIndexed<N, T, void_t<decltype(std::declval<T>().confirmSpace(std::declval<Tag<N>>(), std::declval<LayoutStruct>(), std::declval<SpaceStateInterface::SpaceType>()))>> : std::true_type { };

    // SFINAE test
    // primary template handles types that have no nested ::type member:
    template< class, class = void_t<> >
    struct HasSpaceConfirmationMethodsIndexedDyn : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    template< class T >
    struct HasSpaceConfirmationMethodsIndexedDyn<T, void_t<decltype(std::declval<T>().confirmSpace(std::declval<ptrdiff_t>(), std::declval<LayoutStruct>(), std::declval<SpaceStateInterface::SpaceType>()))>> : std::true_type { };

    /** \brief a mini tester class... */



    struct HasSpaceConfirmationMethodsTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasspaceconfirmationmethods_test.h"
#endif


#endif
