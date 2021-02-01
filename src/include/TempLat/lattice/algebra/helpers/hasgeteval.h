#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETEVAL_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETEVAL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"


namespace TempLat {


    /** \brief SFINAE to detect wether something has a getEval(ptrdiff_t i) method.
     * 
     * 
     * Unit test: make test-hasgeteval
     **/

    template< class, class = void_t<> >
    struct HasGetEval : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    template< class T >
    struct HasGetEval<T, void_t<decltype(std::declval<T>().getEval(std::declval<ptrdiff_t >()))>> : std::true_type { };


    struct HasGetEvalTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasgeteval_test.h"
#endif


#endif
