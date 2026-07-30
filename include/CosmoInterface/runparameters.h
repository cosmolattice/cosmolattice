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

  /** @brief Tolerance for comparing run parameters with each other or with a sentinel value.
   *
   * Deliberately independent of the model's FloatType. AlmostEqual defaults its epsilon to
   * sqrt(numeric_limits<T>::epsilon()), which is 1.5e-8 in double but 3.4e-4 in single precision:
   * with that default, a single-precision run would accept lSide = 2*pi/kIR = N*dx agreeing only in
   * the fourth digit, and would treat any H0 below 3.4e-4 as exactly zero. These are host-side
   * comparisons of input parameters, so there is no reason to relax them with the field precision.
   **/
  static constexpr double runParameterTolerance = 1e-8;

  /** @brief A class which contains the parameters useful to run a simulation but not model specific (dt for example).
   *
   *
   **/
  template <typename T = double> class RunParameters
  {
  public:
    // Put public methods here. These should change very little over time.

    // Reads the "snapshots" input parameter, falling back to the deprecated "energy_snapshot"
    // alias if "snapshots" was not specified. If both are specified, "snapshots" takes precedence.
    static std::vector<std::string> getSnapshotLabels(ParameterParser &par)
    {
      const bool hasOld = par.getParams().count("energy_snapshot") > 0;
      const bool hasNew = par.getParams().count("snapshots") > 0;
      if (hasOld && hasNew)
        say << "WARNING: both 'snapshots' and the deprecated 'energy_snapshot' parameters were specified; "
               "'snapshots' takes precedence and 'energy_snapshot' is ignored.";
      else if (hasOld)
        say << "WARNING: the input parameter 'energy_snapshot' is deprecated, please use 'snapshots' instead. "
               "Its value is used to fill 'snapshots'.";
      if (hasOld && !hasNew)
        return par.get<std::string, 15>("energy_snapshot", std::vector<std::string>(15, Constants::defaultString));
      return par.get<std::string, 15>("snapshots", std::vector<std::string>(15, Constants::defaultString));
    }

    // List of run parameters and their default values:
    RunParameters(ParameterParser &par)
        : N(par.getOverride<ptrdiff_t>("N", Important)),     // Number of lattice points per dimension
          kIR(par.getOverride<T>("kIR", -1, Important)),     // IR cutoff
          lSide(par.getOverride<T>("lSide", -1, Important)), // Side length
          dt(par.get<T>("dt", Important)),                   // Time step
          dx(par.getOverride<T>("dx", -1, Important)),       // Lattice spacing
          expansion(par.get<bool>("expansion", true,
                                  Important)),             // If true: self-consistent expansion. If false: no expansion
          t0(par.get<T>("t0", 0, Important)),         // Initial time
          tMax(par.get<T>("tMax", 10000 * dt, Important)), // Final time
          kCutoff(par.get<T>("kCutOff", -1.0,
                             Important)), // Momenta cutoff in spectra of initial fluctuations. -1 means no cutoff.
          SIC(par.get<InitialConditionsType::S>("ICtype_S", InitialConditionsType::S::Default)),
          U1IC(par.get<InitialConditionsType::U1>("ICtype_U1", InitialConditionsType::U1::Default)),
          tOutFreq(par.get<T>("tOutputFreq", 10 * dt, Important)),      // Printing time interval of frequent output
          tOutInfreq(par.get<T>("tOutputInfreq", 100 * dt, Important)), // Printing time interval of infrequent output
          tOutRareFreq(par.get<T>("tOutputRareFreq", 1000 * dt, Important)), // Printing time interval of rare output
          tOutVerb(par.get<T>("tOutputVerb", 100 * dt)),                     // Output verbosity
          tBackupFreqFloat(par.get<T>("tBackupFreq", -1)),                   // Frequency of backups
          baseSeed(par.getSeed("baseSeed")),                 // Seed for random generator of initial fluctuations
          outFn(par.get<std::string>("outputfile", "./")()), // Folder where output is saved
          energySnapshotMeas(getSnapshotLabels(par)),        // Energy terms for which snapshots are printed
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
                                   : 0.0),                   // For fixed background expansion: equation of state
          H0(fixedBackground ? par.get<T>("H0", 0.0) : 0.0), // For fixed background expansion: initial Hubble parameter
          a0(par.get<T>("a0", 1.0)),                         // Initial scale factor (default 1.0)
          spectraVerbosity(par.get<int>("spectraVerbosity", 0)), // Verbosity of spectra files
          deltaKBin(par.get<double>("deltaKBin", 1)),            // Bin width of the spectra
          nBinsSpectra(floor(sqrt(3.0) / 2.0 * N / deltaKBin)),  // Number of bins in spectra
          hdf5Averages(par.get<bool>("hdf5Averages", false)), hdf5FlushFreq(par.get<ptrdiff_t>("hdf5FlushFreq", 10)),
          hdf5Spectra(par.get<bool>(
              "hdf5Spectra", false)), // If true, spectra are printed in HDF5 format. If false, printed in txt format.
          eType(par.get<EvolverType>("evolver", LF, Important)), // Type of evolution algorithm
          appendMode(par.get<bool>("appendToFiles",
                                   false)), // If true, output is appended to pre-existing files. If false, existing
                                            // files cause an error unless overwriteFiles=true.
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
          GWprojectorType(par.get<int>("GWprojectorType",
                                       2)), // Type of GWprojector (real = 1, backwards = 2 (default), forward = 3)
          flagON(par.get<bool>("flagON", false)), flagChiralPS(par.get<bool>("flagChiralPS", false)),
          unbinnedSpectra(par.get<bool>("saveUnbinnedSpectra", false)),
          lcorr((SIC == InitialConditionsType::S::DefectsNetwork || U1IC == InitialConditionsType::U1::DefectsNetwork)
                    ? par.get<T>("lcorr")
                    : 0.0),
          deltaNoise((SIC == InitialConditionsType::S::DefectsWhiteNoise ||
                      U1IC == InitialConditionsType::U1::DefectsWhiteNoise)
                         ? par.get<T>("deltaNoise")
                         : 0.0),
          doDiffusion(par.get<bool>("doDiffusion", false)), tmaxdiff(doDiffusion ? par.get<T>("tmaxdiff") : 0.0),
          dtdiff(doDiffusion ? par.get<T>("dtdiff") : 0.0),
          diffType(doDiffusion ? par.get<EvolverType>("diffusionevolver", RK2) : RK2), // Type of evolution algorithm
          tOutFreqDiff(doDiffusion ? par.get<T>("tOutputFreqDiff", 10 * dtdiff)
                                   : tMax), // Printing time interval of output during the diffusion phase
          tOutRareFreqDiff(doDiffusion ? par.get<T>("tOutputRareFreqDiff", 100 * dtdiff)
                                       : tMax), // Printing time interval of output during the diffusion phase
          energySnapshotMeasDiffusion(
              doDiffusion ? par.get<std::string, 15>("snapshots_diffusion",
                                                     std::vector<std::string>(15, Constants::defaultString))
                          : std::vector<std::string>(15, Constants::defaultString)),
          doResolutionPreserving(fixedBackground
                                     ? par.get<bool>("doResolutionPreserving", false)
                                     : false), // Whether a resolution-preserving techniques is performed or not. It
                                               // requires fixed background expansion (using it for self.consistent
                                               // expansion is yet not tested nor thoroughly thought)
          sRP(doResolutionPreserving ? par.get<T>("sRP") : 1.0),
          tRP0(doResolutionPreserving ? par.get<T>("tRP0", t0) : t0),
          tRPMax(doResolutionPreserving ? par.get<T>("tRPMax", gettRPMaxtening()) : t0),
          measureDefectsEnergies(par.get<bool>("measureDefectsEnergies", false)),
          measureDefectsStructure(par.get<bool>("measureDefectsStructure", false))
    {
      // The size of the lattice can be specified with any of kIR, lSide or dx. At least one of them is
      // needed, and any subset which is given must obey lSide = 2*pi/kIR = N*dx.
      // NOTE: We must have kIR=2*pi/lside for consistency.
      {
        const bool hasKIR = !AlmostEqual(kIR, -1, runParameterTolerance);
        const bool hasLSide = !AlmostEqual(lSide, -1, runParameterTolerance);
        const bool hasDx = !AlmostEqual(dx, -1, runParameterTolerance);

        if (!hasKIR && !hasLSide && !hasDx)
          throw(RunParametersMissing("You need to specify either kIR, lSide or dx, abort."));

        // Deduce the side length from whichever of the three was provided; the ones given but not used
        // here are checked for consistency right after.
        if (!hasLSide) lSide = hasKIR ? 2 * Constants::pi<T> / kIR : dx * N;

        if (hasKIR && !AlmostEqual(kIR, 2 * Constants::pi<T> / lSide, runParameterTolerance))
          throw(RunParametersInconsistent(
              "kIR = " + std::to_string(kIR) + " and lSide = " + std::to_string(lSide) +
              " are not consistent. If you think they should be, try removing one of the two."));
        if (hasDx && !AlmostEqual(dx, lSide / N, runParameterTolerance))
          throw(RunParametersInconsistent(
              "dx = " + std::to_string(dx) + " and lSide = " + std::to_string(lSide) + " (with N = " +
              std::to_string(N) + ") are not consistent. If you think they should be, try removing one of the two."));

        kIR = 2 * Constants::pi<T> / lSide;
        dx = lSide / N;

        // To not have wrong info in the parameters of the parser.
        if (!hasKIR) par.erase("kIR");
        if (!hasLSide) par.erase("lSide");
        if (!hasDx) par.erase("dx");
      }

      if (appendMode && overwriteMode)
        throw(RunParametersInconsistent("'appendToFiles' and 'overwriteFiles' cannot both be true. Pick one."));

      if (AlmostEqual(dt, 0., runParameterTolerance)) throw(RunParametersInconsistent("dt cannot be zero, abort."));
      if (tMax < t0) throw(RunParametersInconsistent("tMax must be >= t0, abort."));
      if (tOutFreq < dt) throw(RunParametersInconsistent("tOutputFreq must be >= dt, abort."));
      if (tOutInfreq < dt) throw(RunParametersInconsistent("tOutputInfreq must be >= dt, abort."));
      if (tOutRareFreq < dt) throw(RunParametersInconsistent("tOutputRareFreq must be >= dt, abort."));
      if (tOutVerb < dt) throw(RunParametersInconsistent("tOutputVerb must be >= dt, abort."));

      T kUV = std::sqrt(3) * Constants::pi<T> / dx; // Maximum momenta in the lattice

      if (kCutoff < 0.0) kCutoff = 2 * kUV; // no cutoff means it's larger than kUV.

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

      if (powerSpectrumType == 0 and unbinnedSpectra == false)
        throw(RunParametersInconsistent("powerSpectrumType 0 is only implemented for the unbinned power spectrum."));
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

    T gettRPMaxtening()
    {
      if (sRP > 0.0)
        throw(RunParametersInconsistent(
            "The parameter sRP of the resolution-preserving phase must obey sRP <= 0. Aborting."));
      if (AlmostEqual(sRP, 0., runParameterTolerance)) return tMax;
      return tRP0 * pow(tMax / tRP0, -1. / (sRP - 1.)); // This is correct for scale factors that evolve as a power law
                                                        // and which have a0 ~ (t/t0)^p !
    }

  public:
    const int N;
    T kIR;
    T lSide;
    T dt;
    T dx;

    const bool expansion;

    const T t0;
    const T tMax;
    T kCutoff;
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

    const T lcorr;
    const T deltaNoise;

    const bool doDiffusion;
    const T tmaxdiff;
    const T dtdiff;
    const EvolverType diffType;
    const T tOutFreqDiff;
    const T tOutRareFreqDiff;
    std::vector<std::string> energySnapshotMeasDiffusion;

    const bool doResolutionPreserving;
    const T sRP;
    const T tRP0;
    const T tRPMax;

    bool measureDefectsEnergies;
    bool measureDefectsStructure;

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
