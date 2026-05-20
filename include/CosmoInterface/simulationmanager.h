#ifndef COSMOINTERFACE_SIMULATIONMANAGER_H
#define COSMOINTERFACE_SIMULATIONMANAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include <filesystem>

#include "CosmoInterface/definitions/averages.h"
#include "TempLat/util/cstyletime.h"
#include "TempLat/lattice/IO/fileio.h"
#include "CosmoInterface/abstractmodel.h"
#include "CosmoInterface/runparameters.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "TempLat/util/conditionaloutput/conditionalfilestream.h"
#include "TempLat/parallel/threadsettings.h"

namespace TempLat
{
  /** @brief A class which handles start and stop of a simulation. Also used to create the info file.
   *
   *
   **/
  template <size_t NDim> class SimulationManager
  {
  public:
    // Put public methods here. These should change very little over time.

    // This is where we decide whether the simulation is running in restart mode or not. We start by
    // reading the "load_file" argument. If the user provides one, then it means they want to restart the simulation.
    SimulationManager(ParameterParser &parser)
        : restart(parser.get<std::string>(
              "load_file",
              parser.get<std::string>(
                  "load_dir", Constants::defaultString)())) // NOTE: Added compatibility with "load_dir", to make it
                                                            // consistent with the typo in the first manual. Still, the
                                                            // full name of the file that wants to be leaded is needed.
    {
      boolRestart = restart != Constants::defaultString;

      // If we are not restarting, we simply delete "load_file" from the parameter parser.
      if (restart == Constants::defaultString) parser.erase("load_file");
      // We set the parser in "override mode", this mechanisms allows the user to specify new parameters over the old
      // saved ones.
      if (boolRestart) parser.activateOverride();
    }

    // TODO: A potential test to do is to save the averages in the h5 as well and compute them upon loading
    // and crash if they are different.

    // In restart mode, we start by loading the former parameters in the paameter parser.
    void getParams(ParameterParser &parser)
    {
#ifdef HAVE_HDF5
      fIO.loader.open(restart);
      fIO.loader.load(parser);
      fIO.loader.close();
#else
      throw(
          FileIOException("You tried to load parameters from a file, but the HDF5 library is not available. Make sure "
                          "you have it installed and that you compiled CosmoLattice with it."));
#endif
    }

    // Function which loads from file all the field variables when running in restart mode.
    template <typename T, class Model> void loadSim(Model &model, T &t0)
    {
#ifdef HAVE_HDF5
      fIO.loader.open(restart);

      ForLoop(i, 0, Model::Ns - 1, fIO.load(model.fldS(i)); fIO.load(model.piS(i)););

      if (model.fldGWs != nullptr) {
        ForLoop(i, 0, 4, fIO.load((*model.fldGWs)(i)); fIO.load((*model.piGWs)(i)););
      }

      ForLoop(i, 0, Model::NCs - 1, fIO.load(model.fldCS(i)); fIO.load(model.piCS(i)););

      ForLoop(i, 0, Model::NSU2Doublet - 1, fIO.load(model.fldSU2Doublet(i)); fIO.load(model.piSU2Doublet(i)););

      ForLoop(i, 0, Model::NU1 - 1, fIO.load(model.fldU1(i)); fIO.load(model.piU1(i)););

      ForLoop(i, 0, Model::NSU2 - 1, fIO.load(model.fldSU2(i)); fIO.load(model.piSU2(i)););

      fIO.loader.load(model.aSI, "aSI");
      fIO.loader.load(model.aI, "aI");
      fIO.loader.load(model.aIM, "aIM");
      fIO.loader.load(model.aDotSI, "aDotSI");
      fIO.loader.load(model.aDotI, "aDotI");
      fIO.loader.load(model.aDotSIM, "aDotSIM");
      fIO.loader.load(t0, "tSave");
      fIO.loader.close();

      Averages::setAllAverages(model);
#else
      throw(FileIOException(
          "You tried to load a simulation from a file, but the HDF5 library is not available. Make sure "
          "you have it installed and that you compiled CosmoLattice with it."));
#endif
    }

    // Function which saves all field variables to disk when asked.
    template <typename T, class Model> void saveSim(ParameterParser &parser, Model &model, T time, std::string tag = "")
    {
#ifdef HAVE_HDF5
      fIO.saver.create(tag);
      ForLoop(i, 0, Model::Ns - 1, fIO.save(model.fldS(i)); fIO.save(model.piS(i)););

      if (model.fldGWs != nullptr) {
        ForLoop(i, 0, 4, fIO.save((*model.fldGWs)(i)); fIO.save((*model.piGWs)(i)););
      }

      ForLoop(i, 0, Model::NCs - 1, fIO.save(model.fldCS(i)); fIO.save(model.piCS(i)););

      ForLoop(i, 0, Model::NSU2Doublet - 1, fIO.save(model.fldSU2Doublet(i)); fIO.save(model.piSU2Doublet(i)););

      ForLoop(i, 0, Model::NU1 - 1, fIO.save(model.fldU1(i)); fIO.save(model.piU1(i)););

      ForLoop(i, 0, Model::NSU2 - 1, fIO.save(model.fldSU2(i)); fIO.save(model.piSU2(i)););

      fIO.saver.save(model.aSI, "aSI");
      fIO.saver.save(model.aI, "aI");
      fIO.saver.save(model.aIM, "aIM");
      fIO.saver.save(model.aDotSI, "aDotSI");
      fIO.saver.save(model.aDotI, "aDotI");
      fIO.saver.save(model.aDotSIM, "aDotSIM");

      fIO.saver.save(time, "tSave");

      fIO.saver.save(parser);
      fIO.saver.close();
#else
      throw(
          FileIOException("You tried to save a simulation to a file, but the HDF5 library is not available. Make sure "
                          "you have it installed and that you compiled CosmoLattice with it."));
#endif
    }

    // Function which performs the automatic backup of the simulation.
    template <typename T, class Model> void backup(ParameterParser &parser, Model &model, T time, std::string folder)
    {
      std::string oldStr(folder + model.name + "_backup.h5");
      std::string newStr(folder + model.name + "_backup.h5~");
      rename(oldStr.c_str(), newStr.c_str()); // Keep the last backup in case something goes wrong after.
      saveSim(parser, model, time, oldStr);
    }

    // Tells the outside part of the program wheter we are in restart mode or not.
    bool doWeRestart() const { return boolRestart; }

    // Create an info file, with all the simulation parameters as well as the parallelization grid the simulation
    // was run on.
    template <typename T, class Model>
    void createInfoFile(ParameterParser &par, const RunParameters<T> &runPar, Model &model, const std::vector<int> &dec,
                        bool amIRoot)
    {
      const std::string fname = base_filename(runPar, model, true, true) + ".infos";
      if (!runPar.appendMode && std::filesystem::exists(fname) && amIRoot) {
        if (runPar.overwriteMode) std::filesystem::remove(fname);
        else throw(FileAlreadyExistsError(
            "Refusing to overwrite existing output file \"" + fname +
            "\". Set 'appendToFiles = true' to append, or 'overwriteFiles = true' to delete."));
      }
      info = std::make_unique<ConditionalFileStream>(
          fname, amIRoot, runPar.appendMode ? std::ios_base::app : std::ios_base::out);
      (*info) << "Parameters: \n" << par << "\n";
      CStyleTime mt;
      mt.now();
      if (!boolRestart)
        (*info) << "The simulation started on the " << mt.date(" ") << " around " << mt.approxTime("") << ".\n";
      else
        (*info) << "The simulation RE-started on the " << mt.date(" ") << " around " << mt.approxTime("") << ".\n";
      (*info) << "It will be running on an MPI grid of (";
      for (size_t i = 0; i < dec.size() - 1; ++i)
        (*info) << dec[i] << ",";
      (*info) << dec.back() << ") cores with " << ThreadSettings::getMaxThreadCount() << " threads per MPI task.\n";
      (*info) << "Memory usage is estimated to be around " << estimateMemoryUsageString(model) << " per MPI task.\n";
      info->flush();
    }

    // Finalize the info file by printing some execution time information.
    void closeInfoFile(int nprocesses)
    {
      CStyleTime mt;
      mt.now();
      (*info) << "The simulation finished on the " << mt.date(" ") << " around " << mt.approxTime("") << ".\n";
      long long int ms = TimeSpent::getProcessAge();

      auto ss = ms / 1000;
      auto msDec = ms % 1000;
      auto nthreads = nprocesses * ThreadSettings::getMaxThreadCount();

      char msBuf[6];
      snprintf(msBuf, 6, "%03lli", msDec);
      (*info) << "The timer indicates that it ran for " << ss << "." << msBuf << "s.\n";
      (*info) << "As it ran on " << nthreads << " cores, this corresponds to " << ms / 1000.0 / 3600.0 * nthreads
              << " core hours.\n";

#ifndef DEVICE_CPU
      (*info) << "As it ran on GPUs, this corresponds to " << ms / 1000.0 / 3600.0 * nprocesses << " GPU hours.\n";
#endif
    }

    template <typename T, class Model>
    static std::string base_filename(const RunParameters<T> &par, Model &model, bool removeLastChar = false,
                                     bool forceModelVerbosity = false)
    {
      auto str = par.outFn + model.extraInfoFn(forceModelVerbosity ? 1 : par.fnVerbosity) + par.extraInfoFn();
      if (removeLastChar) str.pop_back();
      return str;
    }

  private:
    // Put all member variables and private methods here. These may change arbitrarily.

    // Manager to load and save fields to disk.
    FileIO<NDim> fIO;
    std::string restart;
    bool boolRestart;
    // Info file.
    std::unique_ptr<ConditionalFileStream> info;

    template <typename Model> std::string estimateMemoryUsageString(const Model &model) const
    {
      size_t totalBytes = 0;

      // Scalar fields (phi and pi for each scalar field)
      if constexpr (Model::Ns > 0) totalBytes += 2 * Model::Ns * model.fldS(0_c).getMemoryManager()->bytes();

      // Complex scalar fields (phi and pi for each complex scalar field)
      if constexpr (Model::NCs > 0)
        totalBytes += 2 * Model::NCs * 2 * model.fldCS(0_c)(0_c).getMemoryManager()->bytes();

      // U(1) gauge fields (Each component of the gauge field, and their conjugate momenta)
      if constexpr (Model::NU1 > 0)
        totalBytes += 2 * Model::NU1 * NDim * model.fldU1(0_c)(1_c).getMemoryManager()->bytes();

      // SU(2) gauge fields (Each component of the gauge field, and their conjugate momenta)
      if constexpr (Model::NSU2 > 0)
        totalBytes += 2 * Model::NSU2 * NDim * 4 * model.fldSU2(0_c)(1_c)(1_c).getMemoryManager()->bytes();

      // SU(2) doublet fields (phi and pi for each component of the doublet)
      if constexpr (Model::NSU2Doublet > 0)
        totalBytes += 2 * Model::NSU2Doublet * 4 * model.fldSU2Doublet(0_c)(0_c).getMemoryManager()->bytes();

      // Gravitational waves (if present)
      if (model.fldGWs != nullptr) {
        totalBytes += 2 * 5 * (*model.fldGWs)(0_c).getMemoryManager()->bytes();
      }

      // Convert to human-readable format
      const char *suffixes[] = {"B", "KB", "MB", "GB", "TB"};
      size_t suffixIndex = 0;
      double readableSize = static_cast<double>(totalBytes);
      while (readableSize >= 1024.0 && suffixIndex < 4) {
        readableSize /= 1024.0;
        ++suffixIndex;
      }
      std::ostringstream oss;
      oss << std::fixed << std::setprecision(2) << readableSize << " " << suffixes[suffixIndex];
      return oss.str();
    }
  };
} // namespace TempLat

#endif
