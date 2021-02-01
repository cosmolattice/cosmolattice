#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALUNARYGETTER_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALUNARYGETTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/conditional/isgettable.h"
//#include "TempLat/lattice/algebra/getter.h"

namespace TempLat {

    /** \brief Default is ENABLE == false: no type def. */
    template <template <typename OPT> class OP, typename T, bool ENABLE>
    struct ConditionalUnaryGetterHelper {
    };

    /** \brief Default is ENABLE == true: typedef the desired operator. */
    template <template <typename OPT> class OP, typename T>
    struct ConditionalUnaryGetterHelper <OP, T, true> {
        typedef OP<T> type;
    };


    /** \brief A struct which is used to enably our operator overloads only when the input type
     *  has a get method. This avoids overriding the unary operators
     *  everywhere else. We don't want to override operator-(double), for example.
     *
     * Unit test: make test-conditionalunarygetter
     **/
    template <template <typename OPT> class OP, typename T>
    struct ConditionalUnaryGetter : public ConditionalUnaryGetterHelper<OP, T, IsGettable<T>::value> {
    };

    struct ConditionalUnaryGetterTester {
        static void Test(TempLat::TDDAssertion&);
    };

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter_test.h"
#endif


#endif
