#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONSINGLEQUANTITY_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONSINGLEQUANTITY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <Kokkos_Core.hpp>
#include <Kokkos_Macros.hpp>
#include <limits>
#include <algorithm>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionsingledatum.h"
#include "TempLat/parallel/mpi/comm/mpicommreference.h"

namespace TempLat
{
  MakeException(RadialProjectionSingleQuantityException);

  /** \brief A class which holds properties of a quantity (average, variance, min, max),
   *  each in a separate vector. Useful during the integration, will be transposed after
   *  the integration is done.
   *
   *
   * Unit test: make test-radialprojectionsinglequantity
   **/
  template <typename T> class RadialProjectionSingleQuantity
  {

  public:
    RadialProjectionSingleQuantity(ptrdiff_t size)
    {
      mAveragesDevice = DeviceView("RadialProjectionSingleQuantity::mAveragesDevice", size);
      mVariancesDevice = DeviceView("RadialProjectionSingleQuantity::mVariancesDevice", size);
      mMinsDevice = DeviceView("RadialProjectionSingleQuantity::mMinsDevice", size);
      Kokkos::deep_copy(mMinsDevice, std::numeric_limits<T>::max());
      mMaxsDevice = DeviceView("RadialProjectionSingleQuantity::mMaxsDevice", size);
      Kokkos::deep_copy(mMaxsDevice, -std::numeric_limits<T>::max());

      mAverages = Kokkos::create_mirror_view(mAveragesDevice);
      mVariances = Kokkos::create_mirror_view(mVariancesDevice);
      mMins = Kokkos::create_mirror_view(mMinsDevice);
      Kokkos::deep_copy(mMins, std::numeric_limits<T>::max());
      mMaxs = Kokkos::create_mirror_view(mMaxsDevice);
      Kokkos::deep_copy(mMaxs, -std::numeric_limits<T>::max());
    }

    size_t size() const { return mAverages.size(); }

    /** \brief Add one new weighted value to the collection of properties. */
    KOKKOS_FUNCTION
    void add_device(ptrdiff_t i, const T &value, const T &weight) const
    {
      checkBounds(i);
      Kokkos::atomic_add(&mAveragesDevice(i), weight * value);
      Kokkos::atomic_add(&mVariancesDevice(i), weight * value * value);
      Kokkos::atomic_min(&mMinsDevice(i), value);
      Kokkos::atomic_max(&mMaxsDevice(i), value);
    }

    void clear()
    {
      Kokkos::deep_copy(mAverages, 0);
      Kokkos::deep_copy(mVariances, 0);
      Kokkos::deep_copy(mMins, std::numeric_limits<T>::max());
      Kokkos::deep_copy(mMaxs, -std::numeric_limits<T>::max());
      push();
    }

    void pull()
    {
      Kokkos::deep_copy(mAverages, mAveragesDevice);
      Kokkos::deep_copy(mVariances, mVariancesDevice);
      Kokkos::deep_copy(mMins, mMinsDevice);
      Kokkos::deep_copy(mMaxs, mMaxsDevice);
    }

    void push()
    {
      Kokkos::deep_copy(mAveragesDevice, mAverages);
      Kokkos::deep_copy(mVariancesDevice, mVariances);
      Kokkos::deep_copy(mMinsDevice, mMins);
      Kokkos::deep_copy(mMaxsDevice, mMaxs);
    }

    /** \brief This is why we keep stuff in vectors, sum up all the results from all processes in an easy way: vectors
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

    /** \brief to be called only after finalize, returning the normalized result, this time transposed: all info per
     * bin, instead of all bins per info. */
    RadialProjectionSingleDatum<T> getFinal(ptrdiff_t i, const T &multiplicity)
    {
      checkBounds(i);
      return RadialProjectionSingleDatum<T>(mAverages[i], mVariances[i], mMins[i], mMaxs[i], multiplicity);
    }

    template <typename S> friend class RadialProjectionResult;

  private:
    using DeviceView = Kokkos::View<T *, DefaultLayout, Kokkos::DefaultExecutionSpace>;
    using HostMirror = typename DeviceView::HostMirror;

    DeviceView mAveragesDevice;
    DeviceView mVariancesDevice;
    DeviceView mMinsDevice;
    DeviceView mMaxsDevice;

    HostMirror mAverages;
    HostMirror mVariances;
    HostMirror mMins;
    HostMirror mMaxs;

    KOKKOS_FORCEINLINE_FUNCTION
    void checkBounds(ptrdiff_t i) const
    {
#ifdef CHECKBOUNDS
#ifdef NOKOKKOS
      if (i < 0 || i >= (ptrdiff_t)mAverages.size()) {
        throw RadialProjectionSingleQuantityException("Out of bounds: ", i, "not in", 0, " -- ", mAverages.size());
      }
#endif
#endif
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
