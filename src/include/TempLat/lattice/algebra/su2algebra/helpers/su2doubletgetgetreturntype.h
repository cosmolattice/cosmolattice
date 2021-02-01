#ifndef COSMOINTERFACE_SU2ALGEBRA_HELPERS_SU2DOUBLETGETGETRETURNTYPE_H
#define COSMOINTERFACE_SU2ALGEBRA_HELPERS_SU2DOUBLETGETGETRETURNTYPE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/util/rangeiteration/tag.h"
#include "su2doubletget.h"
namespace TempLat {


    /** \brief A class which gives the return type of the get function of one of the doublet element.
     *
     * 
     * Unit test: make test-su2doubletgetgetreturntype
     **/

    template <typename T>
    struct SU2DoubletGetGetReturnType {
        /* why not declval for the IterationCoordinates? Well, get takes a reference, which cannot be
         * a temporary. So we dereference a NULL pointer, which is a value reference.
         * Safe, because it is only at compile time. */
        typedef typename GetGetReturnType<typename std::remove_const<typename std::remove_reference<decltype(SU2DoubletGetter::get(std::declval<T>(),std::declval<Tag<0>>()))>::type>::type>::type type;
        static constexpr bool isComplex = IsComplexType<type>::value;
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "su2doubletgetgetreturntype_test.h"
#endif


#endif
