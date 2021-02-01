#ifndef TEMPLAT_UTIL_ISTUPLELIKE_H
#define TEMPLAT_UTIL_ISTUPLELIKE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include <type_traits>
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"

namespace TempLat {


    /** \brief A class which tests whether or not a class behaves like a tuple.
     *
     *
     * Unit test: make test-istuplelike
     **/

    template< class, class = void_t<> >
    struct IsTupleLike : std::false_type { };

    template< class T >
    struct IsTupleLike<T, void_t<typename std::tuple_size<T>::type> > : std::true_type { };

  struct IsTupleLikeTester {
  public:
#ifdef TEMPLATTEST
      static inline void Test(TDDAssertion& tdd);
#endif
  };




} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/istuplelike_test.h"
#endif


#endif
