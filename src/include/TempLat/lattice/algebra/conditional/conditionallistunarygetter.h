#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALLISTUNARYGETTER_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALLISTUNARYGETTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"
namespace TempLat {


    /** \brief A class which is part of the implementation of the list algebra. Conditionnaly returns some binary operation type.     *
     * 
     * Unit test: make test-conditionallistunarygetter
     **/


    struct ConditionalListUnaryGetterTester {
        static void Test(TempLat::TDDAssertion&);
    };

    /** \brief Default is ENABLE == false: no type def. */
    template <template <typename OPT> class OP, typename S, bool ENABLE>
    struct ConditionalListUnaryGetterHelper {
    };

    /** \brief Default is ENABLE == true: typedef the desired operator. */
    template <template <typename OPT> class OP, typename S>
    struct ConditionalListUnaryGetterHelper <OP, S,  true> {
        typedef OP<S> type;
    };
    template <template <typename OPT> class OP, typename S, bool OVERRIDE = true>
    struct ConditionalListUnaryGetter : public ConditionalListUnaryGetterHelper<OP, S, (IsSTDGettable<0,S>::value || IsTempLatGettable<0,S>::value ) && OVERRIDE> {
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/conditional/conditionallistunarygetter_test.h"
#endif


#endif
