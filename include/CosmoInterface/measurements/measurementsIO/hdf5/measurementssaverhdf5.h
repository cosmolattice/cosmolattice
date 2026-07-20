#ifndef COSMOINTERFACE_MEASUREMENTS_MEASUREMENTSIO_HDF5_MEASUREMENTSSAVERHDF5_H
#define COSMOINTERFACE_MEASUREMENTS_MEASUREMENTSIO_HDF5_MEASUREMENTSSAVERHDF5_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Year: 2025

#ifdef HAVE_HDF5
#include "CosmoInterface/measurements/measurementsIO/filesmanager.h"
#include "TempLat/lattice/IO/HDF5/helpers/hdf5timeseries.h"

namespace TempLat
{

  /** @brief A class which saves the averages in HDF5 format.
   *
   *
   * Unit test: ctest -R test-measurementssaverhdf5
   **/

  template <typename T> class MeasurementsSaverHDF5
  {
  public:
    template <size_t NDim>
    MeasurementsSaverHDF5(FilesManager<NDim> &fm, std::string fn, bool pAmIRoot, bool appendMode,
                          const std::vector<std::string> &pHeaders)
        : mHeaders(pHeaders), currentAv(0), flushCount(1), flushFreq(fm.getFlushFreq()), grpName(fn),
          fName(fm.getHDF5Fn()), amIRoot(pAmIRoot)
    {
      HDF5File file;
      file.open(fName, ReadWrite);
      auto group = file.createOrOpenGroup(grpName);
      for (auto h : mHeaders) {
        if (appendMode && H5Lexists(group, h.c_str(), H5P_DEFAULT) > 0) {
          averages.emplace_back(group.reopenDataset(h));
          averages.back().setOffset(averages.back().getSizes()[0]);
          averages.back().extend(fm.getNMeas());
        } else {
          averages.emplace_back(group.template createTimeSeries<T>(h, {0}, {4096}));
          averages.back().extend(fm.getNMeas());
        }
        averages.back().close();
      }
      group.close();
      file.close();
    }

    template <size_t NDim>
    MeasurementsSaverHDF5(FilesManager<NDim> &fm, const Field<T, NDim> &fld, bool pAmIRoot, bool appendMode,
                          const std::vector<std::string> &headers)
        : MeasurementsSaverHDF5(fm, fm.getCurredName(fld, false), pAmIRoot, appendMode, headers)
    {
    }

    void addAverage(const T &r)
    {
      averages[currentAv].push(r);
      if (currentAv == averages.size() - 1) {
        currentAv = 0;
      } else
        currentAv += 1;
    }

    void save(bool lastMeas)
    {
      if (lastMeas or flushCount % flushFreq == 0) {
        HDF5File file;
        file.open(fName, ReadWrite);
        auto group = file.getGroup(grpName);
        for (auto &av : averages) {
          av.reopen(group.reopenDataset(mHeaders[currentAv]));
          av.flush(amIRoot);
          av.close();
          currentAv += 1;
        }
        group.close();
        file.close();

        flushCount = 1;
        currentAv = 0;
      } else {
        flushCount += 1;
      }
    }

  private:
    std::vector<HDF5TimeSeries<T>> averages;
    std::vector<std::string> mHeaders;
    size_t currentAv, flushCount, flushFreq;
    std::string grpName, fName;
    bool amIRoot;
  };

} // namespace TempLat

#endif // HAVE_HDF5
#endif
