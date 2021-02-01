#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETCOMPONENT_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETCOMPONENT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/helpers/isstdgettable.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"


namespace TempLat {

    /** \brief A class which calls getComp.
     *
     * 
     * Unit test: make test-getcomponent
     **/

    class GetComponent { //need two is gettable cause need to know what function to use in the return types.

    public:

        template <int N, typename U>
        static inline
        typename std::enable_if<IsTempLatGettable<N,U>::value , decltype(std::declval<U>().getComp(std::declval<Tag<N>>()))>::type
        get( U&& obj, Tag<N> t) {
            return obj.getComp(t);
        }

        template <int N, typename U>
        static inline
        typename std::enable_if<IsSTDGettable<N,U>::value && !IsTempLatGettable<N,U>::value , decltype(std::get<N>(std::declval<U>()))>::type
        get( U&& obj, Tag<N> t) {
            return std::get<N>(obj);
        }

        template <int N, typename U>
        static inline
        typename std::enable_if<!IsSTDGettable<N,U>::value && !IsTempLatGettable<N,U>::value , U>::type&
        get( U&& obj, Tag<N> t) {
            return obj;
        }

    };

    struct GetComponentTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/getcomponent_test.h"
#endif


#endif
