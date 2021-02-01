#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTERVECTOR_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTERVECTOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"


namespace TempLat {


    /** \brief A class which is part of the implementation of the list algebra. Conditionnaly returns some binary operation type.
     *
     *
     * Unit test: make test-conditionalbinarygetterscalarvector
     **/
     struct ConditionalListBinaryGetterTester {
         static void Test(TempLat::TDDAssertion&);
     };

     /** \brief Default is ENABLE == false: no type def. */
     template <template <typename OPT, typename OPS> class OP, typename S, typename T, bool ENABLE>
     struct ConditionalListBinaryGetterHelper {
     };

     /** \brief Default is ENABLE == true: typedef the desired operator. */
     template <template <typename OPT, typename OPS> class OP, typename S, typename T>
     struct ConditionalListBinaryGetterHelper <OP, S, T, true> {
         typedef OP<S, T> type;
     };
     template <template <typename OPT, typename OPS> class OP, typename S, typename T, bool OVERRIDE = true>
     struct ConditionalListBinaryGetter : public ConditionalListBinaryGetterHelper<OP, S, T, (IsSTDGettable<0,S>::value || IsSTDGettable<0,T>::value || IsTempLatGettable<0,S>::value || IsTempLatGettable<0,T>::value ) && OVERRIDE> {
     };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/conditional/conditionallistbinarygetter_test.h"
#endif


#endif
