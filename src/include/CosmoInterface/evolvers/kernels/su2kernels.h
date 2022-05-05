#ifndef COSMOINTERFACE_EVOLVERS_KERNELS_SU2KERNELS_H
#define COSMOINTERFACE_EVOLVERS_KERNELS_SU2KERNELS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/definitions/gaugederivatives.h"
#include "CosmoInterface/definitions/mattercurrents.h"
#include "CosmoInterface/definitions/potential.h"
#include "TempLat/lattice/algebra/gaugealgebra/plaquetteback.h"

#include "TempLat/util/tdd/tdd.h"


namespace TempLat {


    /** \brief A class that computes the kernel for the SU2 gauge fields.
     *
     * 
     * Unit test: make test-su2kernels
     **/

    class SU2Kernels {
    public:
        /* Put public methods here. These should change very little over time. */
        SU2Kernels() = delete;


        template <class Model, int N>
        static auto get(Model& model, Tag<N> n)
        {
        
        	// Computes different terms in the SU(2) gauge kernels:
        
        	// SU(2) gauge current
            auto SU2Source = MatterCurrents::SU2Current(model, n);

			// Gradient (forward contribution)
            auto GradSU2 = MakeVector(i,1,Model::NDim,
                                Total(j,1,Model::NDim,
                                        IfElse(!IsEqual(i,j), plaq(model.fldSU2(n),i,j), ZeroType())
                                )
            );

			// Gradient (backward contribution)
            auto GradSU2Back = MakeVector(i,1,Model::NDim,
                                    Total(j,1,Model::NDim,
                                            IfElse(!IsEqual(i,j), plaqBack(model.fldSU2(n),i,j), ZeroType())
                                    )
            );

			auto normSU2Source =  (1 / model.dx) * pow(model.aI, 1 + model.alpha);  // Rescaling for SU2Source
            auto normGrad =  pow<3>(1 / model.dx) / model.gQ_SU2DblSU2(0_c, n) * pow(model.aI, -1 + model.alpha);  // Rescaling for GradSU2 and GradSU2Back
            // --> Note: the 0_c in normGrad is correct. In our convention, the link is defined with the coupling of the 0th doublet.

			// Returns kernel for the SU(2) gauge fields' EOM:
            return - normGrad * (GradSU2 - GradSU2Back) - normSU2Source * SU2Source;
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
#include "CosmoInterface/evolvers/kernels/su2kernels_test.h"
#endif


#endif
