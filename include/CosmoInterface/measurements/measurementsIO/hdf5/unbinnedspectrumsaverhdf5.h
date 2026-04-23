#ifndef COSMOINTERFACE_MEASUREMENTS_MEASUREMENTSIO_HDF5_UNBINNEDSPECTRUMSAVERHDF5_H
#define COSMOINTERFACE_MEASUREMENTS_MEASUREMENTSIO_HDF5_UNBINNEDSPECTRUMSAVERHDF5_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Jorge Baeza-Ballesteros,  Year: 2026
#ifdef HAVE_HDF5
#include "TempLat/util/prettytostring.h"
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "CosmoInterface/runparameters.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5file.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5timeseries.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/unbinnedradialprojectionresult.h"

namespace TempLat
{
  /** @brief A class which save the unbinned spectrum (or any UnbinnedRadialProjectionResults) in a hdf5 format.
   *
   *
   * Unit test: ctest -R test-unbinnedspectrumsaverhdf5
   **/
  template <typename T> class UnbinnedSpectrumSaverHDF5
  {
  public:
    template <size_t NDim>
    UnbinnedSpectrumSaverHDF5(FilesManager<NDim> &fm, const std::string &fn, bool pAmIRoot, bool append,
                      const RunParameters<T> &rPar)
        : filename(fm.getHDF5UnbinnedSpectraFn()), verbosity(rPar.spectraVerbosity), nBins(rPar.nBinsSpectra),
          deltaKBin(rPar.deltaKBin), nGrid(rPar.N), kIR(rPar.kIR), uninitialized(true), grpName(fn), amIRoot(pAmIRoot),
          appendMode(append), nMeas(fm.getNInfreqMeas()), flushCount(0), flushFreq(fm.getFlushFreq())
    {
    }

    template <size_t NDim>
    UnbinnedSpectrumSaverHDF5(FilesManager<NDim> &fm, const Field<T, NDim> &fld, bool pAmIRoot, bool append,
                      const RunParameters<T> &rPar)
        : UnbinnedSpectrumSaverHDF5(fm, fm.getCurredName(fld, false, "spectra"), pAmIRoot, append, rPar)
    {
    }

    virtual ~UnbinnedSpectrumSaverHDF5() = default;

    void save(bool lastMeas, std::vector<std::shared_ptr<UnbinnedRadialProjectionResult<T>>> arr, T t)
    {
      if (uninitialized) initialize(arr);

      push_spectra(arr, t);
      if (lastMeas or flushCount % flushFreq == 0) {
        HDF5File file;
        file.open(filename, ReadWrite);
        auto group = file.getGroup(grpName);
        flush_spectra(group);
        group.close();
        file.close();
        flushCount = 0;
      } else {
        flushCount += 1;
      }
    }

  private:
    void flush_single_spectra(std::shared_ptr<HDF5TimeSeries<T>> ptr, std::string name, HDF5Group &group)
    {
      ptr->reopen(group.reopenDataset(name));
      ptr->flush(amIRoot);
      ptr->close();
    }

    void initialize(std::vector<std::shared_ptr<UnbinnedRadialProjectionResult<T>>> arr)
    {
      size_t klength = arr.back()->size();
      HDF5File file;
      file.open(filename, ReadWrite);

      auto group = file.createOrOpenGroup(grpName);
      std::vector<hsize_t> dims{0, klength};
      std::vector<hsize_t> chunks{64, klength};

      auto openOrCreate = [&](const std::string &name) {
        std::shared_ptr<HDF5TimeSeries<T>> ts;
        if (appendMode && H5Lexists(group, name.c_str(), H5P_DEFAULT) > 0) {
          ts = std::make_shared<HDF5TimeSeries<T>>(group.reopenDataset(name));
          ts->setOffset(ts->getSizes()[0]);
        } else {
          ts = std::make_shared<HDF5TimeSeries<T>>(group.template createTimeSeries<T>(name, dims, chunks));
        }
        ts->extend(nMeas);
        ts->close();
        return ts;
      };

      multData = openOrCreate("momMultiplicity");
      binAvData = openOrCreate("momBinAverage");
      for (size_t i = 0; i < arr.size(); ++i)
        valueAvData.emplace_back(openOrCreate("spectAverage_" + std::to_string(i)));

      if (verbosity == 2) {
        throw(NotImplementedInHDF5("Verbosity 2 is for the spectrum is not supported currently in hdf5. Easy to fix if "
                                   "you need it and want to have it work."));
      }

      uninitialized = false;
      group.close();
      file.close();
    }

    void flush_spectra(HDF5Group &group)
    {
      flush_single_spectra(multData, "momMultiplicity", group);
      flush_single_spectra(binAvData, "momBinAverage", group);

      for (size_t i = 0; i < valueAvData.size(); ++i) {
        flush_single_spectra(valueAvData[i], "spectAverage_" + std::to_string(i), group);
      }
    }

    void push_spectra(std::vector<std::shared_ptr<UnbinnedRadialProjectionResult<T>>> arr, T t)
    {
      if (nBins > -1) {
        for (size_t i = 0; i < arr.size(); ++i) {
          arr[i]->rescaleBins(kIR);
        }
      }

      std::vector<T> mult;
      for (auto &&it : (*arr[0])) {
         mult.emplace_back(std::get<1>(it).multiplicity * 2);
      }
      multData->push(mult);

      std::vector<T> binAv;
      for (auto &&it : (*arr[0])) {
        binAv.emplace_back(std::get<0>(it));
      }
      binAvData->push(binAv);

      std::vector<T> valAv;
      for (size_t i = 0; i < arr.size(); ++i) {
        valAv.clear();
        for (auto &&it : (*arr[i])) {
          valAv.emplace_back(std::get<1>(it).average);
        }
        valueAvData[i]->push(valAv);
      }
    }

    std::string filename;
    int verbosity;
    int nBins;
    int deltaKBin;
    T nGrid, kIR;

    bool uninitialized;
    std::string grpName;
    bool amIRoot;
    bool appendMode;
    ptrdiff_t nMeas;
    size_t flushCount;
    size_t flushFreq;

    std::shared_ptr<HDF5TimeSeries<T>> multData;
    std::shared_ptr<HDF5TimeSeries<T>> binAvData;
    std::vector<std::shared_ptr<HDF5TimeSeries<T>>> valueAvData;
  };

} // namespace TempLat

#endif
#endif
