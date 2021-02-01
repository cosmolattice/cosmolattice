#ifndef TEMPLAT_LATTICE_ALGEBRA_GAUGEALGEBRA_BACKWARDCOVARIANTDERIVATIVE_H
#define TEMPLAT_LATTICE_ALGEBRA_GAUGEALGEBRA_BACKWARDCOVARIANTDERIVATIVE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/assignabletuple.h"
#include "TempLat/lattice/algebra/listoperators/foldmultiply.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"


namespace TempLat {


    /** \brief A class which
     *
     * 
     * Unit test: make test-backwardcovariantderivative
     **/

    template<size_t dim, class... Args>
    auto BackwardCovariantDerivative(Args... args)
    {
        constexpr size_t size = sizeof...(args);
        auto list = make_list(args...);

        auto scalar = list.getComp(Tag<size-1>());

        //constexpr size_t dim = decltype(list.getComp(0_c))::size;//get the spatial dimension from tone of the gauge vector.
        auto dx = GetDx::getDx(scalar);

        auto CovMinus = MakeVector(i,1,dim,
                                  (1.0/dx)*(scalar- fold_multiply(MakeArray(j,0,size-2,dagger(shift(list.getComp(j)(i),-i)))) * shift(scalar,-i))
        );


        return CovMinus;

    }



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/gaugealgebra/backwardcovariantderivative_test.h"
#endif


#endif
