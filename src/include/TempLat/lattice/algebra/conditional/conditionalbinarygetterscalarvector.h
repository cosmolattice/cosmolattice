#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTERSCALARVECTOR_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTERSCALARVECTOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasvectorgetmethod.h"

namespace TempLat {


    /** \brief A class which is part of the implementation of the scalar multiplication for the vector algebra.
     *
     *
     * Unit test: make test-conditionalbinarygetterscalarvector
     **/
     struct ConditionalBinaryGetterScalarVectorTester {
         static void Test(TempLat::TDDAssertion&);
     };

     /** \brief Default is ENABLE == false: no type def. */
     template <template <typename OPT, typename OPS> class OP, typename S, typename T, bool ENABLE>
     struct ConditionalBinaryGetterScalarVectorHelper {
     };

     /** \brief Default is ENABLE == true: typedef the desired operator. */
     template <template <typename OPT, typename OPS> class OP, typename S, typename T>
     struct ConditionalBinaryGetterScalarVectorHelper <OP, S, T, true> {
         typedef OP<S, T> type;
     };
     template <template <typename OPT, typename OPS> class OP, typename S, typename T, bool OVERRIDE = true>
     struct ConditionalBinaryGetterScalarVector : public ConditionalBinaryGetterScalarVectorHelper<OP, S, T, ((HasVectorGetMethod<S>::value && !HasVectorGetMethod<T>::value) || (!HasVectorGetMethod<S>::value && HasVectorGetMethod<T>::value)) && OVERRIDE> {
     };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/conditional/conditionalbinarygetterscalarvector_test.h"
#endif


#endif
