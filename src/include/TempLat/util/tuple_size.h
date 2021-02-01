#ifndef TEMPLAT_UTIL_TUPLE_SIZE_H
#define TEMPLAT_UTIL_TUPLE_SIZE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"

namespace TempLat {


    /** \brief A class which overloads tuple_size for the fcn composite objects.
     *
     * 
     * Unit test: make test-tuple_size
     **/

    template <typename Tuple, class = void_t<>>
    struct tuple_size_helper{
        static const size_t value = 1;
    };

    template <typename Tuple>
    struct tuple_size_helper<Tuple,void_t< decltype(std::tuple_size<Tuple>::value)>>{
        static const size_t value = std::tuple_size<Tuple>::value;
    };

    template <typename Tuple, class = void_t<>>
    struct tuple_size{
        static const size_t value = tuple_size_helper<Tuple>::value;
    };

    template <typename Tuple>
    struct tuple_size<Tuple, void_t<decltype(Tuple::size)> >{
    static const size_t value = Tuple::size;
};

    class tuple_sizeTester{
    public:
        /* Put public methods here. These should change very little over time. */
        tuple_sizeTester() {

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
#include "TempLat/util/tuple_size_test.h"
#endif


#endif
