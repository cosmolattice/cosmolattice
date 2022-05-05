#ifndef COSMOINTERFACE_EVOLVERS_POSITIONVERLET_H
#define COSMOINTERFACE_EVOLVERS_POSITIONVERLET_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio, Nicolas Loayza  Year: 2021


#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/listoperators/listoperators.h"
#include "TempLat/lattice/algebra/su2algebra/su2algebra.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/staticif.h"
#include "TempLat/lattice/measuringtools/averager.h"
#include "TempLat/lattice/algebra/gaugealgebra/u1exponential.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldshift.h"
#include "CosmoInterface/evolvers/positionverletparameters.h"
#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/evolvers/kernels/kernels.h"
#include "CosmoInterface/definitions/fixedbackgroundexpansion.h"
#include "CosmoInterface/definitions/energies.h"
#include "CosmoInterface/definitions/averages.h"

namespace TempLat {


    /** \brief A class which implements a position verlet algorithm that evolves scalar singlets, gravitational waves, complex scalars, SU2 doublets, and U(1) and SU(2) gauge fields.
     *
     * 
     **/



    template<typename T = double>
    class PositionVerlet {
    public:
        /* Put public methods here. These should change very little over time. */

        template<class Model>
        PositionVerlet(Model& model, RunParameters<T>& rPar) :
        expansion(rPar.expansion),
        fixedBackground(rPar.fixedBackground), // A boolean. If true, expansion is fixed
        aBackground(model, rPar),  // Used for fixed background expansion
        ws(PositionVerletParameters<T>::getCoeffs(rPar.eType)), // Vector containing the numerical parameters of the chosen VV method
        order(PositionVerletParameters<T>::getOrder(rPar.eType)) // Order of the VV algorithm
        {
        }

        template<class Model>
        void evolve(Model &model, T tMinust0) {

            /*
             * Position Verlet can be understood as moving momenta by half step, fields by a step and momenta by half a step again.
             *
             * 1. phi_1/2 = phi_0 + dt/2 * pi_0   [DRIFT]
             * 2. pi_1 = pi_0 + dt *kernel_0  	  [KICK]
             * 3. phi_1 = pi_1/2 + dt/2 *kernel_1  [DRIFT]
             *
             * */

            T w;
            size_t parSize = ws.size(); // Number of distinct parameters (1 for PV2, 2 for PV4, 4 for PV6...)

            size_t stages = (parSize - 1) * 2 + 1; // Number of operations in each iteration

            for(size_t i = 0; i < stages; ++i ){  // loop over operations...

                w = ws[(i < parSize ? i : stages - i - 1 )];

				// We start by computing the drifts (phi_0 --> phi_1/2):

                if(model.Ns > 0) driftScalar(model, w);
                if (model.fldGWs != nullptr) driftGWs(model, w);
                if(model.NCs > 0) driftCS(model, w);
                if(model.NSU2Doublet > 0) driftSU2Doublet(model, w);
                if(model.NU1 > 0) driftU1Vector(model, w);
                if(model.NSU2 > 0) driftSU2Vector(model, w);

				// Kicks pi_0 --> pi_1
				
				if (expansion && !fixedBackground)  driftScaleFactor(model, tMinust0, w);
				
                if(model.Ns > 0) kickScalar(model, w);
                if (model.fldGWs != nullptr) kickGWs(model, w);
                if(model.NCs > 0) kickCS(model, w);
                if(model.NSU2Doublet > 0) kickSU2Doublet(model, w);
                if(model.NU1 > 0) kickU1Vector(model, w);
                if(model.NSU2 > 0) kickSU2Vector(model, w);

                 if (expansion){
				 	 
                    if(!fixedBackground) {storeMomentaAverages(model);       
										  storeFieldsAveragesSI(model); }
                    kickScaleFactor(model, w);
                }
                
                // We finish by computing the drifts (phi_1/2 --> phi_1):
              if (expansion && !fixedBackground)  driftScaleFactor(model, tMinust0, w);

                if(model.Ns > 0) driftScalar(model, w);
                if (model.fldGWs != nullptr) driftGWs(model, w);
                if(model.NCs > 0) driftCS(model, w);
                if(model.NSU2Doublet > 0) driftSU2Doublet(model, w);
                if(model.NU1 > 0) driftU1Vector(model, w);
                if(model.NSU2 > 0) driftSU2Vector(model, w);
				
				 if (expansion && !fixedBackground) { storeFieldsAverages(model); } 
                    
                
            }
        }
		
		
        // This function is called before doing the measurements. It is used only to set aDotI to its correct value in case
        // of a background expansion.
        template<class Model>
        void sync(Model& model, T tMinust0) {
            if(fixedBackground) model.aDotI = aBackground.dot(tMinust0);
        }
        
    	/********
        * KICKS
        *********/ 
        
        // Evolves piS(i)
        template<class Model>
        void kickScalar(Model &model, T w) {
			
           ForLoop(i, 0, Model::Ns - 1, 
           		model.piS(i) += (w * model.dt) * ScalarSingletKernels::get(model, i) ;
           			);
        }
		
		
		// Evolves piGWs(i)
		template<class Model>
        void kickGWs(Model &model, T w) {

           ForLoop(i, 0, Model::NGWs - 1,
           		(*model.piGWs)(i) += (w * model.dt ) * GWsKernels::get(model, i) ;
           			);
        }
		
        // Evolves piCS(n)
        template<class Model>
        void kickCS(Model& model, T w) {

            ForLoop(n, 0, Model::NCs -1,
            	model.piCS(n) += model.dt  * w * ComplexScalarKernels::get(model,n);
            		);
        }

		// Evolves piSU2Doublet(n)
        template<class Model>
        void kickSU2Doublet(Model& model, T w) {

            ForLoop(n, 0, Model::NSU2Doublet -1,
            	model.piSU2Doublet(n) += model.dt  * w *(SU2DoubletKernels::get(model,n));
                    );
        }

		// Evolves piU1(a)
        template<class Model>
        void kickU1Vector(Model& model, T w) {
        
           ForLoop(a,0,Model::NU1 - 1,
                    model.piU1(a) +=  model.dt * w  * U1Kernels::get(model,a)
                            );
        }

		// Evolves piSU2(n)
        template<class Model>
        void kickSU2Vector(Model& model, T w)
        {
            ForLoop(n,0,Model::NSU2 - 1,
                    model.piSU2(n) +=  model.dt  * w * SU2Kernels::get(model,n);
                    );
        }
            
        // Evolves aDotI
        template<class Model>
        void kickScaleFactor(Model &model, T w) {
            model.aDotI +=  model.dt * ScaleFactorKernels::getSI(model) * w;
        }
    
        
        /********
        * DRIFTS
        *********/  

		// Evolves fldS
        template<class Model>
        void driftScalar(Model &model, T w) 
        {
            model.fldS +=  pow(model.aI, model.alpha - 3) * (model.dt/2 * w * model.piS );
        	
        }
        
      
        // Evolves fldGWs
        template<class Model>
        void driftGWs(Model &model, T w) 
        {
            (*model.fldGWs) +=  pow(model.aI, model.alpha - 3) * (model.dt/2 * w * (*model.piGWs) );
        }
		
		
		// Evolves fldCS
        template<class Model>
        void driftCS(Model& model, T w)
        {
            model.fldCS += pow(model.aI, model.alpha - 3) * model.dt/2 * w * model.piCS ;
        }
        
        // Evolves fldSU2Doublet
        template<class Model>
        void driftSU2Doublet(Model& model, T w)
        {
            model.fldSU2Doublet += pow(model.aI, model.alpha - 3) * model.dt/2 * w * model.piSU2Doublet ;
        }

		// Evolves fldU1
        template<class Model>
        void driftU1Vector(Model& model, T w)
        {
            model.fldU1 +=  pow(model.aI, model.alpha - 1) * w * model.dt/2 * model.piU1;
        }

		// Evolves fldSU2
        template<class Model>
        void driftSU2Vector(Model& model, T w)
        {
            ForLoop(n, 0, Model::NSU2 - 1,
                    auto rescaledPi = MakeVector(i,1,Model::NDim,
                            toSU2(w /2.0 * pow(model.aI, model.alpha - 1) * model.dx * model.dt * model.gQ_SU2DblSU2(0_c, n) * model.piSU2(n)(i))
                            ); //The 0_c is correct. In our convention, the link is normalized wrt the first doublet charge.

                            model.fldSU2(n) =   (rescaledPi * rescaledPi ) * model.fldSU2(n);
            );
        }

		// Evolves scale factor
        template<class Model>
        void driftScaleFactor(Model &model, T tMinust0, T w) {
            model.aIM = model.aI;  // at t
            if(fixedBackground){  // if fixed background, the scale factor is given by the power-law function in fixedbackgroundexpansion.h
                model.aI = aBackground(tMinust0);
                model.aSI = aBackground(tMinust0 + model.dt / 2.0);
            }
            else{  // if self-consistent expansion, the scale factor is evolved with the PV algorithm
                model.aI += model.dt/2 * model.aDotI * w;
            }
        }
    	/********
        * FUNCTIONS STORING VOLUME AVERAGES OF COMPOSITE FIELDS AND MOMENTA at different times
        *********/ 

		template<class Model>
        void storeMomentaAverages(Model& model) {
            if (Model::Ns > 0) {
                model.pi2AvIM = model.pi2AvI;  // at t
                model.pi2AvI = Averages::pi2S(model); // at t+dt
                model.pi2AvSI = 0.5 * (model.pi2AvIM + model.pi2AvI); // at t+dt/2 (average)
            }
            if (Model::NCs > 0) {
            	model.CSpi2AvIM = model.CSpi2AvI;
            	model.CSpi2AvI = Averages::pi2CS(model);
            	model.CSpi2AvSI = 0.5*(model.CSpi2AvIM+model.CSpi2AvI);
            	}
            	
            if (Model::NSU2Doublet > 0) {
            	model.SU2DblPi2AvIM = model.SU2DblPi2AvI;
            	model.SU2DblPi2AvI = Averages::pi2SU2Doublet(model);
           	 	model.SU2DblPi2AvSI = 0.5*(model.SU2DblPi2AvIM+model.SU2DblPi2AvI);
            }
            if (Model::NU1 > 0) {
            	model.U1pi2AvIM = model.U1pi2AvI;
            	model.U1pi2AvI = Averages::pi2U1(model);
            	model.U1pi2AvSI = 0.5*(model.U1pi2AvIM+model.U1pi2AvI);
            }
            if (Model::NSU2 > 0) {
            	model.SU2pi2AvIM = model.SU2pi2AvI;
            	model.SU2pi2AvI = Averages::pi2SU2(model);
            	model.SU2pi2AvSI = 0.5*(model.SU2pi2AvIM+model.SU2pi2AvI);
            }

        }
        
        template<class Model>
        void storeFieldsAveragesSI(Model& model) {
            if (Model::Ns > 0) model.grad2AvSI = Averages::grad2S(model);
            if (model.NCs > 0) model.CSgrad2AvSI = Averages::grad2CS(model);
            if (model.NSU2Doublet > 0) model.SU2DblGrad2AvSI = Averages::grad2SU2Doublet(model);
            if (Model::NU1 > 0)  model.U1Mag2AvSI = Averages::B2U1(model);
            if (Model::NSU2 > 0)  model.SU2Mag2AvSI = Averages::B2SU2(model);
            model.potAvSI = average(Potential::potential(model));
        }
		
		template<class Model>
        void storeFieldsAverages(Model& model) {
            if (Model::Ns > 0) model.grad2AvI = Averages::grad2S(model);
            if (model.NCs > 0) model.CSgrad2AvI = Averages::grad2CS(model);
            if (model.NSU2Doublet > 0) model.SU2DblGrad2AvI = Averages::grad2SU2Doublet(model);
            if (Model::NU1 > 0)  model.U1Mag2AvI = Averages::B2U1(model);
            if (Model::NSU2 > 0)  model.SU2Mag2AvI = Averages::B2SU2(model);
            model.potAvI = average(Potential::potential(model));
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        const bool expansion;
        bool fixedBackground;

        FixedBackgroundExpansion<T> aBackground;


        const std::vector<T> ws;
        size_t order;

    };
    struct PositionVerletTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/evolvers/positionverlet_test.h"
#endif


#endif
