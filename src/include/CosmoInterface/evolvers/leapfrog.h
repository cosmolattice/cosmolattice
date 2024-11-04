#ifndef COSMOINTERFACE_EVOLVERS_LEAPFROG_H
#define COSMOINTERFACE_EVOLVERS_LEAPFROG_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/evolvers/kernels/kernels.h"
#include "CosmoInterface/definitions/averages.h"
#include "CosmoInterface/definitions/hubbleconstraint.h"
#include "CosmoInterface/definitions/fixedbackgroundexpansion.h"

namespace TempLat {

    /** \brief A class which implements a leapfrog scheme to evolve  scalar singlets, complex scalars, SU2 doublets, and U(1) and SU(2) gauge fields.
     *
     **/

    template <class T>
    class LeapFrog {
    public:
        /* Put public methods here. These should change very little over time. */

        template<class Model>
        LeapFrog(Model& model, RunParameters<T>& rPar) :
                expansion(rPar.expansion),
                synced(!rPar.doWeRestart), //If this is the initial time, all fields are a t=0
                fixedBackground(rPar.fixedBackground),  // A boolean. If true, expansion is fixed (and not self-consistent)
                aBackground(model, rPar)   // Used for fixed background expansion
        {
             if(synced)  model.aDotSI = model.aDotI - model.dt / 2.0 * ScaleFactorKernels::get(model);
             // If this is time t=0 in a new simulation, we set aDotSI at t=-1/2, so we can
             // evolve it by a full time step without changing the algorithm.
        }

        template<class Model>
        void evolve(Model& model, T tMinust0) {

            /*
             * Leapfrog has momenta living at half-integer steps
             * and coordinates at integer steps.
             *
             * 1. pi_1/2 = pi_(-1/2) + dt * kernel_0  [KICKS]
             * 2. phi_1 = phi_0 + dt * pi_1/2  [DRIFTS]
             *
             * */

            // We start by computing the kicks:

            // First, we check whether our momenta are in a "normal state", namely living at
            // -0.5 (not synced with the fields) or whether they have been synced with the fields
            // for measurement purposes. In this case, they have already been evolved by half
            // a time step and we need to evolve them only by another half time step.
            T weight = synced ? 0.5 : 1.0;

            if (model.Ns > 0) kickScalar(model, weight);
            if (model.fldGWs != nullptr) kickGWs(model, weight);
            if (model.NCs > 0) kickCS(model, weight);
            if (model.NSU2Doublet > 0) kickSU2Doublet(model, weight);
            if (model.NU1 > 0) kickU1Vector(model, weight);
            if (model.NSU2 > 0) kickSU2Vector(model, weight);

            if (expansion) {
                if(!fixedBackground) {
                    storeMomentaAverages(model);
                    kickScaleFactor(model);
                }
                // We always evolve only the semi-integer scale factor time derivative
                // from semi-sums of momenta at semi_integer time so we don't need to worry
                //  whether or not the field were synced or not.

            // Now we compute the drifts:
                driftScaleFactor(model, tMinust0 + model.dt);
            }

            if (model.Ns > 0) driftScalar(model);
            if (model.fldGWs != nullptr) driftGWs(model);
            if (model.NCs > 0) driftCS(model);
            if (model.NSU2Doublet > 0) driftSU2Doublet(model);
            if (model.NU1 > 0) driftU1Vector(model);
            if (model.NSU2 > 0) driftSU2Vector(model);

            if (expansion && !fixedBackground) storeFieldsAverages(model);

            synced = false;
        }

        // Function used to synchronize the momentum to the field, by evolving them
        // only by half a time step. Called before performing the measurements,
        //  so everything can be measured at integer time.
        template<class Model>
        void sync(Model& model, T tMinust0) {
            if(! synced) {
                if (model.Ns > 0) kickScalar(model, 0.5);
                if (model.fldGWs != nullptr) kickGWs(model, 0.5);
                if (model.NCs > 0) kickCS(model, 0.5);
                if (model.NSU2Doublet > 0) kickSU2Doublet(model, 0.5);
                if (model.NU1 > 0) kickU1Vector(model, 0.5);
                if (model.NSU2 > 0) kickSU2Vector(model, 0.5);

                if (expansion) {
                    if (Model::Ns > 0) model.pi2AvI = Averages::pi2S(model); // at t
                    if (Model::NCs > 0) model.CSpi2AvI = Averages::pi2CS(model); // at t
                    if (Model::NSU2Doublet > 0) model.SU2DblPi2AvI = Averages::pi2SU2Doublet(model); // at t
                    if (Model::NU1 > 0) model.U1pi2AvI = Averages::pi2U1(model); // at t
                    if (Model::NSU2 > 0) model.SU2pi2AvI = Averages::pi2SU2(model); // at t
                    if (!fixedBackground) model.aDotI = model.aDotSI + model.dt / 2.0 * ScaleFactorKernels::get(model);
                    else model.aDotI = aBackground.dot(tMinust0);
                }

            }
            synced = true;
        }

        /********
        * KICKS
        *********/

        template<class Model>
        void kickScaleFactor(Model& model) {
            model.aDotSIM = model.aDotSI;
            model.aDotSI +=  model.dt * ScaleFactorKernels::get(model);
            model.aDotI = (model.aDotSIM + model.aDotSI ) / 2.0;
        }


        template<class Model>
        void kickScalar(Model& model, T w) {
            ForLoop(n, 0, Model::Ns -1,
                    model.piS(n) += (w * model.dt) * ScalarSingletKernels::get(model,n);
            );
        }

		template<class Model>
        void kickGWs(Model& model, T w) {
            ForLoop(n, 0, Model::NGWs - 1,
                    (*model.piGWs)(n) += (w * model.dt) * GWsKernels::get(model,n);
            );
        }

        template<class Model>
        void kickCS(Model& model, T w) {
            ForLoop(n, 0, Model::NCs -1,
                    model.piCS(n) += (w * model.dt)  * ComplexScalarKernels::get(model,n);
            );
        }

        template<class Model>
        void kickSU2Doublet(Model& model, T w) {
            ForLoop(n, 0, Model::NSU2Doublet -1,
                    model.piSU2Doublet(n) += (w * model.dt)  * SU2DoubletKernels::get(model,n);
            );
        }

        template<class Model>
        void kickU1Vector(Model& model, T w) {
            ForLoop(n,0,Model::NU1 - 1,
                    model.piU1(n) +=  (w * model.dt) * U1Kernels::get(model,n)
            );
        }

        template<class Model>
        void kickSU2Vector(Model& model, T w)
        {
            ForLoop(n,0,Model::NSU2 - 1,
                    model.piSU2(n) +=  (w * model.dt) * SU2Kernels::get(model,n);
            );
        }

        /********
        * DRIFTS
        *********/


        template<class Model>
        void driftScaleFactor(Model& model, T tMinust0) {
            model.aIM = model.aI;  // at t
            if(fixedBackground){  // if fixed background, the scale factor is given by the power-law function in fixedbackgroundexpansion.h
                model.aI = aBackground(tMinust0);
                model.aSI = aBackground(tMinust0 + model.dt / 2.0);
            }
            else{    // if self-consistent expansion, the scale factor is evolved with the VV algorithm
                model.aI += model.dt * model.aDotSI ;  // at t+dt
                model.aSI = (model.aIM + model.aI) / 2.0;  // at t+dt/2 (average)
            }
        }
        template<class Model>
        void driftScalar(Model& model) {
            // Here we rescale the canonical momentum appropriately to get
            // the time derivative of the field.
            model.fldS +=  pow(model.aSI, model.alpha - 3) * (model.dt * model.piS );

            // Note that here we use a compact syntax, without specifying the ForLoop, which
            // will be done internally. Equivalently, one can write the ForLoop explicitly
            // as for the kernels (the ForLoop syntax is used in the kernels as it is more
            // convenient to define kernel function which return the kernel for a specific
            // field).

        }

         template<class Model>
        void driftGWs(Model& model) {

            (*model.fldGWs) +=  pow(model.aSI, model.alpha - 3) * (model.dt * (*model.piGWs) );

        }


        template<class Model>
        void driftCS(Model& model)
        {
            model.fldCS += pow(model.aSI, model.alpha - 3) * model.dt  * model.piCS ;
        }

        template<class Model>
        void driftSU2Doublet(Model& model)
        {
            model.fldSU2Doublet += pow(model.aSI, model.alpha - 3) * model.dt * model.piSU2Doublet ;
        }

        template<class Model>
        void driftU1Vector(Model& model)
        {
            model.fldU1 +=  pow(model.aSI, model.alpha - 1) * model.dt * model.piU1;
        }

        template<class Model>
        void driftSU2Vector(Model& model)
        {
            ForLoop(n, 0, Model::NSU2 - 1,
                    auto rescaledPi = MakeVector(i,1,Model::NDim,
                                                 toSU2(0.5 * pow(model.aSI, model.alpha - 1) * model.dx * model.dt * model.gQ_SU2DblSU2(0_c, n) * model.piSU2(n)(i))
                    ); //The 0_c is correct. In our convention, the link is normalized wrt the first doublet charge.

                            model.fldSU2(n) =   (rescaledPi * rescaledPi ) * model.fldSU2(n);
            );
            // Here for instance we use the ForLoop again, as this makes it easier to define
            // the rescaled momenta in this case.
        }


        /********
        * FUNCTIONS STORING VOLUME AVERAGES OF COMPOSITE FIELDS AND MOMENTA at different times
        *********/

        template<class Model>
        void storeMomentaAverages(Model& model) {
            if (Model::Ns > 0) {
                model.pi2AvSIM = model.pi2AvSI;  // at t-dt/2
                model.pi2AvSI = Averages::pi2S(model); // at t+dt/2
                model.pi2AvI = 0.5 * (model.pi2AvSIM + model.pi2AvSI); // at t (average)
            }

            if (Model::NCs > 0) {
                model.CSpi2AvSIM = model.CSpi2AvSI;  // at t-dt/2
                model.CSpi2AvSI = Averages::pi2CS(model); // at t+dt/2
                model.CSpi2AvI = 0.5 * (model.CSpi2AvSIM + model.CSpi2AvSI); // at t (average)
            }
            if (Model::NSU2Doublet > 0) {
                model.SU2DblPi2AvSIM = model.SU2DblPi2AvSI; // at t-dt/2
                model.SU2DblPi2AvSI = Averages::pi2SU2Doublet(model); // at t+dt/2
                model.SU2DblPi2AvI = 0.5 * (model.SU2DblPi2AvSIM + model.SU2DblPi2AvSI); // at t (average)
            }
            if (Model::NU1 > 0) {
                model.U1pi2AvSIM = model.U1pi2AvSI; // at t-dt/2
                model.U1pi2AvSI = Averages::pi2U1(model); // at t+dt/2
                model.U1pi2AvI = 0.5 * (model.U1pi2AvSIM + model.U1pi2AvSI); // at t (average)
            }
            if (Model::NSU2 > 0) {
                model.SU2pi2AvSIM = model.SU2pi2AvSI; // at t-dt/2
                model.SU2pi2AvSI = Averages::pi2SU2(model); // at t+dt/2
                model.SU2pi2AvI = 0.5 * (model.SU2pi2AvSIM + model.SU2pi2AvSI); // at t (average)
            }
        }



        template<class Model>
        void storeFieldsAverages(Model& model) {
            if (Model::Ns > 0) model.grad2AvI = Averages::grad2S(model); // at t
            if (model.NCs > 0) model.CSgrad2AvI = Averages::grad2CS(model); // at t
            if (model.NSU2Doublet > 0) model.SU2DblGrad2AvI = Averages::grad2SU2Doublet(model); // at t
            if (Model::NU1 > 0)  model.U1Mag2AvI = Averages::B2U1(model); // at t
            if (Model::NSU2 > 0)  model.SU2Mag2AvI = Averages::B2SU2(model); // at t
            model.potAvI = average(Potential::potential(model)); // at t
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        bool expansion;
        bool synced;
        bool fixedBackground;

        FixedBackgroundExpansion<T> aBackground;
    };


    class LeapFrogTester{
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/evolvers/leapfrog_test.h"
#endif


#endif
