#ifndef COSMOINTERFACE_MEASUREMENTS_GWSMEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_GWSMEASURER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros, Adrien Florio, Nicolás Layza,  Year: 2022

#include "CosmoInterface/measurements/meansmeasurer.h"
#include "CosmoInterface/measurements/measurementsIO/spectrumsaver.h"
#include "CosmoInterface/measurements/measurementsIO/spectrumGWsaver.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "CosmoInterface/measurements/measurementsIO/measurementssaver.h"
#include "TempLat/util/templatvector.h"

#include "CosmoInterface/definitions/GWsProjector.h"
// #include "CosmoInterface/definitions/checkTT.h"

#include "CosmoInterface/measurements/gwspowerspectrum.h"





namespace TempLat {


    /** \brief A class which contains standard measurements for the GWs sector.
     *
     *
     * Unit test: make test-gwsmeasurer
     **/

    template <typename T>
    class GWsMeasurer {
    public:
        /* Put public methods here. These should change very little over time. */
        template <typename Model>
        GWsMeasurer(Model& model, FilesManager& filesManager, const RunParameters<T>& par, bool append):
        PSType(par.powerSpectrumType)
 		{

            bool amIRoot = model.getToolBox()->amIRoot();
            // We create a file containing the spectra
        	spectraOut.emplace_back(
                			SpectrumGWSaver<T>(filesManager, "gws", amIRoot, append, par, !model.fldGWs)
                                    ); // File for spectra
            // standardOut.emplace_back( MeasurementsSaver<T>(filesManager, "TT_Test", amIRoot, append, getTTHeaders(model)) ); // File for TT_check

        }

        // template <typename Model, typename Check>
        // void measureStandard(Model& model, T t, Check& TestTransTrace) {
        //     standardOut(0).addAverage(t);
        //     standardOut(0).addAverage(TestTransTrace.checkTrans(model, 1_c));
        //     standardOut(0).addAverage(TestTransTrace.checkTrans(model, 2_c));
        //     standardOut(0).addAverage(TestTransTrace.checkTrans(model, 3_c));
        //     standardOut(0).addAverage(TestTransTrace.checkTrace(model));
        //     standardOut(0).save();
        // }

        template <typename Model>
        void measureSpectra(Model& model, T t, GWsPowerSpectrumMeasurer& GWsPSMeasurer) {
                   if (model.fldGWs != nullptr) spectraOut(0).save(t, model, GWsPSMeasurer.gwspowerSpectrum(model));


        }

     	// Returns string with the header of the strin parameters file
        // template <typename Model>
        // std::vector<std::string> getTTHeaders(Model& model) const
        // {
        //    std::vector<std::string> ret;
        //    ret.emplace_back("t");
        //    ret.emplace_back("lambda_1");
        //    ret.emplace_back("lambda_2");
        //    ret.emplace_back("lambda_3");
        //    ret.emplace_back("delta");

        //    return ret;
        // }



    private:

        /* Put all member variables and private methods here. These may change arbitrarily. */

        TempLatVector<SpectrumGWSaver<T>> spectraOut;
        TempLatVector<MeasurementsSaver<T>> standardOut;

        const int PSType;
    };


    class GWsMeasurerTester{
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


} /* FCN */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/gwsmeasurer_test.h"
#endif


#endif
