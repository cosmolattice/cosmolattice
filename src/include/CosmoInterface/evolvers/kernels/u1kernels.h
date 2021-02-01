#ifndef COSMOINTERFACE_EVOLVERS_KERNELS_U1KERNELS_H
#define COSMOINTERFACE_EVOLVERS_KERNELS_U1KERNELS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/definitions/mattercurrents.h"
#include "TempLat/lattice/algebra/spatialderivatives/backdiff.h"
#include "TempLat/lattice/algebra/spatialderivatives/forwdiff.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class that computes the kernel of the U(1) gauge fields.
     *
     * 
     * Unit test: make test-u1kernels
     **/

    class U1Kernels {
    public:
        /* Put public methods here. These should change very little over time. */
        U1Kernels() {

        }

        template <class Model, int N>
        static auto get(Model& model, Tag<N> a){
        
        	// Computes different terms in the U(1) gauge kernels:

			// --> U(1) gauge current
            auto U1Source = MakeArray(n,0,Model::NU1-1,MatterCurrents::U1Current(model, n));

 			// --> lapl(A_i)
            auto LaplU1 = MakeArray(n,0,Model::NU1-1, 
                                    MakeVector(i, 1, Model::NDim,
                                               LatLapl<Model::NDim>(model.fldU1(n)(i)));
            );

			// --> \partial_i \partial_j A_i
            auto GradU1 = MakeArray(n,0,Model::NU1-1,MakeVector(i, 1, Model::NDim,
                                                                Total(j, 1, Model::NDim,
                                                                      backDiff(forwDiff(model.fldU1(n)(j), i), j)
                                                                ));
            );

            auto normU1Source = pow(model.aI, 1 + model.alpha); // Rescaling for U1Source
            auto normGrad =  pow(model.aI, -1 + model.alpha); // Rescaling for GradU1 and LaplU1 
       
            
            // Returns kernel for the U(1) gauge fields' EOM:
            return normGrad * (LaplU1(a) - GradU1(a)) - normU1Source * U1Source(a);

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
#include "CosmoInterface/evolvers/kernels/u1kernels_test.h"
#endif


#endif
