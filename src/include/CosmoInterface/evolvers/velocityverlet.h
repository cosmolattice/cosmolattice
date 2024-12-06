#ifndef COSMOINTERFACE_EVOLVERS_VELOCITYVERLET_H
#define COSMOINTERFACE_EVOLVERS_VELOCITYVERLET_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020


#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/listoperators/listoperators.h"
#include "TempLat/lattice/algebra/su2algebra/su2algebra.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/staticif.h"
#include "TempLat/lattice/measuringtools/averager.h"
#include "TempLat/lattice/algebra/gaugealgebra/u1exponential.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldshift.h"
#include "CosmoInterface/evolvers/velocityverletparameters.h"
#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/evolvers/kernels/kernels.h"
#include "CosmoInterface/definitions/fixedbackgroundexpansion.h"
#include "CosmoInterface/definitions/energies.h"
#include "CosmoInterface/definitions/averages.h"


namespace TempLat {


    /** \brief A class which implements a velocity verlet algorithm that evolves scalar singlets, complex scalars, SU2 doublets, and U(1) and SU(2) gauge fields.
     *
     *
     **/



    template<typename T = double>
    class VelocityVerlet {
    public:
        /* Put public methods here. These should change very little over time. */

        template<class Model>
        VelocityVerlet(Model& model, RunParameters<T>& rPar) :
        expansion(rPar.expansion),
        fixedBackground(rPar.fixedBackground), // A boolean. If true, expansion is fixed
        aBackground(model, rPar),  // Used for fixed background expansion
        ws(VelocityVerletParameters<T>::getCoeffs(rPar.eType)), // Vector containing the numerical parameters of the chosen VV method
        order(VelocityVerletParameters<T>::getOrder(rPar.eType)) // Order of the VV algorithm
        {
        }


        template<class Model>
        void evolve(Model &model, T tMinust0) {

            /*
             * Velocity Verlet can be understood as moving momenta by half step, fields by a step and momenta by half a step again.
             *
             * 1. pi_1/2 = pi_0 + dt/2 *kernel_0  [KICK]
             * 2. phi_1 = phi_0 + dt * pi_1/2  [DRIFT]
             * 3. pi_1 = pi_1/2 + dt/2 *kernel_1  [KICK]
             *
             * */

            T w;
            size_t parSize = ws.size(); // Number of distinct parameters (1 for VV2, 2 for VV4, 4 for VV6...)

            size_t stages = (parSize - 1) * 2 + 1; // Number of operations in each iteration

            for(size_t i = 0; i < stages; ++i ){  // loop over operations...

                w = ws[(i < parSize ? i : stages - i - 1 )];

				// We start by computing the kicks (pi_0 --> pi_1/2):
                if(expansion && !fixedBackground) kickScaleFactorHalf(model, w);   // only if self-consistent expansion

                if(model.Ns > 0) kickScalar(model, w);
                if (model.fldGWs != nullptr) kickGWs(model, w);
                if(model.NCs > 0) kickCS(model, w);
                if(model.NSU2Doublet > 0) kickSU2Doublet(model, w);
                if(model.NU1 > 0) kickU1Vector(model, w);
                if(model.NSU2 > 0) kickSU2Vector(model, w);

                if (expansion){
                    if(!fixedBackground) storeMomentaAverages(model);

                    // Now we compute the drifts (phi_0 --> phi_1)
                    driftScaleFactor(model, tMinust0 + model.dt, w);
                }


                if(model.Ns > 0) driftScalar(model, w);
                if (model.fldGWs != nullptr) driftGWs(model, w);
                if(model.NCs > 0) driftCS(model, w);
                if(model.NSU2Doublet > 0) driftSU2Doublet(model, w);
                if(model.NU1 > 0) driftU1Vector(model, w);
                if(model.NSU2 > 0) driftSU2Vector(model, w);

                if(expansion && !fixedBackground) storeFieldsAverages(model);

                // Now we compute the second kick (pi_1/2 --> pi_1)

                if(model.Ns > 0) kickScalar(model, w);
                if (model.fldGWs != nullptr) kickGWs(model, w);
                if(model.NCs > 0) kickCS(model, w);
                if(model.NSU2Doublet > 0) kickSU2Doublet(model, w);
                if(model.NU1 > 0) kickU1Vector(model, w);
                if(model.NSU2 > 0) kickSU2Vector(model, w);

                if (expansion && !fixedBackground)
                {
                    storeMomentaAverages(model);
                    kickScaleFactorOne(model, w);
                }

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
           		model.piS(i) += (w * model.dt / 2) * ScalarSingletKernels::get(model, i) ;
           			);
        }

		template<class Model>
        void kickGWs(Model &model, T w) {

           ForLoop(i, 0, Model::NGWs - 1,
           		(*model.piGWs)(i) += (w * model.dt / 2) * GWsKernels::get(model, i) ;
           			);
        }

        // Evolves piCS(n)
        template<class Model>
        void kickCS(Model& model, T w) {

            ForLoop(n, 0, Model::NCs -1,
            	model.piCS(n) += model.dt / 2 * w * ComplexScalarKernels::get(model,n);
            		);
        }

		// Evolves piSU2Doublet(n)
        template<class Model>
        void kickSU2Doublet(Model& model, T w) {

            ForLoop(n, 0, Model::NSU2Doublet -1,
            	model.piSU2Doublet(n) += model.dt / 2 * w *(SU2DoubletKernels::get(model,n));
                    );
        }

		// Evolves piU1(a)
        template<class Model>
        void kickU1Vector(Model& model, T w) {

           ForLoop(a,0,Model::NU1 - 1,
                    model.piU1(a) +=  model.dt * w / 2 * U1Kernels::get(model,a)
                            );
        }

		// Evolves piSU2(n)
        template<class Model>
        void kickSU2Vector(Model& model, T w)
        {
            ForLoop(n,0,Model::NSU2 - 1,
                    model.piSU2(n) +=  model.dt / 2  * w * SU2Kernels::get(model,n);
                    );
        }

        // Evolves aDotSI
        template<class Model>
        void kickScaleFactorHalf(Model &model, T w) {
            model.aDotSI = model.aDotI + (model.dt / 2) * ScaleFactorKernels::get(model) * w;
        }

        // Evolves aDotI
        template<class Model>
        void kickScaleFactorOne(Model &model, T w) {
            model.aDotI = model.aDotSI + (model.dt / 2) * ScaleFactorKernels::get(model) * w;
        }

        /********
        * DRIFTS
        *********/

		// Evolves fldS
        template<class Model>
        void driftScalar(Model &model, T w)
        {
            model.fldS +=  pow(model.aSI, model.alpha - 3) * (model.dt * w * model.piS );
        }

        // Evolves fldGWs
        template<class Model>
        void driftGWs(Model &model, T w)
        {
            (*model.fldGWs) +=  pow(model.aSI, model.alpha - 3) * (model.dt * w * (*model.piGWs) );
        }

		// Evolves fldCS
        template<class Model>
        void driftCS(Model& model, T w)
        {
            model.fldCS += pow(model.aSI, model.alpha - 3) * model.dt * w * model.piCS ;
        }

        // Evolves fldSU2Doublet
        template<class Model>
        void driftSU2Doublet(Model& model, T w)
        {
            model.fldSU2Doublet += pow(model.aSI, model.alpha - 3) * model.dt * w * model.piSU2Doublet ;
        }

		// Evolves fldU1
        template<class Model>
        void driftU1Vector(Model& model, T w)
        {
            model.fldU1 +=  pow(model.aSI, model.alpha - 1) * w * model.dt * model.piU1;
        }

		// Evolves fldSU2
        template<class Model>
        void driftSU2Vector(Model& model, T w)
        {
            ForLoop(n, 0, Model::NSU2 - 1,
                    auto rescaledPi = MakeVector(i,1,Model::NDim,
                            toSU2(w /2.0 * pow(model.aSI, model.alpha - 1) * model.dx * model.dt * model.gQ_SU2DblSU2(0_c, n) * model.piSU2(n)(i))
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
            else{  // if self-consistent expansion, the scale factor is evolved with the VV algorithm
                model.aI += model.dt * model.aDotSI * w;
                model.aSI = (model.aIM + model.aI) / 2.0;
            }
        }

    	/********
        * FUNCTIONS STORING VOLUME AVERAGES OF COMPOSITE FIELDS AND MOMENTA at different times
        *********/

        template<class Model>
        void storeMomentaAverages(Model& model) {
            if (Model::Ns > 0)  model.pi2AvI = Averages::pi2S(model);
            if (Model::NCs > 0) model.CSpi2AvI = Averages::pi2CS(model);
            if (Model::NSU2Doublet > 0) model.SU2DblPi2AvI = Averages::pi2SU2Doublet(model);
            if (Model::NU1 > 0) model.U1pi2AvI = Averages::pi2U1(model);
            if (Model::NSU2 > 0) model.SU2pi2AvI = Averages::pi2SU2(model);

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
    struct VelocityVerletTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/evolvers/velocityverlet_test.h"
#endif


#endif
