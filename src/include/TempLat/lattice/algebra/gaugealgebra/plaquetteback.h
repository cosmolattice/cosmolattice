#ifndef COSMOINTERFACE_GAUGEALGEBRA_PLAQUETTEBACK_H
#define COSMOINTERFACE_GAUGEALGEBRA_PLAQUETTEBACK_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/su2algebra/su2dagger.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/gaugealgebra/plaquette.h"
#include "TempLat/lattice/algebra/listoperators/listshift.h"
#include "TempLat/lattice/memory/memorytoolbox.h"

namespace TempLat {


    /** \brief A class which implemets a backward plaquette. Useful in equations of motions for example.
     *
     * 
     * Unit test: make test-plaquetteback
     **/

    struct PlaquetteBackTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<int Mu, int Nu, typename R>
    auto plaqBack(const R& Us, Tag<Mu> mu, Tag<Nu> nu){
        return shift(dagger(Us(nu)),-nu)*shift(Us(mu),-nu) * (shift(shift(Us(nu),mu),-nu) * dagger(Us(mu)));
        //return (dagger(shift<-Nu>(Us(nu))) * shift<-Nu>(Us(mu))) * (shift<-Nu>(shift<Mu>(Us(nu)) ) *  dagger(shift<Mu>(Us(mu)))) * (dagger(Us(nu))  *shift<-Nu>(Us(nu)));
    }




} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/gaugealgebra/plaquetteback_test.h"
#endif


#endif
