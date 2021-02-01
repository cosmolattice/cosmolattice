#ifndef COSMOINTERFACE_GAUGEALGEBRA_FORWARDCOVARIANTDERIVATIVE_H
#define COSMOINTERFACE_GAUGEALGEBRA_FORWARDCOVARIANTDERIVATIVE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/assignabletuple.h"
#include "TempLat/lattice/algebra/listoperators/foldmultiply.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"


namespace TempLat {


    /** \brief A class which computes the forward covariant derivative with an arbitrary number of gauge fields.
     *
     * 
     * Unit test: make test-forwardcovariantderivative
     **/

    class ForwardCovariantDerivativeTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
/*
    template<size_t dim, class... Args>
    auto ForwardCovariantDerivative(Args... args)
    {
        constexpr size_t size = sizeof...(args);
        auto list = make_list(args...);

        auto scalar = list.getComp(Tag<size-1>());

        //constexpr size_t dim = decltype(list.getComp(0_c))::size;//get the spatial dimension from tone of the gauge vector.
        auto dx = decltype(scalar)::Getter::get(scalar,0_c).getDx();

        auto CovPlus = MakeVector(i,1,dim,
                                 (1.0/dx)*(binary_fold(
                                         [](auto x, auto y){return x*y;},
                                         MakeArray(j,0,size-2,list.getComp(j)(i)),
                                         [](auto x){return x;},shift(scalar,i))
                                         - scalar)
        );

        return CovPlus;

    }
*/

} /* TempLat */

#ifdef TEMPLATTEST
#include "forwardcovariantderivative_test.h"
#endif


#endif
