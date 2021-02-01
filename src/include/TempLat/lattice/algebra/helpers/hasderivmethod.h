#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDERIVMETHOD_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDERIVMETHOD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio, originally by  Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"

namespace TempLat {



     /** \brief A SFINAE-test class which finds out whether a method has  derivatives implemented.
      **/

     // SFINAE test
     // primary template handles types that have no nested ::type member:
     template< class, class = void_t<> >
     struct HasDerivMethod : std::false_type { };

     // specialization recognizes types that do have a nested ::type member:
     template< class T >
     struct HasDerivMethod<T, void_t<decltype(std::declval<T>().d(0))>> : std::true_type { };

     /** \brief a mini tester class... */
     struct HasDerivMethodTester {
#ifdef TEMPLATTEST
         static inline void Test(TDDAssertion& tdd);
#endif
     };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasderivmethod_test.h"
#endif


#endif
