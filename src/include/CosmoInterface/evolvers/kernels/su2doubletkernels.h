#ifndef COSMOINTERFACE_EVOLVERS_KERNELS_SU2DOUBLETKERNELS_H
#define COSMOINTERFACE_EVOLVERS_KERNELS_SU2DOUBLETKERNELS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "CosmoInterface/definitions/potential.h"
#include "CosmoInterface/definitions/gaugederivatives.h"

namespace TempLat {


    /** \brief A class which holds the kernels for the SU2Doublet.
     *
     * 
     * Unit test: make test-su2doubletkernels
     **/

    class SU2DoubletKernels {
    public:
        /* Put public methods here. These should change very little over time. */
        SU2DoubletKernels() = delete;

        template <class Model, int N>
        static auto get(Model& model, Tag<N> n)
        {
        	// Returns kernel for SU2 doublets (formed by covariant laplacian and potential derivative):
            return pow(model.aI, 1 + model.alpha) * GaugeDerivatives::covLaplacianSU2Doublet(model,n) - pow(model.aI, 3 + model.alpha) / 2 * Potential::derivSU2Doublet(model,n);
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
#include "CosmoInterface/evolvers/kernels/su2doubletkernels_test.h"
#endif


#endif
