#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASKIR_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASKIR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"

namespace TempLat {


    /** \brief A SFINAE class to detect if the getKIR method is defined.
     *
     *
     * Unit test: make test-haskir
     **/

     template< class, class = void_t<> >
     struct HasKIR : std::false_type { };

     // specialization recognizes types that do have a nested ::type member:
     template< class T >
     struct HasKIR<T, void_t<decltype(std::declval<typename std::remove_cv<typename std::remove_reference<T>::type>::type>().getKIR())>> : std::true_type { };

     struct HasKIRTester {
     public:
#ifdef TEMPLATTEST
         static inline void Test(TDDAssertion& tdd);
#endif
     };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/haskir_test.h"
#endif


#endif
