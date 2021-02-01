#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETGETRETURNTYPE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETGETRETURNTYPE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <type_traits>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/iscomplextype.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"

namespace TempLat {

    /** \brief get(IterationCoordinates&) might be a function which returns a real value or a complex
     * value. Sometimes we need to explicitly access what type it is.
     *
     *
     * Unit test: make test-getgetreturntype
     **/

    template <typename T>
    struct GetGetReturnType {
        /* why not declval for the IterationCoordinates? Well, get takes a reference, which cannot be
         * a temporary. So we dereference a NULL pointer, which is a value reference.
         * Safe, because it is only at compile time. */
        typedef typename std::remove_const<typename std::remove_reference<decltype(GetValue::get(std::declval<typename std::remove_reference<T>::type>(),std::declval<int>()))>::type>::type type;
        static constexpr bool isComplex = IsComplexType<type>::value;
    };

    struct GetGetReturnTypeTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/getgetreturntype_test.h"
#endif


#endif
