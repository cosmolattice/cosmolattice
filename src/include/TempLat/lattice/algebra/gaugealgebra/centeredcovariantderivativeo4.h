#ifndef TEMPLAT_LATTICE_ALGEBRA_GAUGEALGEBRA_CENTEREDCOVARIANTDERIVATIVEO4_H
#define TEMPLAT_LATTICE_ALGEBRA_GAUGEALGEBRA_CENTEREDCOVARIANTDERIVATIVEO4_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

//MATHEMATICS OF  COMPUTATION VOLUME 51,  NUMBER 184 OCTOBER 1988, PAGES 699-706 Generation of Finite Difference Formulas on Arbitrarily Spaced Grids By  Bengt Fornberg

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/assignabletuple.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/listoperators/foldmultiply.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"

namespace TempLat {


    /** \brief A class which computes a O(dx^4) discrete covariant derivative.
     *
     * 
     * Unit test: make test-centeredcovariantderivativeo4
     **/

    template<size_t dim, class... Args>
    auto CenteredCovariantDerivativeO4(Args... args)
    {
        constexpr size_t size = sizeof...(args);
        auto list = make_list(args...);

        auto scalar = list.getComp(Tag<size-1>());


        auto dx = GetDx::getDx(scalar);

        auto UPlus = MakeVector(i,1,dim,fold_multiply(MakeArray(j,0,size-2,list.getComp(j)(i)))) ;
        auto UPlusPlus = MakeVector(i,1,dim,fold_multiply(MakeArray(j,0,size-2,shift(list.getComp(j)(i),i)))) ;
        auto UMinus = MakeVector(i,1,dim,fold_multiply(MakeArray(j,0,size-2,dagger(shift(list.getComp(j)(i),-i))))) ;
        auto UMinusMinus = MakeVector(i,1,dim,fold_multiply(MakeArray(j,0,size-2,dagger(shift(shift(list.getComp(j)(i),-i),-i))))) ;

        auto Cov = MakeVector(i,1,dim,
                              (1 / dx) * (
                                      -(1.0/12) * UPlusPlus(i) * shift(shift(scalar,i),i) +
                                      +(2.0/3.0) * UPlus(i) * shift(scalar,i)
                                      -(2.0/3.0) * UMinus(i) * shift(scalar,-i))
                                      +(1.0/12.0) * UMinusMinus(i) * shift(shift(scalar,-i),-i)
        );
        return Cov;
    }
    template<size_t dim, class T>
    auto CenteredDerivativeO4(T t)
    {
        return CenteredCovariantDerivativeO4<dim>(t);
    }

} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/gaugealgebra/centeredcovariantderivativeo4_test.h"
#endif


#endif
