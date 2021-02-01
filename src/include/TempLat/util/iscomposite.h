#ifndef TEMPLAT_UTIL_ISCOMPOSITE_H
#define TEMPLAT_UTIL_ISCOMPOSITE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/tuple_size.h"
#include "TempLat/util/rangeiteration/tag.h"


namespace TempLat {


    /** \brief A class which allows to check whether a class is composite or not. Allows to write general function that works on composite classes.
     *
     * 
     * Unit test: make test-iscomposite
     **/

    template< class, int N >
    struct IsCompositeHelper : std::true_type { };

// specialization recognizes types that do have a nested ::type member:
    template< class T >
    struct IsCompositeHelper<T,1> : std::false_type { };

    template<typename R>
    using IsComposite = IsCompositeHelper<R,tuple_size<decltype(std::remove_reference<R>::type::Getter::get(std::declval<R&>(),std::declval<Tag<0>>()))>::value>;


    struct IsCompositeTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/iscomposite_test.h"
#endif


#endif
