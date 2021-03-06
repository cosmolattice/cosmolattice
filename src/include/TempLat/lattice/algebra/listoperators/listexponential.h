#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTEXPONENTIAL_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTEXPONENTIAL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/static_max.h"
#include "TempLat/util/tuple_size.h"
#include "TempLat/util/tdd/tdd.h"
#include "listunaryoperator.h"
#include "TempLat/lattice/algebra/conditional/conditionallistunarygetter.h"
#include "TempLat/lattice/algebra/operators/exponential.h"


namespace TempLat {


    /** \brief A class which implements the exponenetiation for the list algebra.
     *
     * 
     * Unit test: make test-listexponential
     **/

    /** \brief A class which adds two getters. Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
     *
     * Unit test: make test-multiply
     **/
    template<typename R>
    class ListExponential : public ListUnaryOperator<R> {
    public:
        using ListUnaryOperator<R>::mR;
        ListExponential(const R& pR): ListUnaryOperator<R>(pR){}


        template <int N>
        auto getComp(Tag<N> t){
            return exp(GetComponent::get(mR, t));
        }

        virtual std::string operatorString() const {
            return "exp";
        }
        template <int N>
        void doWeNeedGhosts(Tag<N> i)
        {
            GhostsHunter::apply(mR,i);
        }
        static const size_t size = tuple_size<R>::value;
    };

    /** \brief A mini struct for instiating the test case. */
    struct ListExponentialTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<typename R>
    typename ConditionalListUnaryGetter<ListExponential, R>::type
    //ListExponential<R>
    exp(const R& r)
    {
        return ListExponential<R>(r);
    }





} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/listoperators/listexponential_test.h"
#endif


#endif
