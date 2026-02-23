#ifndef TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTPLANNER_H
#define TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTPLANNER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#include "TempLat/parallel/device_memory.h"
#include "TempLat/util/exception.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/fft/external/kokkosfft/kokkosfftplanholder.h"
#include "TempLat/fft/external/kokkosfft/kokkosfftplanner.h"
#include "TempLat/lattice/memory/memoryblock.h"

namespace TempLat
{
  MakeException(KokkosFFTPlannerException);

  /** @brief A class which implements part of FFTLibraryInterface, the planner part. Here all calls to KokkosFFT_plan
   *... are made.
   *
   *
   * Unit test: ctest -R test-kokkosfftplanner
   **/
  template <size_t NDim> class KokkosFFTPlanner : public FFTLibraryInterface<NDim>
  {
  public:
    // Put public methods here. These should change very little over time.
    KokkosFFTPlanner() = default;

    virtual void setPlannerPatience(int level) {}

    /** @brief Create fully working plans, which must self-destruct in the FFTPlanInterface's destructor. Use
     * shared_ptr's.
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, float>> getPlans_float(const MPICartesianGroup &group,
                                                                          const FFTLayoutStruct<NDim> &layout)
    {
      return make_plans<float>(group, layout);
    };

    /** @brief Create fully working plans, which must self-destruct in the FFTPlanInterface's destructor. Use
     * shared_ptr's.
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, double>> getPlans_double(const MPICartesianGroup &group,
                                                                            const FFTLayoutStruct<NDim> &layout)
    {
      return make_plans<double>(group, layout);
    };

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    unsigned int patienceFlag;

    template <typename T>
    std::shared_ptr<KokkosFFTPlanHolder<NDim, T>> make_plans(const MPICartesianGroup &group,
                                                             const FFTLayoutStruct<NDim> &layout)
    {
      device::array<int, NDim> configStarts;
      device::array<int, NDim> fourierStarts;

      device::array<int, NDim> configSizes;
      device::array<int, NDim> fourierSizes;
      for (size_t i = 0; i < NDim; ++i) {
        configStarts[i] = layout.configurationSpace.getLocalStarts()[i];
        fourierStarts[i] = layout.fourierSpace.getLocalStarts()[i];

        configSizes[i] = layout.configurationSpace.getLocalSizes()[i];
        fourierSizes[i] = layout.fourierSpace.getLocalSizes()[i];
      }

      complex<T> *dummy_f = nullptr;
      auto fourier_view = device::apply(
          [&](auto &&...args) { return device::memory::NDViewUnmanaged<NDim, complex<T>>(dummy_f, args...); },
          fourierSizes);
      T *dummy_c = nullptr;
      auto config_view = device::apply(
          [&](auto &&...args) { return device::memory::NDViewUnmanaged<NDim, T>(dummy_c, args...); }, configSizes);

      typename KokkosFFTPlanHolder<NDim, T>::Plans plans;
      plans.configSizes = configSizes;
      plans.fourierSizes = fourierSizes;

      using c2rType = typename KokkosFFTPlanHolder<NDim, T>::PlanType_c2r;
      using r2cType = typename KokkosFFTPlanHolder<NDim, T>::PlanType_r2c;

      auto axes = KokkosFFT::axis_type<NDim>{};
      for (size_t i = 0; i < NDim; ++i)
        axes[i] = i;

      plans.c2rPlan = std::shared_ptr<c2rType>(new c2rType(Kokkos::DefaultExecutionSpace(), fourier_view, config_view,
                                                           KokkosFFT::Direction::backward, axes));
      plans.r2cPlan = std::shared_ptr<r2cType>(
          new r2cType(Kokkos::DefaultExecutionSpace(), config_view, fourier_view, KokkosFFT::Direction::forward, axes));

      return std::make_shared<KokkosFFTPlanHolder<NDim, T>>(group, plans);
    }
  };

} // namespace TempLat

#endif
