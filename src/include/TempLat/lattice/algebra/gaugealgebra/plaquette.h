#ifndef COSMOINTERFACE_GAUGEALGEBRA_PLAQUETTE_H
#define COSMOINTERFACE_GAUGEALGEBRA_PLAQUETTE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/su2algebra/su2shift.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    \

    /** \brief A class which defines plaquette operator.
     * 
     * 
     * Unit test: make test-plaquette
     **/

    struct PlaquetteTester
    {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<int Mu, int Nu, typename R>
    auto plaq(const R& Us, Tag<Mu> mu, Tag<Nu> nu){
        //Now one has to be careful how to compute chain of matrix multiplication. Better to group them in smaller matrices, write (A*B)*(C*D) instead of
        // A*B*C*D. Reason: first expression will not expand the algebra, second will. Actually, the fully expanded expression even causes some segfault
        // from around 6 matrices chains, as you are using too much memory on the stack!
         return (Us(mu) * shift(Us(nu),mu) ) * ( dagger(shift(Us(mu),nu)) * dagger(Us(nu)) );

    }



} /* TempLat */

#ifdef TEMPLATTEST
#include "plaquette_test.h"
#endif


#endif
