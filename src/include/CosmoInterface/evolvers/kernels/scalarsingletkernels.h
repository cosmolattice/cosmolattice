#ifndef COSMOINTERFACE_EVOLVERS_KERNELS_SCALARKERNELS_H
#define COSMOINTERFACE_EVOLVERS_KERNELS_SCALARKERNELS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "CosmoInterface/definitions/potential.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"

namespace TempLat {


    /** \brief A class that computes the kernel for the scalar singlets.
     *
     * 
     * Unit test: make test-scalarkernels
     **/


    class ScalarSingletKernels {
    public:
        /* Put public methods here. These should change very little over time. */
        ScalarSingletKernels() = delete;

        template <class Model, int N>
        static auto get(Model& model, Tag<N> n){
        
        	// Returns kernel for scalar singlets (formed by laplacian and potential derivative):
            return (pow(model.aI, 1 + model.alpha) *
                    LatLapl<Model::NDim>(model.fldS(n))
                    - pow(model.aI, 3 + model.alpha) * Potential::derivS(model,n) );
        }

		
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/evolvers/kernels/scalarsingletkernels_test.h"
#endif


#endif
