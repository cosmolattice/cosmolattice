#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTER_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"
#include "TempLat/util/rangeiteration/tag.h"


namespace TempLat {


    /** \brief A class which can be used to detect whether something is a composite object or not.
     *
     * 
     * Unit test: make test-hasstaticgetter
     **/

    template< class, class = void_t<> >
    struct HasStaticGetter : std::false_type { };

    template< class T >
    struct HasStaticGetter<T, void_t<decltype(T::Getter::get(std::declval<T>(),std::declval<Tag<0>>()))>> : std::true_type { };

    struct HasStaticGetterDummy {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasstaticgetter_test.h"
#endif


#endif
