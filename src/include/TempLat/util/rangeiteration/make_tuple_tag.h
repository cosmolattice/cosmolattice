#ifndef TEMPLAT_UTIL_RANGEITERATION_MAKE_TUPLE_TAG_H
#define TEMPLAT_UTIL_RANGEITERATION_MAKE_TUPLE_TAG_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat {


    /** \brief Function that creates a tuple of (f(Tag<0>, ..., f(Tag<N>) ).
     * 
     * 
     * Unit test: make test-make_tuple_tag
     **/

    template <int Start, typename F, int... I>
    inline auto make_tuple_tag_impl(F&& f, std::integer_sequence<int,I...> iseq) {
        return std::make_tuple(f(Tag<I+Start>())...);
    }
    template <int Start , int End  ,typename F>
    inline auto make_tuple_tag(F&& f) {
        return make_tuple_tag_impl<Start>(std::forward<F>(f), std::make_integer_sequence<int,End-Start>());
    }

    template <int End  ,typename F>
    inline auto make_tuple_tag(F&& f) {
        return make_tuple_tag_impl<0>(std::forward<F>(f), std::make_integer_sequence<int,End>());
    }

#define MakeTupleFromTag(i,start,end,expr) make_tuple_tag<start,end+1>([&](auto i){return expr;})


    class make_tuple_tagTester {
    public:
        /* Put public methods here. These should change very little over time. */
        make_tuple_tagTester() {

        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/rangeiteration/make_tuple_tag_test.h"
#endif


#endif
