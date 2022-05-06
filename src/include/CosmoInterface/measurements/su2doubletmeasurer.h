#ifndef COSMOINTERFACE_MEASUREMENTS_SU2DOUBLETMEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_SU2DOUBLETMEASURER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/measurements/meansmeasurer.h"
#include "CosmoInterface/measurements/measurementsIO/spectrumsaver.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "TempLat/util/templatvector.h"
#include "CosmoInterface/measurements/powerspectrum.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"

namespace TempLat {


    /** \brief A class which contains standard measurements for the SU2 doublets.
     *
     *
     **/

    template <typename T>
    class SU2DoubletMeasurer{
    public:
        /* Put public methods here. These should change very little over time. */
        template<class Model>
        SU2DoubletMeasurer(Model& model, FilesManager& filesManager, const RunParameters<T>& par, bool append) {
            bool amIRoot = model.getToolBox()->amIRoot();

            // We create four files for the averages of each component of the SU(2) doublet, another for the averages of the norm,
            // and another for the spectra of the norm:
            ForLoop(k,0,Model::NSU2Doublet -1 ,
                    standardCompOut.emplace_back({});
                    ForLoop(i, 0, 3,   // Averages of the SU(2) doublet components
                            standardCompOut.back().emplace_back(
                            	MeasurementsSaver<T>(filesManager, model.fldSU2Doublet(k)(i), amIRoot, append, MeansMeasurer::header()))
                    );
                    standardNormOut.emplace_back(   // Averages of the norm
                    	MeasurementsSaver<T>(filesManager, "norm_SU2Doublet_"+std::to_string(k), amIRoot, append, MeansMeasurer::header()));
                    spectraNormOutFld.emplace_back(   // Spectra of the norm
                            SpectrumSaver<T>(filesManager, "norm_SU2Doublet_scalar_" + std::to_string(k), amIRoot, append, par)
                    );
            );
        }

        // We measure the corresponding averages with MeansMeasurer::measure, and add them to the files.
		// NOTE: For scalar fields, the momenta is defined as pi=a^(3-alpha)*f', with f' the time-derivative; see documentation.
		//       The sqrt(2) is to retrive the field components, real and imaginary, of each
		// Top and Bottom components of the Doublet,  in the following sense:
		//Phi = (Phi_Top,Phi_Bottom)^T = (varphi_Top,varphi_Bottom)^T/sqrt{2}, where
		//varphi_Top = (varphi_0+i*varphi_1), varphi_Bottom = (varphi_2+i*varphi_3)
		// [equivalently Phi_Top = (varphi_0+i*varphi_1)/sqrt{2} and similarly for Phi_Bottom],
		//so we plot the mean of varphi_i (i = 0,1,2,3) separately. However, when plotting
		//the modulus we do it of the full Doublet field, so
		//|Phi| = sqrt{varphi_0^2+varphi_1^2+varphi_2^2+varphi_3^2}/sqrt{2}., or equivalently:
		//|Phi| = sqrt{Re{Phi_Top}^2+Im{Phi_Top}^2+Re{Phi_Bottom}^2+Im{Phi_Bottom}^2}
		template <typename Model>
        void measureStandard(Model& model, T t) {
            ForLoop(i, 0, Model::NSU2Doublet - 1,
                            ForLoop(k, 0, 3,
                                    MeansMeasurer::measure(standardCompOut(i)(k), sqrt(2) * model.fldSU2Doublet(i)(k), sqrt(2) * model.piSU2Doublet(i)(k) * pow(model.aI, model.alpha - 3), t);
                                    standardCompOut(i)(k).save();

                            );
                            MeansMeasurer::measure(standardNormOut(i),  norm(model.fldSU2Doublet(i)), norm(model.piSU2Doublet(i)) * pow(model.aI, model.alpha - 3), t);
                            standardNormOut(i).save();
            );
        }

        // We measure the power spectrum of the norm and its time-derivative as the sum of its components.
        template <typename Model>
        void measureSpectra(Model& model, T t, PowerSpectrumMeasurer& PSMeasurer) {
            ForLoop(i, 0, Model::NSU2Doublet - 1,
                    spectraNormOutFld(i).save(t,
                            Total(j,0,3, PSMeasurer.powerSpectrum(model.fldSU2Doublet(i)(j))),
                            Total(j,0,3,  pow(model.aI, 2 * model.alpha - 6) * PSMeasurer.powerSpectrum(model.piSU2Doublet(i)(j)))
                            );
            );
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        TempLatVector<TempLatVector<MeasurementsSaver<T>>> standardCompOut; //Two dimensionnal. One dimension is number of doublet, second is the component.
        TempLatVector<MeasurementsSaver<T>> standardNormOut;

        TempLatVector<SpectrumSaver<T>> spectraNormOutFld;

        EvolverType eType;

    };

    class SU2DoubletMeasurerTester{
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/su2doubletmeasurer_test.h"
#endif


#endif
