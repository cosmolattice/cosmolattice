#ifndef COSMOINTERFACE_SU2ALGEBRA_HELPERS_SU2GETGETRETURNTYPE_H
#define COSMOINTERFACE_SU2ALGEBRA_HELPERS_SU2GETGETRETURNTYPE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2get.h"


namespace TempLat {


    /** \brief A class which determines what is the return type of the composite su2 objects
     *
     * 
     * Unit test: make test-su2getgetreturntype
     **/

    template <typename T>
    struct SU2GetGetReturnType {
        typedef typename GetGetReturnType<typename std::remove_const<typename std::remove_reference<decltype(SU2Getter::get(std::declval<T>(),std::declval<Tag<1>>()))>::type>::type>::type type; //Note: we check the type of the field 1 and not 0, as 1 is really an object.
        static constexpr bool isComplex = IsComplexType<type>::value;
    };

    struct SU2GetGetReturnTypeTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "su2getgetreturntype_test.h"
#endif


#endif
