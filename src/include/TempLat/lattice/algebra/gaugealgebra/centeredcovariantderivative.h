#ifndef TEMPLAT_LATTICE_ALGEBRA_GAUGEALGEBRA_CENTEREDCOVARIANTDERIVATIVE_H
#define TEMPLAT_LATTICE_ALGEBRA_GAUGEALGEBRA_CENTEREDCOVARIANTDERIVATIVE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/assignabletuple.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/listoperators/foldmultiply.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"

namespace TempLat {


    /** \brief A class which computes centered covariant derivatives.
     *
     * 
     * Unit test: make test-centeredcovariantderivative
     **/

    template<size_t dim, class... Args>
    auto CenteredCovariantDerivative(Args... args)
    {
        constexpr size_t size = sizeof...(args);
        auto list = make_list(args...);

        auto scalar = list.getComp(Tag<size-1>());


        auto dx = GetDx::getDx(scalar);

        auto UPlus = MakeVector(i,1,dim,fold_multiply(MakeArray(j,0,size-2,list.getComp(j)(i)))) ;
        auto UMinus = MakeVector(i,1,dim,fold_multiply(MakeArray(j,0,size-2,dagger(shift(list.getComp(j)(i),-i))))) ;

        auto Cov = MakeVector(i,1,dim,
                              (0.5 / dx) * (UPlus(i) * shift(scalar,i) -
                                               UMinus(i) * shift(scalar,-i))
        );


        return Cov;

    }
    template<size_t dim, class T>
    auto CenteredDerivative(T t)
    {
        return CenteredCovariantDerivative<dim>(t);
    }




} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/gaugealgebra/centeredcovariantderivative_test.h"
#endif


#endif
