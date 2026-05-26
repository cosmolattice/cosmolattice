#ifndef TEMPLAT_COSMOINTERFACE_RUNPARAMETERS_H
#define TEMPLAT_COSMOINTERFACE_RUNPARAMETERS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "TempLat/parameters/parameterparser.h"
#include "TempLat/util/constants.h"
#include "TempLat/lattice/latticeparameters.h"
#include "TempLat/util/almostequal.h"
#include "TempLat/util/floattostring.h"
#include "CosmoInterface/evolvers/evolvertype.h"
#include "CosmoInterface/initializers/initialconditionstype.h"

namespace TempLat
{
  MakeException(RunParametersMissing);
  MakeException(RunParametersInconsistent);

  /** @brief A class which contains the parameters useful to run a simulation but not model specific (dt for example).
   *
   *
   **/
  template <typename T = double> class RunParameters
  {
  public:
    // Put public methods here. These should change very little over time.

    // List of run parameters and their default values:
    RunParameters(ParameterParser &par)
        : N(par.getOverride<ptrdiff_t>("N", Important)),     // Number of lattice points per dimension
          kIR(par.getOverride<T>("kIR", -1, Important)),     // IR cutoff
          lSide(par.getOverride<T>("lSide", -1, Important)), // Side length
          dt(par.get<T>("dt", Important)),                   // Time step
          expansion(par.get<bool>("expansion", true,
                                  Important)),             // If true: self-consistent expansion. If false: no expansion
          t0(par.get<double>("t0", 0, Important)),         // Initial time
          tMax(par.get<T>("tMax", 10000 * dt, Important)), // Final time
          kCutoff(par.get<T>("kCutOff", std::numeric_limits<double>::infinity(),
                             Important)),                          // Momenta cutoff in spectra of initial fluctuations
          SIC(par.get<InitialConditionsType::S>("ICtype_S", InitialConditionsType::S::Default)),
          U1IC(par.get<InitialConditionsType::U1>("ICtype_U1", InitialConditionsType::U1::Default)),
          tOutFreq(par.get<T>("tOutputFreq", 10 * dt, Important)), // Printing time interval of frequent output
          tOutInfreq(par.get<T>("tOutputInfreq", 100 * dt, Important)), // Printing time interval of infrequent output
          tOutRareFreq(par.get<T>("tOutputRareFreq", 1000 * dt, Important)), // Printing time interval of rare output
          tOutVerb(par.get<T>("tOutputVerb", 100 * dt)),                     // Output verbosity
          tBackupFreqFloat(par.get<T>("tBackupFreq", -1)),                   // Frequency of backups
          baseSeed(par.getSeed("baseSeed")),                 // Seed for random generator of initial fluctuations
          outFn(par.get<std::string>("outputfile", "./")()), // Folder where output is saved
          energySnapshotMeas(par.get<std::string, 14>(
              "energy_snapshot",
              std::vector<std::string>(14, Constants::defaultString))), // Energy terms for which snapshots are printed
          snapLower(par.get<ptrdiff_t, 10>(
              "snap_lowercoord",
              std::vector<ptrdiff_t>(10, 0))), // Lower coordinates of the snapshoted volumes, in all dimensions
          snapUpper(par.get<ptrdiff_t, 10>(
              "snap_uppercoord",
              std::vector<ptrdiff_t>(10, N))), // Upper coordinates of the snapshoted volumes, in all dimensions
          snapStep(
              par.get<ptrdiff_t, 10>("snap_stepcoord", std::vector<ptrdiff_t>(10, 1))), // Step used for snapshoting
          fixedBackground(expansion ? par.get<bool>("fixedBackground", false)
                                    : false), // If true, expansion is given by a fixed background
          omegaEoS(fixedBackground ? par.get<T>("omegaEoS", 1.0 / 3.0)
                                   : 0.0),              // For fixed background expansion: equation of state
          H0(fixedBackground ? par.get<T>("H0") : 0.0), // For fixed background expansion: initial Hubble parameter
          a0(par.get<T>("a0", 1.0)),                    // Initial scale factor (default 1.0)
          spectraVerbosity(par.get<int>("spectraVerbosity", 0)), // Verbosity of spectra files
          deltaKBin(par.get<double>("deltaKBin", 1)),            // Bin width of the spectra
          nBinsSpectra(floor(sqrt(3.0) / 2.0 * N / deltaKBin)),  // Number of bins in spectra
          hdf5Averages(par.get<bool>("hdf5Averages", false)), hdf5FlushFreq(par.get<ptrdiff_t>("hdf5FlushFreq", 10)),
          hdf5Spectra(par.get<bool>(
              "hdf5Spectra", false)), // If true, spectra are printed in HDF5 format. If false, printed in txt format.
          eType(par.get<EvolverType>("evolver", LF, Important)), // Type of evolution algorithm
          appendMode(par.get<bool>(
              "appendToFiles",
              false)), // If true, output is appended to pre-existing files. If false, existing files cause an error unless overwriteFiles=true.
          overwriteMode(par.get<bool>(
              "overwriteFiles",
              false)), // If true (and appendToFiles=false), existing output files are deleted before writing.
          saveEndPath(par.get<std::string>("save_dir",
                                           Constants::defaultString)()), // Folder where simulation is saved at the end
          backupPath(par.get<std::string>(
              "backup_dir", Constants::defaultString)()),      // Folder where simulation is saved during the simulation
          printHeaders(par.get<bool>("print_headers", false)), // If true, headers are printed in all output files
          fnVerbosity(par.get<int>(
              "fn_verbosity",
              0)),            // Different verbosity in the output filename. By default, no info about model or params.
          doWeRestart(false), // Boolean which tells if we are runing in restart mode or not. Set in the main.
          tolerance(par.get<T>("tolerance", -1)), // For adaptative solvers only
          powerSpectrumType(par.get<int>("PS_type", 1)), powerSpectrumVersion(par.get<int>("PS_version", 1)),
          withGWs(par.get<bool>("withGWs", false, Important)),
          eTypeGW(par.get<bool>("doLFforGWs", true, Important) ? LF : eType), // Type of evolution algorithm
          GWprojectorType(par.get<int>("GWprojectorType", 2)), // Type of GWprojector (real = 1, backwards = 2 (default), forward = 3)
          flagON(par.get<bool>("flagON", false)),
          flagChiralPS(par.get<bool>("flagChiralPS", false)),
          unbinnedSpectra(par.get<bool>("saveUnbinnedSpectra", false))
    {
      if (AlmostEqual(lSide, -1)) {
        if (AlmostEqual(kIR, -1))
          throw(RunParametersMissing("You need to specify either kIR or lSide, abort."));
        else
          lSide = 2 * Constants::pi<T> / kIR;
        par.erase("lSide"); // To not have wrong info in the parameter of the parser.
      } else if (AlmostEqual(kIR, -1)) {
        kIR = 2 * Constants::pi<T> / lSide;
        par.erase("kIR");
      } else if (!AlmostEqual(kIR, 2 * Constants::pi<T> / lSide))
        throw(RunParametersInconsistent(
            "kIR = " + std::to_string(kIR) + " and lSide = " + std::to_string(lSide) +
            " are not consistent. If you think they should be, try removing one of the two."));
      // NOTE: We must have kIR=2*pi/lside for consistency.

      if (appendMode && overwriteMode)
        throw(RunParametersInconsistent(
            "'appendToFiles' and 'overwriteFiles' cannot both be true. Pick one."));

      if (AlmostEqual(dt, 0.)) throw(RunParametersInconsistent("dt cannot be zero, abort."));
      if (tMax < t0) throw(RunParametersInconsistent("tMax must be >= t0, abort."));
      if (tOutFreq < dt) throw(RunParametersInconsistent("tOutputFreq must be >= dt, abort."));
      if (tOutInfreq < dt) throw(RunParametersInconsistent("tOutputInfreq must be >= dt, abort."));
      if (tOutRareFreq < dt) throw(RunParametersInconsistent("tOutputRareFreq must be >= dt, abort."));
      if (tOutVerb < dt) throw(RunParametersInconsistent("tOutputVerb must be >= dt, abort."));

      dx = lSide / N;                             // Lattice spacing
      kUV = std::sqrt(3) * Constants::pi<T> / dx; // Maximum momenta in the lattice

      // Now we see if the user wants to save the simulation at the end of the run. This is
      // specified by specifying a path different from the Constants::defaultString. If not
      // remove the save_dir from the ParameterParser.
      if (saveEndPath == Constants::defaultString) {
        boolSaveEnd = false;
        par.erase("save_dir");
      } else {
        boolSaveEnd = true;
      }
      // Check if we want to backup the simulation. This is turned on by specifying a positive backup frequency.
      if (tBackupFreqFloat < 0)
        boolBackup = false;
      else {
        boolBackup = true;
        tBackupFreqInt = tBackupFreqFloat / dt;
      }
      // Lastly, check if the user wants the backup to be saved somewhere specific. If not, it will be saved in the
      // same folder than the measurements.
      if (backupPath == Constants::defaultString) {
        backupPath = outFn;
        par.erase("backup_dir");
      }

      if (powerSpectrumType < 0 or powerSpectrumType > 2)
        throw(RunParametersInconsistent("powerSpectrumType " + std::to_string(powerSpectrumType) +
                                        " is not a valid powerSpectrumType."));
      if (powerSpectrumVersion < 1 or powerSpectrumVersion > 3)
        throw(RunParametersInconsistent("powerSpectrumVersion " + std::to_string(powerSpectrumVersion) +
                                        " is not a valid powerSpectrumVersion."));
      if (spectraVerbosity < 0 or spectraVerbosity > 2)
        throw(RunParametersInconsistent("spectraVerbosity " + std::to_string(spectraVerbosity) +
                                        " is not a valid spectraVerbosity."));
    }

    void setDoWeRestart(bool pDoWeRestart) { doWeRestart = pDoWeRestart; }

    ptrdiff_t getFlushFreq() const { return hdf5FlushFreq; }

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
    const InitialConditionsType::S SIC;
    const InitialConditionsType::U1 U1IC;
    const T tOutFreq;
    const T tOutInfreq;
    const T tOutRareFreq;
    const T tOutVerb;
    const T tBackupFreqFloat;
    ptrdiff_t tBackupFreqInt;
    const std::string baseSeed;
    const std::string outFn;
    std::vector<std::string> energySnapshotMeas;
    std::vector<ptrdiff_t> snapLower;
    std::vector<ptrdiff_t> snapUpper;
    std::vector<ptrdiff_t> snapStep;

    const bool fixedBackground;

    T omegaEoS, H0, a0;

    const int spectraVerbosity;
    const double deltaKBin;
    const int nBinsSpectra;

    const bool hdf5Averages;
    const ptrdiff_t hdf5FlushFreq;
    const bool hdf5Spectra;

    const EvolverType eType;

    bool boolSaveEnd;
    bool boolBackup;

    bool appendMode;
    bool overwriteMode;

    std::string saveEndPath;
    std::string backupPath;

    const bool printHeaders;

    const int fnVerbosity;

    mutable bool doWeRestart;

    T tolerance; // For adaptative solvers only.

    const int powerSpectrumType;
    const int powerSpectrumVersion;

    const bool withGWs;
    const EvolverType eTypeGW;
    const int GWprojectorType;
    const bool flagON;
    const bool flagChiralPS;
    const bool unbinnedSpectra;

    LatticeParameters<T> getLatParams() { return LatticeParameters<T>(dx, lSide, kIR); }

    std::string extraInfoFn() const
    {
      std::string ret = "";
      if (fnVerbosity == 0 or fnVerbosity == 1) return ret;

      if (fnVerbosity > 1) ret += "N_" + std::to_string(N) + "_kIR_" + FloatToString::format(kIR) + "_";

      if (fnVerbosity > 2) ret += "dt_" + FloatToString::format(dt) + "_evolver_" + to_string(eType) + "_";

      return ret;
    }
  };

  template <typename R> auto createParams(int argc, char *argv[]) { return std::make_shared<R>(argc, argv); }
} // namespace TempLat

#endif
