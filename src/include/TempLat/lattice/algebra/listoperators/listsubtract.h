#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_VECTORSUBSTRACT_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_VECTORSUBSTRACT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/static_max.h"
#include "TempLat/util/tuple_size.h"
#include "TempLat/util/tdd/tdd.h"
#include "listbinaryoperator.h"
#include "TempLat/lattice/algebra/conditional/conditionallistbinarygetter.h"
#include "TempLat/lattice/algebra/operators/subtract.h"



namespace TempLat {

    /** \brief A class which adds two getters. Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
     *
     * Unit test: make test-multiply
     **/
    template<typename R, typename T>
    class ListSubtraction : public ListBinaryOperator<R,T> {
    public:
        using ListBinaryOperator<R,T>::mR;
        using ListBinaryOperator<R,T>::mT;
        ListSubtraction(const R& pR, const T& pT): ListBinaryOperator<R,T>(pR, pT){}


        template <int N>
        auto getComp(Tag<N> t){
            return GetComponent::get(mR, t) - GetComponent::get(mT, t);
        }

        virtual std::string operatorString() const {
            return "-";
        }
        template <int N>
        void doWeNeedGhosts(Tag<N> i)
        {
            GhostsHunter::apply(mR,i);
            GhostsHunter::apply(mT,i);
        }
        static const size_t size = static_max<tuple_size<R>::value, tuple_size<T>::value >::value;
    };




    /** \brief A mini struct for instiating the test case. */
    struct ListSubtractTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<typename R, typename T>
    typename ConditionalListBinaryGetter<ListSubtraction, R, T>::type
    //ListSubtraction<R,T>
    operator-(const R& r, const T& t)
    {
        return ListSubtraction<R,T>(r, t);
    }


}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/listoperators/listsubtract_test.h"
#endif


#endif
