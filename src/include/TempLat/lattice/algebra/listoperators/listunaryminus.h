#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTUNARYMINUS_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTUNARYMINUS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/static_max.h"
#include "TempLat/util/tuple_size.h"
#include "TempLat/util/tdd/tdd.h"
#include "listunaryoperator.h"
#include "TempLat/lattice/algebra/conditional/conditionallistunarygetter.h"
#include "TempLat/lattice/algebra/operators/unaryminus.h"

namespace TempLat {


    /** \brief A class which adds two getters. Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
     *
     * Unit test: make test-multiply
     **/
    template<typename R>
    class ListUnaryMinus : public ListUnaryOperator<R> {
    public:
        using ListUnaryOperator<R>::mR;
        ListUnaryMinus(const R& pR): ListUnaryOperator<R>(pR){}


        template <int N>
        auto getComp(Tag<N> t){
            return -GetComponent::get(mR, t);
        }

        virtual std::string operatorString() const {
            return "-";
        }
        template <int N>
        void doWeNeedGhosts(Tag<N> i)
        {
            GhostsHunter::apply(mR,i);
        }
        static const size_t size = tuple_size<R>::value;
    };

    /** \brief A mini struct for instiating the test case. */
    struct ListUnaryMinusTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<typename R>
    typename ConditionalListUnaryGetter<ListUnaryMinus, R>::type
    //ListUnaryMinus<R>
    operator-(const R& r)
    {
        return ListUnaryMinus<R>(r);
    }





} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/listoperators/listunaryminus_test.h"
#endif


#endif
