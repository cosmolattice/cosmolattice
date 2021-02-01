#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDOWENEEDGHOSTS_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDOWENEEDGHOSTS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019


#include <type_traits>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat {



  /** \brief SFNIAE test to see if the doWeNeedGhost method is defined.
   *
   *
   * Unit test: make test-hasdoweneedghosts
   **/


   template< class, class = void_t<> >
   struct HasDoWeNeedGhostsMethod : std::false_type { };

   // specialization recognizes types that do have a nested ::type member:
   template< class T >
   struct HasDoWeNeedGhostsMethod<T, void_t<decltype(std::declval<T>().doWeNeedGhosts())>> : std::true_type { };

   template< int, class, class = void_t<> >
   struct HasDoWeNeedGhostsMethodIndexed : std::false_type { };

   // specialization recognizes types that do have a nested ::type member:
   template<int N,  class T >
   struct HasDoWeNeedGhostsMethodIndexed<N,T, void_t<decltype(std::declval<T>().doWeNeedGhosts(std::declval<Tag<N>>()))>> : std::true_type { };

    template< class, class = void_t<> >
    struct HasDoWeNeedGhostsMethodIndexedDyn : std::false_type { };

    // specialization recognizes types that do have a nested ::type member:
    template< class T >
    struct HasDoWeNeedGhostsMethodIndexedDyn<T, void_t<decltype(std::declval<T>().doWeNeedGhosts(std::declval<ptrdiff_t>()))>> : std::true_type { };



struct HasDoWeNeedGhostsTester {
  public:
#ifdef TEMPLATTEST
      static inline void Test(TDDAssertion& tdd);
#endif
  };


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasdoweneedghosts_test.h"
#endif


#endif
