#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISTempLatGETTABLE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISTempLatGETTABLE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat {


    /** \brief A class which which checks for the existence of getComp-
     *
     * 
     * Unit test: make test-istemplatgettable
     **/
    template<int, class, class = void_t<> >
    struct IsTempLatGettable : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    template<int N, class T>
    struct IsTempLatGettable<N, T, void_t<std::remove_const<typename std::remove_reference<decltype(std::declval<T>().getComp(std::declval<Tag<N>>()))>>>> : std::true_type {
    };

    struct IsTempLatGettableTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };




} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/istemplatgettable_test.h"
#endif


#endif
