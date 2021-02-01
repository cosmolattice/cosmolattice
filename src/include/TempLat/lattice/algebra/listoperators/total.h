#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_TOTAL_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_TOTAL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/util/tdd/tdd.h"
#include "binaryfold.h"
#include "TempLat/lattice/algebra/operators/add.h"
#include "listadd.h"


namespace TempLat {


    /** \brief A function which applies + to the element of a list, elementwise. Specialisation of binary_fold.
     *
     * 
     * 
     * Unit test: make test-total
     **/

    class TotalTester {

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    template <class Tuple, typename Function>
    constexpr auto total(Tuple&& tup, Function&& func)
    {
        return binary_fold([](auto x, auto y){return x+y;},std::forward<Tuple>(tup),std::forward<Function>(func),ZeroType());
    }

    template <class Tuple>
    constexpr auto total(Tuple&& tup)
    {
        return binary_fold([](auto x, auto y){return x+y;},std::forward<Tuple>(tup),[](auto x){return x;},ZeroType());
    }




} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/listoperators/total_test.h"
#endif


#endif
