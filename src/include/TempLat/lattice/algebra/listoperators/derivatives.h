#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_DERIVATIVES_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_DERIVATIVES_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/make_tuple_tag.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class which aaplies vectorially the automatic derivative. Not used anywhere.
     *
     * 
     * Unit test: make test-derivatives
     **/

    class Derivatives {
    public:
        /* Put public methods here. These should change very little over time. */
        Derivatives() {

        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<typename T, typename R>
    auto derivatives(T&& expr, R& others)
    {
        constexpr size_t size = tuple_size<R>::value;
        return make_tuple_tag<size>([&](auto i){
            auto phi = R::Getter::get(others,i);
            return GetDeriv::get(expr,phi);
        });
    }



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/listoperators/derivatives_test.h"
#endif


#endif
