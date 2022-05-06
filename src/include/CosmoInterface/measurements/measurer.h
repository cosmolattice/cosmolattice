#ifndef TEMPLAT_COSMOINTERFACE_ABSTRACTMEASURER_H
#define TEMPLAT_COSMOINTERFACE_ABSTRACTMEASURER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include <sstream>
#include "CosmoInterface/runparameters.h"
#include <iomanip>

#include "CosmoInterface/measurements/scalarsingletmeasurer.h"
#include "CosmoInterface/measurements/gwsmeasurer.h"
#include "CosmoInterface/measurements/complexscalarmeasurer.h"
#include "CosmoInterface/measurements/su2doubletmeasurer.h"
#include "CosmoInterface/measurements/u1measurer.h"
#include "CosmoInterface/measurements/su2measurer.h"
#include "CosmoInterface/measurements/energiesmeasurer.h"
#include "CosmoInterface/measurements/scalefactormeasurer.h"
#include "CosmoInterface/measurements/energysnapshotmeasurer.h"

#include "CosmoInterface/evolvers/evolver.h"


namespace TempLat {

    /** \brief A class which contains the measurements.
     *
     *
     **/

    template<typename T = double>
    class Measurer {
    public:
        /* Put public methods here. These should change very little over time. */
        template<class Model>
        Measurer(Model& model, const RunParameters<T>& par) :
          filesManager(par.outFn /*+ model.name*/, model.getToolBox(), false, par.hdf5Spectra, par.printHeaders), // File manager controlling output format and layout, see class.
          outputFreq(par.tOutFreq / par.dt), // Number of steps between frequent output
          infreqOutputFreq(par.tOutInfreq / par.dt),  // Number of steps between infrequent output
          rareOutputFreq(par.tOutRareFreq / par.dt),  // Number of steps between rare output
          verbOutputFreq(par.tOutVerb / par.dt),   // Number of steps between updates in the terminal
          t0(par.t0),   // Initial time of the simulation
          amIRoot(model.getToolBox()->amIRoot()),  // Boolean that returns true if in the root processor
          expansion(par.expansion),    // Type of expansion (no expansion, fixed background, or self-consistent)
          eType(par.eType),   // Type of evolution algorithm (e.g. VV2, LF2...)
          scalarSingletMeasurer(model,filesManager, par, par.appendMode),   // Measurer for scalar fields
          gwsMeasurer(model,filesManager, par, par.appendMode), 
          complexScalarMeasurer(model,filesManager, par, par.appendMode),   // Measurer for complex scalars
          su2DoubletMeasurer(model,filesManager, par, par.appendMode),   // Measurer for SU(2) doublets
          u1Measurer(model,filesManager, par, par.appendMode),   // Measurer for U(1) gauge fields
          su2Measurer(model,filesManager, par, par.appendMode),    // Measurer for SU(2) gauge fields
          energiesMeasurer(model,filesManager, par, par.appendMode),   // Measurer of energies and scale factor
          scaleFactorMeasurer(model,filesManager, par, par.appendMode),   // Measurer of energies and scale factor
          energySnapshotsMeasurer(model, filesManager,  par.energySnapshotMeas),    // Measurer of energy and field snapshots
          spectraTime(filesManager, "spectra_times", amIRoot, par.appendMode, {"tSpectra"}, filesManager.getUseHDF5()),   // Output file that indicates at which times spectra are computed
          PSMeasurer(par),
          // TestTransTrace(par),
          GWsPSMeasurer(par)
        {
        }



        template<typename R, class Model>
        void measure(int n, R t, Model& model)
        {
            bool isInitialTime = (n==0);

            // Frequent output (averages):
            if( (n % outputFreq == 0 )  ){

              if(!expansion){
                  model.potAvI = average(Potential::potential(model));
              }
              //"Standard" measurementsIO (mean, rms etc) of fields and momentum

              scalarSingletMeasurer.measureStandard(model,t);
              // Means, rms, etc, of scalar singlet
              complexScalarMeasurer.measureStandard(model,t);
              // Means, rms, etc, of complex scalar
              su2DoubletMeasurer.measureStandard(model,t);
              // Means, rms, etc, of SU(2) doublet
              u1Measurer.measureStandard(model,t);
              // Means, rms, etc, of U(1) gauge field
              su2Measurer.measureStandard(model,t);
              // Means, rms, etc, of SU(2) gauge field
              energiesMeasurer.measure(model,t, isInitialTime);
              // Energy contributions and conservation check
              scaleFactorMeasurer.measure(model,t);
              // Scale factor and derivatives
              // gwsMeasurer.measureStandard(model,t, TestTransTrace);
              // Transversality and tracelessness test of GWs

              filesManager.flush();
          }

          // Infrequent output (spectra):
          if(n% infreqOutputFreq == 0 ) {

              scalarSingletMeasurer.measureSpectra(model,t, PSMeasurer);
              // Scalar singlet spectra
              gwsMeasurer.measureSpectra(model,t, GWsPSMeasurer);
              // GWs spectra
              complexScalarMeasurer.measureSpectra(model,t, PSMeasurer);
              // Complex scalar spectra
              su2DoubletMeasurer.measureSpectra(model,t, PSMeasurer);
              // SU(2) doublet spectra
              u1Measurer.measureSpectra(model,t, PSMeasurer);
              // Electric and magnetic spectra, U(1) gauge sector
              su2Measurer.measureSpectra(model,t, PSMeasurer);
              // Electric and magnetic spectra, SU(2) gauge sector

              if(!filesManager.getUseHDF5()){
                  spectraTime.addAverage(t);
                  // Each time infrequent output is computed, time is added to the file
                  // average_spectra_time.txt
                  spectraTime.save();
              }

          }
          // Rare output (snapshots):
            if(n % rareOutputFreq == 0){
                energySnapshotsMeasurer.measure(model,t);
                // 3D snapshots of energy contributions and fields
            }
            if ( n % verbOutputFreq == 0 && amIRoot) {
              // If in root processor, print update message in terminal at frequent times.
                model.getToolBox()->resetVerbose();
                sayMPI << "Step " << n << " done. Current time:" << t <<"\n";
            }else model.getToolBox()->unsetVerbose();

        }
         


        bool areWeMeasuring(int n) const
        {
            return (n % outputFreq == 0 || n % infreqOutputFreq == 0 || n % rareOutputFreq == 0 );
        }
		
		
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        FilesManager filesManager;
        int outputFreq, infreqOutputFreq, rareOutputFreq, verbOutputFreq;
        T t0;

        bool amIRoot,expansion;
        EvolverType eType;

        ScalarSingletMeasurer<T> scalarSingletMeasurer;
        GWsMeasurer<T> gwsMeasurer; 
        ComplexScalarMeasurer<T> complexScalarMeasurer;
        SU2DoubletMeasurer<T> su2DoubletMeasurer;
        U1Measurer<T> u1Measurer;
        SU2Measurer<T> su2Measurer;
        EnergiesMeasurer<T> energiesMeasurer;
        ScaleFactorMeasurer<T> scaleFactorMeasurer;
        EnergySnapshotsMeasurer energySnapshotsMeasurer;

        MeasurementsSaver<T> spectraTime;
        PowerSpectrumMeasurer PSMeasurer;
        // CheckTT TestTransTrace;
        GWsPowerSpectrumMeasurer GWsPSMeasurer;




    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/measurements/measurer_test.h"
#endif


#endif
