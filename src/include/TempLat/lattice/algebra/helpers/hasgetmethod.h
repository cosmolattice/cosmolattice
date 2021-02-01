#ifndef TEMPLAT_LATTICE_ALGEBRA_HASGETMETHOD_H
#define TEMPLAT_LATTICE_ALGEBRA_HASGETMETHOD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <type_traits>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"

namespace TempLat {

    /** \brief A SFINAE-test class which determines at compile time whether an object has a method `get`.
     *
     * Spent a lot of time reading through https://stackoverflow.com/questions/257288/is-it-possible-to-write-a-template-to-check-for-a-functions-existence?answertab=active#tab-top
     * while the definitive and perfect answer was just at:
     * https://en.cppreference.com/w/cpp/types/void_t
     * Unit test: make test-hasgetmethod
     **/

    // SFINAE test
    // primary template handles types that have no nested ::type member:


//"Simple" version, the same
/** \brief
 * Template way of checking if a class has a get method.
 *
 *
 *
 **/

    template< class, class = void_t<> >
    struct HasGetMethod : std::false_type { };

    template< class T >
    struct HasGetMethod<T, void_t<decltype(std::declval<T>().get(std::declval<ptrdiff_t>()))>> : std::true_type { };

  struct HasGetMethodTester {
  public:
#ifdef TEMPLATTEST
      static inline void Test(TDDAssertion& tdd);
#endif
  };

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasgetmethod_test.h"
#endif


#endif
