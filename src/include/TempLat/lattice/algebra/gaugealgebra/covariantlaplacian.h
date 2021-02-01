#ifndef COSMOINTERFACE_GAUGEALGEBRA_COVARIANTLAPLACIAN_H
#define COSMOINTERFACE_GAUGEALGEBRA_COVARIANTLAPLACIAN_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/assignabletuple.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/listoperators/foldmultiply.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"

namespace TempLat {


    /** \brief A class which
     *
     * 
     * Unit test: make test-covariantlaplacian
     **/

    class CovariantLaplacianTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
/*
    //Bug, mysterious seg fault for SU2. Workaround: take only three args, U1s, SU2s and phi and write multiplication explicitly. Some forwarding problem?
    template<size_t dim, class... Args>
    auto CovariantLaplacian(Args... args)
    {
        constexpr size_t size = sizeof...(args);
        auto list = make_list(args...);

        auto scalar = list.getComp(Tag<size-1>());


//        constexpr size_t dim = decltype(list.getComp(0_c))::size;//get the spatial dimension from tone of the gauge vector.
//        say << dim;

        auto CovPlus = total(MakeArray(i,1,dim,
            binary_fold( [](auto x, auto y){return x*y;}, MakeArray(j,0,size-2,list.getComp(j).getComp(i-1_c)),[](auto x){return x;},shift(scalar,i))
                             ));


        say << CovPlus;

        auto CovMinus = total(MakeArray(i,1,dim,
                                       binary_fold( [](auto x, auto y){return x*y;}, MakeArray(j,0,size-2,dagger(shift(list.getComp(j).getComp(i-1_c),-i))),[](auto x){return x;},shift(scalar,-i))
                             ));

        say << CovMinus;



        auto central =  (2.0 * dim) * scalar;

        auto dx2 = pow<2>(GetDx::getDx(scalar));

        return (1.0 / dx2 ) * (CovPlus + CovMinus - central);

    }

    template<size_t dim, class T>
    auto ComplexLaplacian(T t)
    {
        return CovariantLaplacian<dim>(t);
    }
*/


} /* TempLat */

#ifdef TEMPLATTEST
#include "covariantlaplacian_test.h"
#endif


#endif
