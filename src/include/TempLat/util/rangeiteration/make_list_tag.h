#ifndef TEMPLAT_UTIL_MAKELISTTAG_H
#define TEMPLAT_UTIL_MAKELISTTAG_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/assignabletuple.h"

namespace TempLat {


    /** \brief A class which
     *
     *
     * Unit test: make test-make_list_tag
     **/

    template <int Start, typename F, int... I>
    inline auto make_list_tag_impl(F&& f, std::integer_sequence<int,I...> iseq) {
        return make_list(f(Tag<I+Start>())...);
    }
    template <int Start , int End  ,typename F>
    inline constexpr auto make_list_tag(F&& f) {
        constexpr bool test = End > Start;
        return static_if<test>(make_list_tag_impl<Start>(std::forward<F>(f), std::make_integer_sequence<int,End-Start>()), std::tuple<>());
    }

    template <int End  ,typename F>
    inline constexpr auto make_list_tag(F&& f) {
        constexpr bool test = End > 0;
        return static_if<test>(make_list_tag_impl<0>(std::forward<F>(f), std::make_integer_sequence<int,End>()), std::tuple<>());
    }

    template <int Start, typename F, int... I>
    inline auto make_vector_tag_impl(F&& f, std::integer_sequence<int,I...> iseq) {
        return make_vector(f(Tag<I+Start>())...);
    }
    template <int Start , int End  ,typename F>
    inline constexpr auto make_vector_tag(F&& f) {
        constexpr bool test = End > Start;
        return static_if<test>(make_vector_tag_impl<Start>(std::forward<F>(f), std::make_integer_sequence<int,End-Start>()), std::tuple<>());
    }

    template <int End  ,typename F>
    inline constexpr auto make_vector_tag(F&& f) {
        constexpr bool test = End > 0;
        return static_if<test>(make_vector_tag_impl<0>(std::forward<F>(f), std::make_integer_sequence<int,End>()),std::tuple<>());
    }



    struct MakeListTagTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

#define MakeArray(i,beg, end, expr) make_list_tag<beg,end+1>([&](auto i){{return expr;};})
#define MakeVector(i,beg, end, expr) make_vector_tag<beg,end+1>([&](auto i){return expr;})


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/rangeiteration/make_list_tag_test.h"
#endif


#endif
