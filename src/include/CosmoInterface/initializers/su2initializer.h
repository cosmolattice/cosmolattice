#ifndef COSMOINTERFACE_INITIALIZERS_SU2INITIALIZER_H
#define COSMOINTERFACE_INITIALIZERS_SU2INITIALIZER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/constants.h"
#include "TempLat/lattice/algebra/algebra.h"
#include "TempLat/lattice/algebra/helpers/getngrid.h"

#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "CosmoInterface/initializers/fluctuationsgenerator.h"
#include "CosmoInterface/definitions/potential.h"
#include "CosmoInterface/definitions/averages.h"
#include "CosmoInterface/definitions/mattercurrents.h"


namespace TempLat {


    /** \brief A class that initializes the SU(2) gauge sector (both non-Abelian gauge fields and SU2 doublets)
     *
     **/

    class SU2Initializer {
    public:
        /* Put public methods here. These should change very little over time. */
        SU2Initializer() = delete;


        // INITIALIZATION: SU(2) GAUGE FIELDS
        // --> Note: aDot has to be initialized before calling this function.

        template<class Model, typename  T>
        static void initializeSU2(Model& model, FluctuationsGenerator<T>& fg, T kCutOff)
        {

        // 1. We set the homogeneous components and fluctuations of the SU(2) doublets.
            initializeSU2Doublet(model, fg.getBaseSeed(), fg, kCutOff);

        // 2. We now impose fluctuations to the TIME-DERIVATIVES of the non-Abelian
        // gauge fields (the amplitudes are set exactly to 0 at all lattice points).
        // This is done by imposing the Gauss constraint in momentum space, where
        // the current (given by the SU2 doublet components) sources the gauge fields.

            if(model.NSU2 > 0) {

                FourierSite<Model::NDim> ntilde(model.getToolBox());

                size_t N = GetNGrid::get(model); // Reads N (number of points per dimension)

                // We define keffm as the backward (complex) lattice momentum, consistent with
                // choosing the backward spatial derivative in the Gauss constraint

                auto expIK = MakeVector(i, 1, Model::NDim, complexPhase(-2.0 * Constants::pi<T> / N * ntilde(i))); // e^(-2*pi*k/N)
                auto keffm = MakeVector(i, 1, Model::NDim, 1_c - expIK(i));
                auto keffm2 = Total(i, 1, Model::NDim, norm2(keffm(i)));

                // We compute the total SU2 current.
                // As a trick, we use model.fldSU2(1_c) to store it temporarily.
                ForLoop(n,0,Model::NSU2-1,
                    auto j0a = model.fldSU2(n)(1_c)(1_c);
                    auto su2density = MatterCurrents::SU2ChargeDensity(model,n);

                    ForLoop(a, 1, 3,
                            j0a = -model.dx * su2density(a);  // SU2 current

                    // We set the time-derivatives of the gauge fields via
                    // the Gauss constraint in momentum space.
                    ForLoop(i, 1, Model::NDim,
                          model.piSU2(n)(i)(a).inFourierSpace() = asFourier(conj(keffm(i)) * (1 / keffm2)) * j0a.inFourierSpace();
                          model.piSU2(n)(i)(a).inFourierSpace().setZeroMode(0);
                          // Set the zero mode to 0.
                  		);
                  	);

                   // We set the amplitude of the gauge links to unity (gauge fields to 0).
                   ForLoop(i, 1, Model::NDim,
                  		ForLoop(a, 1, 3,
                  			model.fldSU2(n)(i)(a) = 0;
          						);
          					);
                  );
            	}
        }

        // INITIALIZATION: SU2 doublets
        // --> Note: aDot has to be initialized before calling this function.

        template<class Model, typename T>
        static void initializeSU2Doublet(Model& model, std::string baseSeed, FluctuationsGenerator<T>& fg, T kCutOff)
        {
            // 1. We set fluctuations to the SU2 doublets:
            addFluctuationsSU2DoubletFromPhases(model,  baseSeed, fg, model.aDotI, kCutOff);

            // 2. We set the initial homogeneous components of the fields and derivatives.
            //    model.fldSU2Doublet(i) and piSU2Doublet(i) are introduced in physical
            //    (dimensionful variables), so we transform them to program variables
            //    by dividing them by f_* and f_* omega_* respectively.
            ForLoop(i, 0, Model::NSU2Doublet - 1 ,
                    model.fldSU2Doublet(i) += model.fldSU2Doublet0(i) / model.fStar;
            		model.piSU2Doublet(i) += model.piSU2Doublet0(i) / model.fStar / model.omegaStar;
            );
            model.SU2DblPi2AvI = Averages::pi2SU2Doublet(model);
            model.SU2DblPi2AvSI = model.SU2DblPi2AvI;

        }

	// Sets fluctuations to SU2 doublet components
        template<class Model, typename T>
        static void addFluctuationsSU2DoubletFromPhases(Model& model, std::string baseSeed, FluctuationsGenerator<T>& fg,  T aDot, T kCutOff){
            // When imposing initial fluctuations to the SU2 doublets,
            // one must ensure that the Gauss constraints are preserved initially.
            // Here we follow the procedure described in arXiv:2006.15122
            // which guarantees precisely this.
            // In particular, we impose a sum of left-moving and right-moving waves
            // to the four components in momentum space, as for scalar singlets.
            // However, in our procedure, some constraints must be imposed between
            // the wave amplitudes and phases.
            // Note that these fluctuations do not exactly follow a Gaussian distribution.

            auto toolBox = model.getToolBox();

            ForLoop(i,0, Model::NSU2Doublet-1,

                // 1. Random amplitudes for the (left-moving and right-moving) waves:
                auto amplitudes = MakeArray(a,0,3,
                  fg.getFluctuationsNorm(model, model.fldSU2Doublet(i)(a),
                    model.masses2SU2Doublet(i)(a),kCutOff)*
                    RandomRayleighField<T>(baseSeed + "norm" + model.fldSU2Doublet(i)(a).toString(),toolBox));

                // 2. Phases of the waves
                // phases of left-moving waves (for each of the four components), set randomly:
                auto leftPhases = MakeArray(a,0,3,
                  RandomUniformUnitaryField<T>(baseSeed + "phaseLeft" + model.fldSU2Doublet(i)(a).toString(),
                   toolBox));

                // the phase of the right-moving wave of the 0 component is also set randomly:
                auto rightPhase0 = RandomUniformUnitaryField<T>(baseSeed + "phaseRight" + model.fldSU2Doublet(i)(0_c).toString(),
                                   toolBox);
                // the phases of the right-moving waves of
                // the other components (1,2,3) are functions of the other ones:

                auto rightPhases = MakeArray(a,0,3,
                  rightPhase0 * leftPhases(a) * conj(leftPhases(0_c)));
                //adds redundantly the 0 phase.


                // 3. Fluctuations for the amplitudes of the SU(2) doublet components:
                ForLoop(a,0,3,
                    model.fldSU2Doublet(i)(a).inFourierSpace() = amplitudes(a) *
                                                                (leftPhases(a) + rightPhases(a)) / sqrt(2) /sqrt(2);
                                                                // Second sqrt(2) comes from normalisation of SU2 doublet
                    model.fldSU2Doublet(i)(a).inFourierSpace().setZeroMode(0);
                    // Sets the zero mode to 0.
                );

                // 4. Fluctuations for the time-derivatives of the SU(2) doublet components:

                FourierSite<Model::NDim> ntilde(model.getToolBox());
                auto k =   ntilde.norm() * model.kIR;
                auto omega0 = fg.omega_k(k ,model.masses2SU2Doublet(i)(0_c), model.fldSU2Doublet(i)(0_c).toString());
                // effective frequency

                ForLoop(a,0,3,
                    model.piSU2Doublet(i)(a).inFourierSpace() = - Constants::I<T> * omega0 * amplitudes(a) * (leftPhases(a) - rightPhases(a)) / sqrt(2) /sqrt(2)
                    - aDot * model.fldSU2Doublet(i)(a).inFourierSpace() / sqrt(2);

                    model.piSU2Doublet(i)(a).inFourierSpace().setZeroMode(0);
                    // Sets the zero mode to 0.

                );
            );
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
#include "CosmoInterface/initializers/su2initializer_test.h"
#endif


#endif
