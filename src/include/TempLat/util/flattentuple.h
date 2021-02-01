#ifndef TEMPLAT_UTIL_FLATTENTUPLE_H
#define TEMPLAT_UTIL_FLATTENTUPLE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class which flatten all directions of a composite object.
     *
     * 
     * Unit test: make test-flattentuple
     **/

    // from https://stackoverflow.com/a/32166787
    // credit Piotr Skotnicki

    template <typename T>
    auto explode(T&& t, char)
    {
        return std::forward_as_tuple(std::forward<T>(t));
    }

    template <typename T, std::size_t I = std::tuple_size<std::decay_t<T>>{}>
    auto explode(T&& t, int);

    template <typename T, std::size_t... Is>
    auto explode(T&& t, std::index_sequence<Is...>)
    {
        return std::tuple_cat(explode(std::get<Is>(std::forward<T>(t)), 0)...);
    }

    template <typename T, std::size_t I>
    auto explode(T&& t, int)
    {
        return explode(std::forward<T>(t), std::make_index_sequence<I>{});
    }

    template <typename T, std::size_t... Is>
    auto decay_tuple(T&& t, std::index_sequence<Is...>)
    {
        return std::make_tuple(std::get<Is>(std::forward<T>(t))...);
    }

    template <typename T>
    auto decay_tuple(T&& t)
    {
        return decay_tuple(std::forward<T>(t), std::make_index_sequence<std::tuple_size<std::decay_t<T>>{}>{});
    }

//Changed the original name from merge_tuple to flattent_tuple.
    template <typename T, std::size_t... Is>
    auto flatten_tuple(T&& t, std::index_sequence<Is...>)
    {
        return decay_tuple(std::tuple_cat(explode(std::get<Is>(std::forward<T>(t)), 0)...));
    }

    template <typename T>
    auto flatten_tuple(T&& t)
    {
        return flatten_tuple(std::forward<T>(t), std::make_index_sequence<std::tuple_size<std::decay_t<T>>{}>{});
    }


    class FlattenTupleTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/flattentuple_test.h"
#endif


#endif
