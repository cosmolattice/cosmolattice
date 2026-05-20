#ifndef COSMOINTERFACE_MEASUREMENTS_STD_SPECTRUMSAVERSTD_H
#define COSMOINTERFACE_MEASUREMENTS_STD_SPECTRUMSAVERSTD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020
//            Modified: Jorge Baeza-Ballesteros,  Year: 2026

#include "TempLat/util/conditionaloutput/outputstream.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionresult.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/field/field.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "CosmoInterface/runparameters.h"
#include <filesystem>

namespace TempLat
{
  /** @brief A class which implements the std spectra output.
   *
   *
   * Unit test: ctest -R test-spectrumsaverstd
   **/
  template <typename T> class SpectrumSaverStd
  {
  public:
    template <size_t NDim>
    SpectrumSaverStd(FilesManager<NDim> &fm, std::string fn, bool amIRoot, bool append, const RunParameters<T> &rPar)
        : mMode(!append ? std::ios_base::out : std::ios_base::app),
          outputSpectrum(makeOutputStream(fm.getWorkingDir() + "spectra_" + fn + ".txt", amIRoot, append,
                                          fm.getOverwriteMode())),
          verbosity(rPar.spectraVerbosity), nBins(rPar.nBinsSpectra), deltaKBin(rPar.deltaKBin), nGrid(rPar.N),
          kIR(rPar.kIR), printHeader(fm.getPrintHeaders())
    {
    }

    virtual ~SpectrumSaverStd() = default;

    template <size_t NDim>
    SpectrumSaverStd(FilesManager<NDim> &fm, const Field<T, NDim> &fld, bool amIRoot, bool append,
                     const RunParameters<T> &rPar)
        : mMode(!append ? std::ios_base::out : std::ios_base::app), verbosity(rPar.spectraVerbosity),
          nBins(rPar.nBinsSpectra), deltaKBin(rPar.deltaKBin), nGrid(rPar.N), kIR(rPar.kIR),
          printHeader(fm.getPrintHeaders())
    {
      std::string name = fld.toString();
      name = name.erase(name.find("(", 3));
      name = fm.getWorkingDir() + "spectra_" + name;
      outputSpectrum = makeOutputStream(name + ".txt", amIRoot, append, fm.getOverwriteMode());
    }

    void save(std::vector<std::shared_ptr<RadialProjectionResult<T>>> arr, T t)
    {
      if (printHeader) {
        if (verbosity != 1) (*outputSpectrum) << "1:binCentralValue" << " ";
        if (verbosity != 0) (*outputSpectrum) << (*arr[0])[0].getHeaderBin(verbosity, verbosity == 1 ? 1 : 2) << " ";
        for (size_t i = 0; i < arr.size() - 1; ++i)
          (*outputSpectrum) << (*arr[i])[0].getHeaderValue(verbosity, false, verbosity == 2 ? 4 * (i + 1) + 2 : i + 2)
                            << " ";
        (*outputSpectrum) << (*(arr.back()))[0].getHeaderValue(verbosity, true,
                                                               verbosity == 2 ? 4 * (arr.size()) + 2 : (arr.size() + 1))
                          << "\n";

        printHeader = false;
      }

      for (size_t i = 0; i < arr.size(); ++i) {
        arr[i]->rescaleBins(kIR);
      }

      // Now we construct the file with the appropriate format.

      std::stringstream sstream;
      for (size_t i = 0; i < (*arr[0]).getNBins(); ++i) {
        sstream.str("");

        if (verbosity == 0) {
          sstream << (*arr[0]).getCentralBinBounds()[i];
        } else if (verbosity == 1) {
          sstream << (*arr[0])[i].getBinString(verbosity);
        } else if (verbosity == 2) {
          sstream << (*arr[0]).getCentralBinBounds()[i] << " " << (*arr[0])[i].getBinString(verbosity);
        }

        sstream << " " << (*arr[0])[i].getValueString(arr.size() <= 1, verbosity, true);
        for (size_t j = 1; j < arr.size() - 1; ++j) {
          sstream << " " << (*arr[j])[i].getValueString(false, verbosity, true);
        }
        if (arr.size() > 1) sstream << " " << (*arr.back())[i].getValueString(true, verbosity, true);
        (*outputSpectrum) << sstream.str() << "\n";

      }
      (*outputSpectrum)  << "\n";
      outputSpectrum->flush();
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    std::shared_ptr<OutputStream<T>> makeOutputStream(const std::string &fname, bool amIRoot, bool appendMode,
                                                      bool overwriteMode)
    {
      if (!appendMode && std::filesystem::exists(fname) && amIRoot) {
        if (overwriteMode) {
          std::filesystem::remove(fname);
        } else {
          throw(FileAlreadyExistsError(
              "Refusing to overwrite existing output file \"" + fname +
              "\". Set 'appendToFiles = true' to append, or 'overwriteFiles = true' to delete."));
        }
      }
      return std::make_shared<OutputStream<T>>(fname, amIRoot, mMode);
    }

    std::ios_base::openmode mMode;
    std::shared_ptr<OutputStream<T>> outputSpectrum;
    int verbosity;
    int nBins;
    int deltaKBin;
    T nGrid, kIR;
    bool printHeader;
  };
} // namespace TempLat

#endif
