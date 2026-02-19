#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONSINGLEQUANTITY_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONSINGLEQUANTITY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <limits>
#include <algorithm>

#include "TempLat/util/exception.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionsingledatum.h"
#include "TempLat/parallel/mpi/comm/mpicommreference.h"

#include "TempLat/parallel/device_memory.h"

namespace TempLat
{
  MakeException(RadialProjectionSingleQuantityException);

  /** @brief A class which holds properties of a quantity (average, variance, min, max),
   *  each in a separate vector. Useful during the integration, will be transposed after
   *  the integration is done.
   *
   *
   * Unit test: ctest -R test-radialprojectionsinglequantity
   **/
  template <typename T> class RadialProjectionSingleQuantity
  {

  public:
    RadialProjectionSingleQuantity(ptrdiff_t size)
    {
      mAveragesDevice = DeviceView("RadialProjectionSingleQuantity::mAveragesDevice", size);
      mVariancesDevice = DeviceView("RadialProjectionSingleQuantity::mVariancesDevice", size);
      mMinsDevice = DeviceView("RadialProjectionSingleQuantity::mMinsDevice", size);
      mMaxsDevice = DeviceView("RadialProjectionSingleQuantity::mMaxsDevice", size);

      mAverages = device::memory::createMirrorView(mAveragesDevice);
      mVariances = device::memory::createMirrorView(mVariancesDevice);
      mMins = device::memory::createMirrorView(mMinsDevice);
      mMaxs = device::memory::createMirrorView(mMaxsDevice);

      clear();
    }

    size_t size() const { return mAverages.size(); }

    /** @brief Add one new weighted value to the collection of properties. */
    DEVICE_FUNCTION
    void add_device(ptrdiff_t i, const T &value, const T &weight) const
    {
      checkBounds(i);
      device::atomic_add(&mAveragesDevice(i), weight * value);
      device::atomic_add(&mVariancesDevice(i), weight * value * value);
      device::atomic_min(&mMinsDevice(i), value);
      device::atomic_max(&mMaxsDevice(i), value);
    }

    void clear()
    {
      device::memory::fill(mAverages, 0);
      device::memory::fill(mVariances, 0);
      device::memory::fill(mMins, std::numeric_limits<T>::max());
      device::memory::fill(mMaxs, -std::numeric_limits<T>::max());
      push();
    }

    void pull()
    {
      device::memory::copyDeviceToHost(mAveragesDevice, mAverages.data());
      device::memory::copyDeviceToHost(mVariancesDevice, mVariances.data());
      device::memory::copyDeviceToHost(mMinsDevice, mMins.data());
      device::memory::copyDeviceToHost(mMaxsDevice, mMaxs.data());
    }

    void push()
    {
      device::memory::copyHostToDevice(mAverages.data(), mAveragesDevice);
      device::memory::copyHostToDevice(mVariances.data(), mVariancesDevice);
      device::memory::copyHostToDevice(mMins.data(), mMinsDevice);
      device::memory::copyHostToDevice(mMaxs.data(), mMaxsDevice);
    }

    /** @brief This is why we keep stuff in vectors, sum up all the results from all processes in an easy way: vectors
     * of the same things and same size just add up. */
    void finalize(MPICommReference comm)
    {
      pull();
      /* reduce! */
      comm.Allreduce(mAverages, MPI_SUM);
      comm.Allreduce(mVariances, MPI_SUM);
      comm.Allreduce(mMins, MPI_MIN);
      comm.Allreduce(mMaxs, MPI_MAX);
    }

    /** @brief to be called only after finalize, returning the normalized result, this time transposed: all info per
     * bin, instead of all bins per info. */
    RadialProjectionSingleDatum<T> getFinal(ptrdiff_t i, const T &multiplicity)
    {
      checkBounds(i);
      return RadialProjectionSingleDatum<T>(mAverages[i], mVariances[i], mMins[i], mMaxs[i], multiplicity);
    }

    template <typename S> friend class RadialProjectionResult;

  private:
    using DeviceView = device::memory::NDView<1, T>;
    using HostMirror = typename DeviceView::host_mirror_type;

    DeviceView mAveragesDevice;
    DeviceView mVariancesDevice;
    DeviceView mMinsDevice;
    DeviceView mMaxsDevice;

    HostMirror mAverages;
    HostMirror mVariances;
    HostMirror mMins;
    HostMirror mMaxs;

    DEVICE_FORCEINLINE_FUNCTION
    void checkBounds(ptrdiff_t i) const
    {
#ifdef CHECKBOUNDS
#ifdef DEVICE_HAS_EXCEPTIONS
      if (i < 0 || i >= (ptrdiff_t)mAverages.size()) {
        throw RadialProjectionSingleQuantityException("Out of bounds: ", i, "not in", 0, " -- ", mAverages.size());
      }
#endif
#endif
    }
  };
} // namespace TempLat

#endif
