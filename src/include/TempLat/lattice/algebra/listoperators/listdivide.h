#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTDIVIDE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTDIVIDE_H
 
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
#include "TempLat/lattice/algebra/operators/divide.h"


namespace TempLat {


    template<typename R, typename T>
    class ListDivision : public ListBinaryOperator<R,T> {
    public:
        using ListBinaryOperator<R,T>::mR;
        using ListBinaryOperator<R,T>::mT;
        ListDivision(const R& pR, const T& pT): ListBinaryOperator<R,T>(pR, pT){}


        template <int N>
        auto getComp(Tag<N> t){
            return GetComponent::get(mR, t) / GetComponent::get(mT, t);
        }

        virtual std::string operatorString() const {
            return "/";
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
    struct ListDivisionTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<typename R, typename T>
    typename ConditionalListBinaryGetter<ListDivision, R, T>::type
    //ListDivision<R,T>
    operator/(const R& r, const T& t)
    {
        return ListDivision<R,T>(r, t);
    }


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/listoperators/listdivide_test.h"
#endif


#endif
