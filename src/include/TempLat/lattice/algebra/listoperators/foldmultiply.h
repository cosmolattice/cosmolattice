#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_FOLDMULTIPLY_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_FOLDMULTIPLY_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "binaryfold.h"
#include "total.h"


namespace TempLat {


    /** \brief A class which
     *  reduces a list over multiplication. Specialisation of binary fold.
     * 
     * 
     * Unit test: make test-foldmultiply
     **/

    struct FoldMultiplyTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template <class Tuple, typename Function>
    constexpr auto fold_multiply(Tuple&& tup, Function&& func)
    {
        return binary_fold([](auto x, auto y){return x*y;},std::forward<Tuple>(tup),std::forward<Function>(func),OneType());
    }

    template <class Tuple>
    constexpr auto fold_multiply(Tuple&& tup)
    {
        constexpr bool test = tuple_size<Tuple>::value>0;
        return static_if<test>(binary_fold([](auto x, auto y){return x*y;},std::forward<Tuple>(tup),[](auto x){return x;},OneType()),OneType());
    }

    template <class Tuple, class Unit>
    constexpr auto fold_multiply_unit(Tuple&& tup, Unit&& unit)
    {
        constexpr bool test = tuple_size<Tuple>::value>0;
        return static_if<test>(binary_fold([](auto x, auto y){return x*y;},std::forward<Tuple>(tup),[](auto x){return x;},std::forward<Unit>(unit)),OneType());
    }



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/listoperators/foldmultiply_test.h"
#endif


#endif
