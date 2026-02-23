#ifndef TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5TIMESERIES_H
#define TEMPLAT_LATTICE_IO_HDF5_HELPERS_HDF5TIMESERIES_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Year: 2025

#ifdef HAVE_HDF5

#include "TempLat/lattice/IO/HDF5/helpers/hdf5dataset.h"
#include "TempLat/util/flattenstd.h"

namespace TempLat
{

  /** @brief A class which implement an easy interface to save timeseries.
   *
   *
   * Unit test: ctest -R test-hdf5timeseries
   **/

  template <typename T> class HDF5TimeSeries : public HDF5Dataset
  {
  public:
    HDF5TimeSeries() : HDF5Dataset(), count(0), curOffset(0) {}

    HDF5TimeSeries(const hid_t &id) : HDF5Dataset(id), count(0), curOffset(0) {}

    HDF5TimeSeries(hid_t &&id) : HDF5Dataset(std::move(id)), count(0), curOffset(0) {}

    void push(const T &elem)
    {
      cache.emplace_back(elem);
      ++count;
    }

    template <typename U> void push(const std::vector<U> &elem)
    {
      std::vector<T> flat = flatten(elem);
      cache.insert(cache.end(), flat.begin(), flat.end());
      count += 1;
    }

    void flush(bool amIRoot)
    {
      std::vector<hsize_t> curSizes = getSizes();
      std::vector<hsize_t> subDims(curSizes);
      std::vector<hsize_t> offsets(curSizes.size());

      subDims[0] = count;
      offsets[0] = curOffset;

      writeSlices(cache, subDims, offsets, amIRoot);
      cache.clear();
      curOffset += count;
      count = 0;
    }

    void extend(hsize_t extra)
    {
      std::vector<hsize_t> curSizes = getSizes();
      std::vector<hsize_t> newSizes(curSizes);
      newSizes[0] += extra;

      H5Dset_extent(mId, newSizes.data());
    }

  private:
    std::vector<T> cache;
    hsize_t count, curOffset;
  };

} // namespace TempLat

#endif

#endif
