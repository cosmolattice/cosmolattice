#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTPOWER_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTPOWER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/static_max.h"
#include "TempLat/util/tuple_size.h"
#include "TempLat/util/tdd/tdd.h"
#include "listbinaryoperator.h"
#include "listunaryoperator.h"
#include "TempLat/lattice/algebra/conditional/conditionallistbinarygetter.h"
#include "TempLat/lattice/algebra/conditional/conditionallistunarygetter.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"


namespace TempLat {


    template<typename R, typename T>
    class ListPower : public ListBinaryOperator<R,T> {
    public:
        using ListBinaryOperator<R,T>::mR;
        using ListBinaryOperator<R,T>::mT;

        ListPower(const R& pR, const T& pT): ListBinaryOperator<R,T>(pR, pT){}


        template <int N>
        auto getComp(Tag<N> t){
            using namespace std;
            return pow(GetComponent::get(mR, t) , GetComponent::get(mT, t));
        }

        virtual std::string operatorString() const {
            return "^";
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
    struct ListPowerTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<typename R, typename T>
    typename ConditionalListBinaryGetter<ListPower, R, T>::type
    //ListPower<R,T>
    pow(const R& r, const T& t)
    {
        return ListPower<R,T>(r, t);
    }




    template<int N, typename R>
    class ListPowerN : public ListUnaryOperator<R>{
    public:
        using ListUnaryOperator<R>::mR;
        ListPowerN(const R& pR): ListUnaryOperator<R>(pR){}
        template <int M>
        auto getComp(Tag<M> t){
            using namespace std;
            return pow<N>(GetComponent::get(mR, t));
        }
        template<int M>
        std::string toString(Tag<M> i) const {

            return GetString::get(mR,i) + "^" + std::to_string(N);
        }

        template <int M>
        void doWeNeedGhosts(Tag<M> i)
        {
            GhostsHunter::apply(mR,i);
        }
        static const size_t size = tuple_size<R>::value;


    };
    template<int N, typename R>
    typename std::enable_if<IsTempLatGettable<0,R>::value || IsSTDGettable<0,R>::value , ListPowerN<N,R>>::type
    pow(const R& r)
    {
        return ListPowerN<N,R>(r);
    }

} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/listoperators/listpower_test.h"
#endif


#endif
