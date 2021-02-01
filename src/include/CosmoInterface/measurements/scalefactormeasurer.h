#ifndef COSMOINTERFACE_MEASUREMENTS_SCALEFACTORMEASURER_H
#define COSMOINTERFACE_MEASUREMENTS_SCALEFACTORMEASURER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2021

#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "CosmoInterface/measurements/measurementsIO/measurementssaver.h"

namespace TempLat {


    /** \brief A class which measure the scale factor, its derivative and the Hubble rate.
     *
     * 
     **/

    template<typename T>
    class ScaleFactorMeasurer {
    public:
        /* Put public methods here. These should change very little over time. */
        template <typename Model>
        ScaleFactorMeasurer(Model& model, FilesManager& filesManager, const RunParameters<T>& par, bool append) :
        expansion(par.expansion),
        scaleOut(filesManager, "scale_factor",  model.getToolBox()->amIRoot(), append, {"t","a","aDot","H"}, !expansion) // Output file for scale factor and derivatives.
        {

        }
        // Measures scale factor and time-derivatives
        template <class Model>
        void measure(Model& model, T t) {
            if (expansion) {
                scaleOut.addAverage(t);  // Time
                scaleOut.addAverage(model.aI);   // Scale factor
                scaleOut.addAverage(model.aDotI);   // First time-derivative of the scale factor
                scaleOut.addAverage(model.aDotI / model.aI);  // Hubble parameter
            }
            scaleOut.save();
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        const bool expansion;
        MeasurementsSaver<T> scaleOut;
    };

    class ScaleFactorMeasurerTester{
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

} /* TempLat */
#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/scalefactormeasurer_test.h"
#endif

#endif
