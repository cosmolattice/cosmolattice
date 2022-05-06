#ifndef COSMOINTERFACE_MEASUREMENTS_SU2MEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_SU2MEASURER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/measurements/meansmeasurer.h"
#include "CosmoInterface/measurements/measurementsIO/spectrumsaver.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "TempLat/util/templatvector.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "CosmoInterface/definitions/gausslaws.h"
#include "CosmoInterface/definitions/fieldfunctionals.h"
#include "CosmoInterface/measurements/powerspectrum.h"
#include "CosmoInterface/runparameters.h"

namespace TempLat {

    /** \brief A class which contains standard measurements for the SU2 gauge fields.
     *
     **/

    template <typename T>
    class SU2Measurer {
    public:
        /* Put public methods here. These should change very little over time. */
        template <typename Model>
        SU2Measurer(Model& model, FilesManager& filesManager, const RunParameters<T>& par, bool append)
        {
            bool amIRoot = model.getToolBox()->amIRoot();

            // We create three files for each SU(2) gauge field:
            ForLoop(i, 0, Model::NSU2 - 1,

                    standardNormOut.emplace_back(
                            MeasurementsSaver<T>(filesManager, "norm_SU2_" + std::to_string(i) , amIRoot, append, MeansMeasurer::headerEB())
                    );
                    // Contains volume-averages of the electric and magnetic fields:
                    // norm squared, norm to the fourth, and variances

                    gauss.emplace_back(
                            MeasurementsSaver<T>(filesManager, "gauss_SU2_" + std::to_string(i), amIRoot, append,
                                  {"t", "var(LHS - RHS)_over_var(LHS + RHS)", "var(LHS)", "var(RHS)" })
                    ); // Checks the degree of conservation of the SU(2) gauss law.

                    spectra.emplace_back(
                            SpectrumSaver<T>(filesManager, "norm_SU2_" + std::to_string(i), amIRoot, append, par)
                    ); // Contains the spectra of the electric and magnetic fields.

            );
        }

        // This measures the corresponding averages with MeansMeasurer::measure, and add them to the files.
        // NOTE: For gauge fields, their momenta is defined as pi=a^(alpha-1)*B'_i, with A'_i the electric field.

        template <typename Model>
        void measureStandard(Model& model, T t) {
            ForLoop(i, 0, Model::NSU2 - 1,
    			auto B = sqrt(FieldFunctionals::B2SU2(model,i));
              	auto E = pow(model.aI, 1 * model.alpha - 1) * sqrt(FieldFunctionals::pi2SU2(model,i));
              	MeansMeasurer::measure(standardNormOut(i),  E, B, t);
              	standardNormOut(i).save();
                gauss(i).addAverage(t);// adds time to the Gauss law file
                auto gaussArr = GaussLaws::checkSU2(model,i);
                // the function returns a 3-component vector with information
                // of the left and right hand sides of the Gauss law.

                gauss(i).addAverage(gaussArr(0)); // var(LHS - RHS)_over_var(LHS + RHS),
                gauss(i).addAverage(gaussArr(1)); // var(LHS)
                gauss(i).addAverage(gaussArr(2)); // and var(RHS)
                gauss(i).save();
            );
        }

        // This measures the electric and magnetic spectra and adds them to the files.
        template <typename Model>
        void measureSpectra(Model& model, T t, PowerSpectrumMeasurer& PSMeasurer) {

            ForLoop(k,0,Model::NSU2-1,

                    auto BSU2 = safeSqrt(FieldFunctionals::B2SU2(model,k));
                    auto ESU2 =  pow(model.aI,  model.alpha - 1) * safeSqrt(FieldFunctionals::pi2SU2(model,k));
                    auto magSpecSU2 = PSMeasurer.powerSpectrum(BSU2);
                    auto elSpecSU2 = PSMeasurer.powerSpectrum(ESU2);

                    spectra(k).save(t, elSpecSU2, magSpecSU2);
            );
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        TempLatVector<MeasurementsSaver<T>> standardNormOut;
        TempLatVector<MeasurementsSaver<T>> gauss;

        TempLatVector<SpectrumSaver<T>> spectra;

        EvolverType eType;

    };

    class SU2MeasurerTester{
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/su2measurer_test.h"
#endif


#endif
