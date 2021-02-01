#ifndef TEMPLAT_UTIL_FOREACH_H
#define TEMPLAT_UTIL_FOREACH_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/tuple_size.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"

namespace TempLat {


    /** \brief A class which
     *
     *
     * A tuple friendly for_each. From https://codereview.stackexchange.com/a/163802
     **/


     template <typename Tuple, typename F, std::size_t ...Indices>
     constexpr void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>) {
         using swallow = int[];
         using namespace std;
         (void)swallow{1,
             (f(GetComponent::get(std::forward<Tuple>(tuple), Tag<Indices>())), void(), int{})...
         };
     }




     template <typename Tuple, typename F>
     constexpr void for_each(Tuple&& tuple, F&& f) {
         using namespace std;
         constexpr std::size_t N = tuple_size<std::remove_reference_t<Tuple>>::value;
         for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f),
                       std::make_index_sequence<N>{});
     }


     template <typename Tuple1, typename Tuple2, typename F, std::size_t ...Indices>
     constexpr void binary_for_each_impl(Tuple1&& tuple1, Tuple2&& tuple2, F&& f, std::index_sequence<Indices...>) {
         using swallow = int[];
         (void)swallow{1,
             (f(std::get<Indices>(std::forward<Tuple1>(tuple1)),std::get<Indices>(std::forward<Tuple2>(tuple2))), void(), int{})...
         };
     }

     template <typename Tuple1, typename Tuple2, typename F>
     constexpr void binary_for_each(Tuple1&& tuple1, Tuple2&& tuple2, F&& f) {
         constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple1>>::value;
         binary_for_each_impl(std::forward<Tuple1>(tuple1),std::forward<Tuple2>(tuple2), std::forward<F>(f),
                       std::make_index_sequence<N>{});
     }

    class ForEachTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/foreach_test.h"
#endif


#endif
