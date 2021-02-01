#ifndef TEMPLAT_UTIL_RANGEITERATION_SUM_IN_RANGE_H
#define TEMPLAT_UTIL_RANGEITERATION_SUM_IN_RANGE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/make_tuple_tag.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/operators/add.h"
#include "TempLat/lattice/algebra/listoperators/total.h"


namespace TempLat {


    /** \brief A class which "reduce", "sums over" tuple-like objects.
     *
     *
     * Unit test: make test-sum_in_range
     **/

     //TODO: implement binary_in_range (used with a lambda) and see if there is a difference.
     // Extra : need to give as template parameter the identity corresponding to the operator.

     //NOTE: Actually, available in c++17 through fold expressions! Possibly need invoke as well. 


 /*    template<typename F, int I>
     constexpr auto sum_in_range_impl(F&& func, Tag<I> t1, Tag<-1> t2)
     {
       return ZeroType();
     }

     template<typename F, int I, int J>
     constexpr auto sum_in_range_impl(F&& func, Tag<I> t1, Tag<J> t2)
     {
       return func(t1) + sum_in_range_impl(std::forward<F>(func),Tag<I-1>(), Tag<J-1>());
     }

     template<int begin, int end, typename F>
     constexpr auto sum_in_range(F&& func)
     {
       return sum_in_range_impl(std::forward<F>(func), Tag<end-1>(), Tag<end-1-begin>());
     }*/

    template<int begin, int end, typename F>
    constexpr auto sum_in_range(F&& func)
    {
        constexpr bool test = end>=begin;
        return static_if<test>(total(make_tuple_tag<begin,end>(std::forward<F>(func))),ZeroType());
    }

     class sum_in_range_Tester {

     public:
#ifdef TEMPLATTEST
         static inline void Test(TDDAssertion& tdd);
#endif
     };

#define Total(i,beg,end,expr) sum_in_range<beg,end+1>([&](auto i){return expr;})

} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/rangeiteration/sum_in_range_test.h"
#endif


#endif
