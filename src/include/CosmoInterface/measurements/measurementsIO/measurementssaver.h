#ifndef COSMOINTERFACE_MEASUREMENTSSAVER_H
#define COSMOINTERFACE_MEASUREMENTSSAVER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "TempLat/util/conditionaloutput/outputstream.h"
#include <sstream>
#include "CosmoInterface/measurements/measurementsIO/std/measurementssaverstd.h"
#include "CosmoInterface/measurements/measurementsIO/hdf5/measurementssaverhdf5.h"

namespace TempLat
{
  /** @brief A class which implements an interface to the measurement IO. Allow to switch between different format.
   *
   * Unit test: ctest -R test-measurementssaver
   **/
  template <typename T> class MeasurementsSaver
  {
  public:
    template <size_t NDim>
    MeasurementsSaver(FilesManager<NDim> &fm, std::string fn, bool amIRoot, bool appendMode,
                      const std::vector<std::string> &headers = {}, bool dontCreate = false)
        : printHeaders(fm.getPrintHeaders()), useHDF5(fm.getUseHDF5())
    {
      if (!dontCreate) {
        if (!useHDF5)
          ms = std::make_shared<MeasurementsSaverStd<T>>(fm, fn, amIRoot, appendMode, headers);
        else
          ms5 = std::make_shared<MeasurementsSaverHDF5<T>>(fm, fn, amIRoot, appendMode, headers);
        IExist = true;
      } else
        IExist = false;
    }

    template <size_t NDim>
    MeasurementsSaver(FilesManager<NDim> &fm, const Field<NDim, T> &fld, bool amIRoot, bool appendMode,
                      const std::vector<std::string> &headers = {}, bool dontCreate = false)
        : printHeaders(fm.getPrintHeaders()), useHDF5(fm.getUseHDF5())
    {
      if (!dontCreate) {
        if (!useHDF5)
          ms = std::make_shared<MeasurementsSaverStd<T>>(fm, fld, amIRoot, appendMode, headers);
        else
          ms5 = std::make_shared<MeasurementsSaverHDF5<T>>(fm, fld, amIRoot, appendMode, headers);
        IExist = true;
      } else
        IExist = false;
    }

    void addAverage(const T &r)
    {
      if (IExist) {
        if (!useHDF5)
          ms->addAverage(r);
        else
          ms5->addAverage(r);
      }
    }

    void save(bool lastMeas = false)
    {
      if (IExist) {
        if (!useHDF5)
          ms->save();
        else
          ms5->save(lastMeas);
      }
    }

  private:
    std::shared_ptr<MeasurementsSaverStd<T>> ms;
    std::shared_ptr<MeasurementsSaverHDF5<T>> ms5;
    bool IExist;
    bool printHeaders;
    bool useHDF5;
  };

} // namespace TempLat

#endif
