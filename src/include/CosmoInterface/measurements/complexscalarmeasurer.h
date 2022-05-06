#ifndef COSMOINTERFACE_MEASUREMENTS_COMPLEXSCALARMEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_COMPLEXSCALARMEASURER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/measurements/meansmeasurer.h"
#include "CosmoInterface/measurements/measurementsIO/spectrumsaver.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/measurements/powerspectrum.h"
#include "TempLat/util/templatvector.h"

namespace TempLat {


    /** \brief A class which contains standard measurements for the complex scalars.
     *
     *
     **/

    template <typename T>
    class ComplexScalarMeasurer {
    public:
        /* Put public methods here. These should change very little over time. */
            template <typename Model>
            ComplexScalarMeasurer(Model& model, FilesManager& filesManager, const RunParameters<T>& par, bool append)
            {
            	bool amIRoot = model.getToolBox()->amIRoot();

            // We create THREE files for each complex scalar, containing the averages (mean, rms, etc) of the real and imaginary components and the norm
            // Files are created with the MeasurementsSaver function: the second parameter is added to the file name; the fifth one adds a header to the file
            	ForLoop(i, 0, Model::NCs - 1,
                    standardReOut.emplace_back(  // real part
                            MeasurementsSaver<T>(filesManager, model.fldCS(i)(0_c), amIRoot, append, MeansMeasurer::header())
                    );
                    standardImOut.emplace_back(  // imaginary part
                            MeasurementsSaver<T>(filesManager, model.fldCS(i)(1_c), amIRoot, append, MeansMeasurer::header())
                    );
                    standardNormOut.emplace_back( // norm
                            MeasurementsSaver<T>(filesManager, "norm_cmplx_scalar_" + std::to_string(i) , amIRoot, append, MeansMeasurer::header())
                    );

					// We also create a fourth file containing the spectra of the norm.
                    spectraNormOut.emplace_back(
                            SpectrumSaver<T>(filesManager, "norm_cmplx_scalar_" + std::to_string(i), amIRoot, append, par)
                    );
            );
        }

		// The following function measures the corresponding averages with MeansMeasurer::measure, and adds them to the files.
		// NOTE: For scalar fields, the momenta is defined as pi=a^(3-alpha)*f', with f' the time-derivative; see documentation.
		//       The sqrt(2) is to retrive the field components, real and imaginary, in the
		//following sense: varphi = (varphi_1+ivarphi_2)/sqrt{2}, so we plot the mean of
		//varphi_1 and verphi_2 separately. However, when plotting the modulus we do it of the
		// full field, so |varphi| = sqrt{Re{varphi}^2 + Im{varphi}^2} =
		// = sqrt{varphi_1^2+varphi_2^2}/sqrt{2}.
        template <typename Model>
        void measureStandard(Model& model, T t) {
            ForLoop(i, 0, Model::NCs - 1,
                    MeansMeasurer::measure(standardReOut(i), sqrt(2) * model.fldCS(i)(0_c), sqrt(2) * model.piCS(i)(0_c) * pow(model.aI, model.alpha - 3), t);
                          standardReOut(i).save();
                    MeansMeasurer::measure(standardImOut(i), sqrt(2) *model.fldCS(i)(1_c), sqrt(2) * model.piCS(i)(1_c) * pow(model.aI, model.alpha - 3), t);
                          standardImOut(i).save();
                    MeansMeasurer::measure(standardNormOut(i),  norm(model.fldCS(i)),  norm(model.piCS(i) * pow(model.aI, model.alpha - 3)), t);
                          standardNormOut(i).save();
            );
        }

		// The following function measures the power spectrum of the norm and its time-derivative as the sum of their components.
        template <typename Model>
        void measureSpectra(Model& model, T t, PowerSpectrumMeasurer& PSMeasurer) {
            ForLoop(i, 0, Model::NCs - 1,
                    spectraNormOut(i).save(t,
                     (PSMeasurer.powerSpectrum(model.fldCS(i)(0_c)) + PSMeasurer.powerSpectrum(model.fldCS(i)(1_c))   ),
                             pow(model.aI, 2 * model.alpha - 6) * (PSMeasurer.powerSpectrum(model.piCS(i)(0_c) ) + PSMeasurer.powerSpectrum(model.piCS(i)(1_c)))
                            );
                    );
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        TempLatVector<MeasurementsSaver<T>> standardReOut;    // Contains averages of real part
        TempLatVector<MeasurementsSaver<T>> standardImOut;   // Contains averages of imaginary part
        TempLatVector<MeasurementsSaver<T>> standardNormOut;   // Contains averages of norm

        TempLatVector<SpectrumSaver<T>> spectraNormOut;   // Contains spectra of norm

    };

    class ComplexScalarMeasurerTester{
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/complexscalarmeasurer_test.h"
#endif


#endif
