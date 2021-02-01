#ifndef COSMOINTERFACE_SU2ALGEBRA_HELPERS_HASSU2DOUBLETGET_H
#define COSMOINTERFACE_SU2ALGEBRA_HELPERS_HASSU2DOUBLETGET_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat {


    /** \brief A class which checks whether the object has a su2doubletget method or not.
     *
     * 
     * Unit test: make test-hassu2doubletget
     **/

    struct HasSU2DoubletGetTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template < class, class = void_t<> >
    struct HasSU2DoubletGet:  std::false_type{
    };

    template < class T >
    struct HasSU2DoubletGet<T, void_t< decltype(std::declval<std::remove_cv_t<T>>().SU2DoubletGet(std::declval<Tag<0>>())) > >:  std::true_type{
    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "hassu2doubletget_test.h"
#endif


#endif
