#ifndef TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTPLANNER_H
#define TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTPLANNER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/parallel/device_memory.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/fft/external/kokkosfft/kokkosfftplanholder.h"
#include "TempLat/fft/external/kokkosfft/kokkosfftplanner.h"
#include "TempLat/lattice/memory/memoryblock.h"
#include <Kokkos_Core_fwd.hpp>
#include <memory>

namespace TempLat
{
  MakeException(KokkosFFTPlannerException);

  /** \brief A class which implements part of FFTLibraryInterface, the planner part. Here all calls to KokkosFFT_plan
   *... are made.
   *
   *
   * Unit test: make test-kokkosfftplanner
   **/
  template <size_t NDim> class KokkosFFTPlanner : public FFTLibraryInterface<NDim>
  {
  public:
    /* Put public methods here. These should change very little over time. */
    KokkosFFTPlanner() {}

    virtual void setPlannerPatience(int level) {}

    /** \brief Create fully working plans, which must self-destruct in the FFTPlanInterface's destructor. Use
     * shared_ptr's.
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, float>> getPlans_float(const MPICartesianGroup &group,
                                                                          const FFTLayoutStruct<NDim> &layout)
    {
      return make_plan<float>(group, layout);
    };

    /** \brief Create fully working plans, which must self-destruct in the FFTPlanInterface's destructor. Use
     * shared_ptr's.
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, double>> getPlans_double(const MPICartesianGroup &group,
                                                                            const FFTLayoutStruct<NDim> &layout)
    {
      return std::make_shared<KokkosFFTPlanHolder<NDim, double>>(group, make_plan(group, layout));
    };

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    unsigned int patienceFlag;
    using PlanType = typename KokkosFFTPlanHolder<NDim, double>::PlanType;

    template <typename T>
    std::shared_ptr<KokkosFFTPlanHolder<NDim, T>> make_plans(const MPICartesianGroup &group,
                                                             const FFTLayoutStruct<NDim> &layout)
    {
      std::array<int, NDim> configStarts;
      std::array<int, NDim> fourierStarts;

      std::array<int, NDim> configSizes;
      std::array<int, NDim> fourierSizes;
      for (size_t i = 0; i < NDim; ++i) {
        configStarts[i] = layout.configurationSpace.getLocalStarts()[i];
        fourierStarts[i] = layout.fourierSpace.getLocalStarts()[i];

        configSizes[i] = layout.configurationSpace.getLocalSizes()[i];
        fourierSizes[i] = layout.fourierSpace.getLocalSizes()[i];
      }
      configSizes[NDim - 1] -= 2;

      std::cout << "KokkosFFT plan: configStarts = ";
      for (auto &&it : configStarts)
        std::cout << it << " ";
      std::cout << "\n";
      std::cout << "KokkosFFT plan: configSizes = ";
      for (auto &&it : configSizes)
        std::cout << it << " ";
      std::cout << "\n";
      std::cout << "KokkosFFT plan: fourierStarts = ";
      for (auto &&it : fourierStarts)
        std::cout << it << " ";
      std::cout << "\n";
      std::cout << "KokkosFFT plan: fourierSizes = ";
      for (auto &&it : fourierSizes)
        std::cout << it << " ";
      std::cout << std::endl;

      void *dummy = nullptr;
      auto fourier_view = std::apply(
          [](auto &&...args) { return device::memory::NDViewUnmanaged<NDim, std::complex<double>>(dummy, args...); },
          fourierSizes);
      auto config_view = std::apply(
          [](auto &&...args) { return device::memory::NDViewUnmanaged<NDim, double>(dummy, args...); }, configSizes);

      auto plan = std::shared_ptr(new KokkosFFT::Plan(Kokkos::DefaultExecutionSpace, fourier_view, config_view));

      return std::make_shared<KokkosFFTPlanHolder<NDim, T>>(group, plan_c2r, plan_r2c);
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
