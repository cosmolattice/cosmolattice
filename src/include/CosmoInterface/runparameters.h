#ifndef TEMPLAT_COSMOINTERFACE_RUNPARAMETERS_H
#define TEMPLAT_COSMOINTERFACE_RUNPARAMETERS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "TempLat/parameters/parameterparser.h"
#include "TempLat/util/constants.h"
#include "TempLat/lattice/latticeparameters.h"
#include "CosmoInterface/evolvers/evolvertype.h"
#include "TempLat/util/almostequal.h"

namespace TempLat {


    /** \brief A class which contains the parameters useful to run a simulation but not model specific (dt for example).
     *
     *
     *
     **/

    MakeException(RunParametersMissing);
    MakeException(RunParametersInconsistent);

    template<typename T = double>
    class RunParameters  {
    public:
        /* Put public methods here. These should change very little over time. */

        // List of run parameters and their default values:
        RunParameters(ParameterParser& par):
                N(par.getOverride<ptrdiff_t>("N",Important)), // Number of lattice points per dimension
                kIR(par.getOverride<T>("kIR",-1,Important)), // IR cutoff
                lSide(par.getOverride<T>("lSide",-1,Important)), // Side length
                dt(par.get<T>("dt",Important)),  // Time step
                expansion(par.get<bool>("expansion",true,Important)), // If true: self-consistent expansion. If false: no expansion
                t0(par.get<double>("t0",0,Important)), // Initial time
                tMax(par.get<T>("tMax",10000 * dt,Important)),  // Final time
                kCutoff(par.get<T>("kCutOff",std::numeric_limits<double>::infinity(),Important)), // Momenta cutoff in spectra of initial fluctuations
                tOutFreq(par.get<T>("tOutputFreq",10 * dt,Important)), // Printing time interval of frequent output
                tOutInfreq(par.get<T>("tOutputInfreq",100 * dt,Important)),  // Printing time interval of infrequent output
                tOutRareFreq(par.get<T>("tOutputRareFreq",1000 * dt,Important)),  // Printing time interval of rare output
                tOutVerb(par.get<T>("tOutputVerb",100 * dt)),  // Output verbosity
                tBackupFreqFloat(par.get<T>("tBackupFreq",-1)),   // Frequency of backups
                baseSeed(par.getSeed("baseSeed")), // Seed for random generator of initial fluctuations
                outFn(par.get<std::string>("outputfile","./")()),  // Folder where output is saved
                energySnapshotMeas(par.get<std::string,10>("energy_snapshot", std::vector<std::string>(10, Constants::defaultString))),  // Energy terms for which snapshots are printed
                fixedBackground(expansion ? par.get<bool>("fixedBackground",false) : false), // If true, expansion is given by a fixed background
                omegaEoS(fixedBackground ? par.get<T>("omegaEoS",1.0 / 3.0) : 0.0), // For fixed background expansion: equation of state
                H0(fixedBackground ? par.get<T>("H0") : 0.0), // For fixed background expansion: initial Hubble parameter
                spectraVerbosity(par.get<int>("spectraVerbosity",0)), // Verbosity of spectra files
                deltaKBin(par.get<double>("deltaKBin",1)), // Bin width of the spectra
                nBinsSpectra(floor(sqrt(3.0)/2.0 * N / deltaKBin)), // Number of bins in spectra
                hdf5Spectra(par.get<bool>("hdf5Spectra",false)), // If true, spectra are printed in HDF5 format. If false, printed in txt format.
                eType(par.get<EvolverType>("evolver",LF,Important)), // Type of evolution algorithm
                appendMode(par.getOverride<bool>("appendToFiles", false)), // If true, output will be appended to pre-existing files. If false, files will be overwritten.
                saveEndPath(par.get<std::string>("save_dir",Constants::defaultString)()), // Folder where simulation is saved at the end
                backupPath(par.get<std::string>("backup_dir",Constants::defaultString)()), // Folder where simulation is saved during the simulation
                printHeaders(par.get<bool>("print_headers", false)), // If true, headers are printed in all output files
                doWeRestart(false), //Boolean which tells if we are runing in restart mode or not. Set in the main.
                tolerance(par.get<T>("tolerance", -1)), //For adaptative solvers only
                powerSpectrumType(par.get<int>("PS_type",1)),
                powerSpectrumVersion(par.get<int>("PS_version",1)),
                GWprojectorType(par.get<int>("GWprojectorType",2)), // Type of GWprojector (real = 1, backwards = 2 (default), forward = 3)
                withGWs(par.get<bool>("withGWs", false, Important)),
                flagON(par.get<bool>("OccNumb", false))
        {
            if (AlmostEqual(lSide, -1)) {
                if (AlmostEqual(kIR, -1))
                    throw (RunParametersMissing("You need to specify either kIR or lSide, abort."));
                else lSide = 2 * Constants::pi<T> / kIR;
                par.erase("lSide"); // To not have wrong info in the parameter of the parser.
            } else if (AlmostEqual(kIR, -1)) {
                kIR = 2 * Constants::pi<T> / lSide;
                par.erase("kIR");
            } else if (!AlmostEqual(kIR, 2 * Constants::pi<T> / lSide))
                throw (RunParametersInconsistent(
                        "kIR = " + std::to_string(kIR) + " and lSide = " + std::to_string(lSide) +
                        " are not consistent. If you think they should be, try removing one of the two."));
            // NOTE: We must have kIR=2*pi/lside for consistency.

            dx = lSide / N; // Lattice spacing
            kUV = std::sqrt(3) * Constants::pi<T> / dx; //Maximum momenta in the lattice

            // Now we see if the user wants to save the simulation at the end of the run. This is
            // specified by specifying a path different from the Constants::defaultString. If not
            // remove the save_dir from the ParameterParser.
            if (saveEndPath == Constants::defaultString) {
                boolSaveEnd = false;
                par.erase("save_dir");
            }
            else {
                boolSaveEnd = true;
            }
            // Check if we want to backup the simulation. This is turned on by specifying a positive backup frequency.
            if(tBackupFreqFloat < 0) boolBackup = false;
            else{
                boolBackup = true;
                tBackupFreqInt = tBackupFreqFloat / dt;
            }
            // Lastly, check if the user wants the backup to be saved somewhere specific. If not, it will be saved in the
            // same folder than the measurements.
            if(backupPath == Constants::defaultString){
              backupPath = outFn;
              par.erase("backup_dir");
            }

            if(powerSpectrumType < 1 or  powerSpectrumType > 2)  throw(RunParametersInconsistent("powerSpectrumType " + std::to_string(powerSpectrumType) + " is not a valid powerSpectrumType."));
            if(powerSpectrumVersion < 1 or  powerSpectrumType > 3) throw(RunParametersInconsistent("powerSpectrumVersion " + std::to_string(powerSpectrumVersion) + " is not a valid powerSpectrumVersion."));
            if(spectraVerbosity < 0 or  spectraVerbosity > 2) throw(RunParametersInconsistent("spectraVerbosity " + std::to_string(spectraVerbosity) + " is not a valid spectraVerbosity."));
        }

        void setDoWeRestart(bool pDoWeRestart)
        {
            doWeRestart = pDoWeRestart;
        }


    public:
      const int N;
      T kUV;
      T kIR;
      T lSide;
      T dx;
      T dt;
      
      const bool expansion;

      const T t0;
      const T tMax;
      const T kCutoff;
      const T tOutFreq;
      const T tOutInfreq;
      const T tOutRareFreq;
      const T tOutVerb;
      const T tBackupFreqFloat;
      ptrdiff_t tBackupFreqInt;
      const std::string baseSeed;
      const std::string outFn;
      std::vector<std::string> energySnapshotMeas;

      const bool fixedBackground;

      T omegaEoS, H0;

      const int spectraVerbosity;
      const double deltaKBin;
      const int nBinsSpectra;


      const bool hdf5Spectra;

      const EvolverType eType;

      bool boolSaveEnd;
      bool boolBackup;

      bool appendMode;

      std::string saveEndPath;
      std::string backupPath;

      const bool printHeaders;

      mutable bool doWeRestart;

      T tolerance; //For adaptative solvers only.

      const int powerSpectrumType;
      const int powerSpectrumVersion;

      const int GWprojectorType;
      const bool withGWs;
      const bool flagON;



      LatticeParameters<T> getLatParams()
      {
        return LatticeParameters<T>(dx, lSide, kIR);
      }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<typename R>
    auto createParams(int argc, char* argv[])
    {
      return std::make_shared<R>(argc,argv);
    }



} /* TempLat */

#ifdef TEMPLATTEST
#include "CosmoInterface/runparameters_test.h"
#endif


#endif
