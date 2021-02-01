#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_COMPLEXGETGETRETURNTYPE_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_COMPLEXGETGETRETURNTYPE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat {


    /** \brief A class which unpack the return type of a complex type.
     *
     * 
     * Unit test: make test-complexgetgetreturntype
     **/

    template <typename T>
    struct ComplexGetGetReturnType {
        /* why not declval for the IterationCoordinates? Well, get takes a reference, which cannot be
         * a temporary. So we dereference a NULL pointer, which is a value reference.
         * Safe, because it is only at compile time. */
        typedef typename GetGetReturnType<typename std::remove_const<typename std::remove_reference<decltype(std::declval<T>().ComplexFieldGet(std::declval<Tag<0>>()))>::type>::type>::type type;
        static constexpr bool isComplex = IsComplexType<type>::value;
    };

    struct ComplexGetGetReturnTypeTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexgetgetreturntype_test.h"
#endif


#endif
