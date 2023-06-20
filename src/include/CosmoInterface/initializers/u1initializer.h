#ifndef COSMOINTERFACE_INITIALIZERS_U1INITIALIZER_H
#define COSMOINTERFACE_INITIALIZERS_U1INITIALIZER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/constants.h"
#include "TempLat/lattice/algebra/algebra.h"
#include "TempLat/lattice/algebra/helpers/getngrid.h"
#include "TempLat/lattice/algebra/complexalgebra/asfourier.h"

#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "CosmoInterface/initializers/fluctuationsgenerator.h"
#include "CosmoInterface/definitions/mattercurrents.h"
#include "CosmoInterface/definitions/averages.h"
#include "CosmoInterface/definitions/potential.h"

namespace TempLat {


    /** \brief A class that initializes the U(1) gauge sector (both Abelian gauge fields and complex scalars)
     *
     **/

    class U1Initializer {
    public:
        /* Put public methods here. These should change very little over time. */
        U1Initializer() = delete;


		// INITIALIZATION: U(1) GAUGE FIELDS
 		// --> Note: aDot has to be initialized before calling this function.

        template<class Model, typename T>
        static void initializeU1(Model& model, FluctuationsGenerator<T>& fg, T kCutOff)
        {
        	// 1. We set the homogeneous components and fluctuations for the complex scalar fields.
            initializeCScalar(model, fg.getBaseSeed(), fg, kCutOff);

			// 2. We now impose fluctuations to the TIME-DERIVATIVES of the Abelian gauge fields (the amplitudes are set exactly to 0 at all lattice points)
			// This is done by imposing the Gauss constraint in momentum space, in which the current (given by the complex scalars) sources the gauge fields.

            if(model.NU1 > 0) {
                FourierSite<Model::NDim> ntilde(model.getToolBox());

                size_t N = GetNGrid::get(model); // Reads N (number of points per dimension)

				// We define keffm as the backward (complex) lattice momentum, consistent with the backward spatial derivative in the Gauss constraint
                auto expIK = MakeVector(i, 1, Model::NDim, complexPhase(-2.0 * Constants::pi<T> / N * ntilde(i))); // e^(-2*pi*k/N)
                auto keffm = MakeVector(i, 1, Model::NDim, 1_c - expIK(i));
                auto keffm2 = Total(i, 1, Model::NDim, norm2(keffm(i)));

				// We compute the total Abelian current. As a trick, we use model.fldU1(1_c) to store it temporally.
                ForLoop(a, 0, Model::NU1-1,
                    auto j0 = model.fldU1(a)(1_c);
                        j0 = -pow<2>(model.dx)* MatterCurrents::U1ChargeDensity(model,a); // U1 current

				// We set the time-derivatives of the gauge fields via the Gauss constraint in momentum space.
                	ForLoop(i, 1, Model::NDim,
                        model.piU1(a)(i).inFourierSpace() =
                                asFourier(conj(keffm(i)) * (1 / keffm2)) * j0.inFourierSpace();

                	model.piU1(a)(i).inFourierSpace().setZeroMode(0); // we set the zero mode to 0.
                	);

                	ForLoop(i, 1, Model::NDim,
                        model.fldU1(a)(i) = 0;   // we set the amplitude of the gauge fields exactly to zero.
                );
                );
            }
        }

    private:

    	// INITIALIZATION: COMPLEX SCALARS
 		// --> Note: aDot has to be initialized before calling this function.

        template<class Model, typename T>
        static void initializeCScalar(Model& model, std::string baseSeed, FluctuationsGenerator<T>& fg, T kCutOff)
        {
        	// 1. We set fluctuations to the complex scalars:
            addFluctuationsCScalarFromPhases(model, baseSeed, fg, model.aDotI, kCutOff);

            // 2. We set the initial homogeneous components of the fields and derivatives.
            // 		model.fldCS0(i) and model.piCS0(i) are introduced in GeV,
            // 		so we transform them to program variables by dividing by f_* and f_* omega_* respectively.
            ForLoop(i,0,Model::NCs - 1,
            	model.fldCS(i) += model.fldCS0(i) / model.fStar;
            	model.piCS(i) +=  model.piCS0(i) / model.fStar / model.omegaStar;
            );

        }

		// Sets fluctuations to complex scalar components
        template<class Model, typename T>
        static void addFluctuationsCScalarFromPhases(Model& model, std::string baseSeed, FluctuationsGenerator<T>& fg, T aDot, T kCutOff){

            // When imposing initial fluctuations to the complex scalars, one must ensure that the Gauss constraints are preserved initially.
            // Here we follow the procedure described in arXiv:2006.15122 which guarantees precisely this.
            // In particular, we impose a sum of left-moving and right-moving waves to the two components in momentum space, like for scalar singlets.
            // However, in our procedure, some constraints must be imposed between the wave amplitudes and phases.
            // Note that these fluctuations will not be exactly Gaussian.

            auto toolBox = model.getToolBox();

            ForLoop(i, 0, Model::NCs - 1,

            		// 1. Norm of the (left-moving and right-moving) waves:
                    auto fFluctuationNorm0 =  fg.getFluctuationsNorm(model, model.fldCS(i)(0_c),model.masses2CS(i)(0_c), kCutOff); // component 0
                    auto fFluctuationNorm1 =  fg.getFluctuationsNorm(model, model.fldCS(i)(1_c),model.masses2CS(i)(1_c), kCutOff); // component 1

					// 2. Random amplitudes for the (left-moving and right-moving) waves:
                    auto a0 = fFluctuationNorm0 * RandomRayleighField<T>(baseSeed + "norm0" + model.fldCS(i)(0_c).toString(),toolBox); // component 0
                    auto a1 = fFluctuationNorm1 * RandomRayleighField<T>(baseSeed + "norm1" + model.fldCS(i)(1_c).toString(),toolBox);  // component 1

					// 3. Random phases for the waves:
					// 	-->	Note: We only generate three of the four phases randomly, the fourth is imposed so that the Gauss constraint is preserved initially, see documentation.
                    auto eitheta00 = RandomUniformUnitaryField<T>(baseSeed + "phase00" + model.fldCS(i)(0_c).toString(), toolBox); // left-moving wave phase, component 0
                    auto eitheta01 = RandomUniformUnitaryField<T>(baseSeed + "phase01" + model.fldCS(i)(0_c).toString(), toolBox); // right-moving wave phase, component 0
                    auto eitheta10 = RandomUniformUnitaryField<T>(baseSeed + "phase10" + model.fldCS(i)(1_c).toString(), toolBox); // left-moving wave phase, component 1
                    auto eitheta11 = eitheta01 * eitheta10 * conj(eitheta00);  // right-moving wave phase, component 1 (depends on the other three!)

					// 4. Fluctuations for the complex field components, imposed as a sum of left-moving and right-moving waves in momentum space:
                    model.fldCS(i)(0_c).inFourierSpace() = a0 * (eitheta00 + eitheta01) / sqrt(2) / sqrt(2); // component 0. Second sqrt(2) comes from normalisation of complex scalar.
                    model.fldCS(i)(0_c).inFourierSpace().setZeroMode(0);  // sets the zero mode to 0
                    model.fldCS(i)(1_c).inFourierSpace() = a1 * (eitheta10 + eitheta11) / sqrt(2) / sqrt(2); // component 1
                    model.fldCS(i)(1_c).inFourierSpace().setZeroMode(0); // sets the zero mode to 0

					// 5. Fluctuations for the complex field time-derivatives:

					// --> First we define the effective frequency of the mode...
                    FourierSite<Model::NDim> ntilde(model.getToolBox());  // Fourier lattice site
                    auto k =  ntilde.norm() * model.getOneField().getKIR();  // comoving momenta (in program units)
                    auto omega0 = fg.omega_k(k,model.masses2CS(i)(0_c), model.fldCS(i)(0_c).toString());  // omega^2 = k^2 + m^2


					// --> ... and then set the fluctuations of the derivatives.
                    model.piCS(i)(0_c).inFourierSpace() = - Constants::I<T> * omega0 * a0 * (eitheta00 - eitheta01) / sqrt(2) / sqrt(2) - aDot * model.fldCS(i)(0_c).inFourierSpace(); // component 0
                    model.piCS(i)(0_c).inFourierSpace().setZeroMode(0); // sets the zero mode to 0
                    model.piCS(i)(1_c).inFourierSpace() = - Constants::I<T> * omega0 * a1 * (eitheta10 - eitheta11) / sqrt(2) / sqrt(2)  - aDot * model.fldCS(i)(1_c).inFourierSpace();  // component 1
                    model.piCS(i)(1_c).inFourierSpace().setZeroMode(0); // sets the zero mode to 0

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
#include "CosmoInterface/initializers/u1initializer_test.h"
#endif


#endif
