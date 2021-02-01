#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTER_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/conditional/isgettable.h"
#include "TempLat/lattice/algebra/helpers/hasvectorgetmethod.h"
#include "TempLat/lattice/algebra/helpers/hasstaticgetter.h"

namespace TempLat {


    struct ConditionalBinaryGetterTester {
        static void Test(TempLat::TDDAssertion&);
    };

    //Commodities for operators return types

    /** \brief Default is ENABLE == false: no type def. */
    template <template <typename OPT, typename OPS> class OP, typename S, typename T, bool ENABLE>
    struct ConditionalBinaryGetterHelper {
    };

    /** \brief Default is ENABLE == true: typedef the desired operator. */
    template <template <typename OPT, typename OPS> class OP, typename S, typename T>
    struct ConditionalBinaryGetterHelper <OP, S, T, true> {
        typedef OP<S, T> type;
    };
    template <template <typename OPT, typename OPS> class OP, typename S, typename T, bool OVERRIDE = true>
struct ConditionalBinaryGetter : public ConditionalBinaryGetterHelper<OP, S, T, ((HasGetMethod<S>::value || HasGetMethod<T>::value) && ! HasVectorGetMethod<S>::value && ! HasVectorGetMethod<T>::value && ! HasStaticGetter<S>::value && ! HasStaticGetter<T>::value) && OVERRIDE> {
    };


}







#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/conditional/conditionalbinarygetter_test.h"
#endif







#endif
