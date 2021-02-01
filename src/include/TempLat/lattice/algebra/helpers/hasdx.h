#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDX_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDX_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"

namespace TempLat {


    /** \brief
     * SFNIAE to detect whether something has a getDx() method or not.
     *
     *
     * Unit test: make test-hasdx
     **/

     template< class, class = void_t<> >
     struct HasDx : std::false_type { };

     // specialization recognizes types that do have a nested ::type member:
     template< class T >
     struct HasDx<T, void_t<decltype(std::declval<typename std::remove_cv<typename std::remove_reference<T>::type>::type>().getDx())>> : std::true_type { };

     struct HasDxTester {
     public:
#ifdef TEMPLATTEST
         static inline void Test(TDDAssertion& tdd);
#endif
     };

} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasdx_test.h"
#endif


#endif
