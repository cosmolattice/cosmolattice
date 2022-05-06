#ifndef COSMOINTERFACE_MEASUREMENTS_U1MEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_U1MEASURER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/measurements/meansmeasurer.h"
#include "CosmoInterface/measurements/measurementsIO/spectrumsaver.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "TempLat/util/templatvector.h"
#include "TempLat/util/function.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/lattice/algebra/gaugealgebra/magneticfield.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "CosmoInterface/measurements/powerspectrum.h"
#include "CosmoInterface/definitions/gausslaws.h"


namespace TempLat {


    /** \brief A class which contains standard measurements for the U1 gauge fields.
     *
     *
     **/

    template <typename T>
    class U1Measurer {
    public:
        /* Put public methods here. These should change very little over time. */
        template <typename Model>
        U1Measurer(Model& model, FilesManager& filesManager, const RunParameters<T>& par, bool append)
        {
            bool amIRoot = model.getToolBox()->amIRoot();


			ForLoop(i, 0, Model::NU1 - 1,

				// We create three files for each U(1) gauge field:

                standardNormOut.emplace_back(
                        MeasurementsSaver<T>(filesManager, "norm_U1_" + std::to_string(i) , amIRoot, append, MeansMeasurer::headerEB())
                ); // Contains volume-averages of the electric and magnetic fields: norm squared, norm to the fourth, and variances


                gauss.emplace_back(
                        MeasurementsSaver<T>(filesManager, "gauss_U1_" + std::to_string(i) , amIRoot, append,
                                {"t", "var(LHS - RHS)_over_var(LHS + RHS)", "var(LHS)", "var(RHS)" })
                ); // Checks the degree of conservation of the U(1) gauss law.

                spectra.emplace_back(
                        SpectrumSaver<T>(filesManager, "norm_U1_" + std::to_string(i), amIRoot, append, par)
                );  // Contains the spectra of the electric and magnetic fields.
            );
        }



        // This measures the corresponding averages with MeansMeasurer::measure, and add them to the files.
		// NOTE: For gauge fields, their momenta is defined as pi=a^(alpha-1)*A'_i, with A'_i the electric field.
        template <typename Model>
        void measureStandard(Model& model, T t) {
            ForLoop(i, 0, Model::NU1 - 1,
                    MeansMeasurer::measure(standardNormOut(i), norm(model.piU1(i) * pow(model.aI, model.alpha - 1)), norm(magneticField(model.fldU1(i))), t);
                            standardNormOut(i).save();

                    gauss(i).addAverage(t); // adds time to the Gauss law file
                    auto gaussU1Arr = GaussLaws::checkU1(model,i); // the function returns a 3-component vector with information of the left and right hand sides of the Gauss law.
                    gauss(i).addAverage(gaussU1Arr(0)); // var(LHS - RHS)_over_var(LHS + RHS),
                    gauss(i).addAverage(gaussU1Arr(1)); // var(LHS),
                    gauss(i).addAverage(gaussU1Arr(2)); // and var(RHS)
                    gauss(i).save();
            );
        }

        // This measures the electric and magnetic spectra and adds them to the files.
        template <typename Model>
        void measureSpectra(Model& model, T t, PowerSpectrumMeasurer& PSMeasurer) {
            ForLoop(k,0,Model::NU1-1,  // Iterates over all gauge fields
                auto magSpecU1 = Total(i, 1, Model::NDim,    // Sums over x, y, z spatial directions
                                       Function(ntilde, pow<2>(ntilde * model.kIR)) * PSMeasurer.powerSpectrum(model.fldU1(k)(i)));
                auto elSpecU1 = Total(i, 1, Model::NDim, pow(model.aI, 2 * model.alpha - 2) * PSMeasurer.powerSpectrum(model.piU1(k)(i)));


                spectra(k).save(t,elSpecU1,magSpecU1);
            );
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        TempLatVector<MeasurementsSaver<T>> standardNormOut;
        TempLatVector<MeasurementsSaver<T>> gauss;
        TempLatVector<SpectrumSaver<T>> spectra;
    };

    class  U1MeasurerTester{
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/u1measurer_test.h"
#endif


#endif
