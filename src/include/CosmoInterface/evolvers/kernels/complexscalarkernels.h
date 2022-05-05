#ifndef COSMOINTERFACE_EVOLVERS_KERNELS_COMPLEXSCALARKERNELS_H
#define COSMOINTERFACE_EVOLVERS_KERNELS_COMPLEXSCALARKERNELS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/definitions/potential.h"
#include "CosmoInterface/definitions/gaugederivatives.h"

namespace TempLat {

    /** \brief A class that computes the kernels for the complex scalars.
     *
     * 
     **/

    class ComplexScalarKernels {
    public:
        /* Put public methods here. These should change very little over time. */
        ComplexScalarKernels() = delete;

        template <class Model, int N>
        static auto get(Model& model, Tag<N> n){
        
        	// Returns kernel for complex scalars (formed by the covariant laplacian and potential derivative terms):
            return pow(model.aI, 1 + model.alpha) * GaugeDerivatives::covLaplacianCS(model,n)
            - pow(model.aI, 3 + model.alpha) / 2 * Potential::derivCS(model,n);
        }
        

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/evolvers/kernels/complexscalarkernels_test.h"
#endif


#endif
