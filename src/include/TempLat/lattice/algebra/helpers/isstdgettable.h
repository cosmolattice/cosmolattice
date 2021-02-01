#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISSTDGETTABLE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISSTDGETTABLE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat {


    /** \brief A class which checks compatibility with std::get.
     *
     * 
     * Unit test: make test-isstdgettable
     **/

        template<int, class, class = void_t<> >
        struct IsSTDGettable : std::false_type {
        };
        // specialization recognizes types that do have a nested ::type member:
        template<int N, class T>
        struct IsSTDGettable<N, T, void_t<decltype(std::get<N>(std::declval<T>()))>> : std::true_type {
        };



    struct IsSTDGettableTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/isstdgettable_test.h"
#endif


#endif
