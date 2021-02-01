#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEVAL_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEVAL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"


namespace TempLat {


    /** \brief SFINAE  class to detect if the haseval method has been defined.
     *
     * 
     * Unit test: make test-haseval
     **/

    template< class, class = void_t<> >
    struct HasEval : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    template< class T >
    struct HasEval<T, void_t<decltype(std::declval<T>().eval(std::declval<ptrdiff_t >()))>> : std::true_type { };

    struct HasEvalTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/haseval_test.h"
#endif


#endif
