#ifndef TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTPLANNER_H
#define TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTPLANNER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#include "TempLat/parallel/device_memory.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/fft/external/kokkosfft/kokkosfftplanholder.h"
#include "TempLat/fft/external/kokkosfft/kokkosfftplanner.h"
#include "TempLat/lattice/memory/memoryblock.h"

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
    // Put public methods here. These should change very little over time.
    KokkosFFTPlanner() {}

    virtual void setPlannerPatience(int level) {}

    /** \brief Create fully working plans, which must self-destruct in the FFTPlanInterface's destructor. Use
     * shared_ptr's.
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, float>> getPlans_float(const MPICartesianGroup &group,
                                                                          const FFTLayoutStruct<NDim> &layout)
    {
      return make_plans<float>(group, layout);
    };

    /** \brief Create fully working plans, which must self-destruct in the FFTPlanInterface's destructor. Use
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

      complex<T> *dummy_f = nullptr;
      auto fourier_view = std::apply(
          [&](auto &&...args) { return device::memory::NDViewUnmanaged<NDim, complex<T>>(dummy_f, args...); },
          fourierSizes);
      T *dummy_c = nullptr;
      auto config_view = std::apply(
          [&](auto &&...args) { return device::memory::NDViewUnmanaged<NDim, T>(dummy_c, args...); }, configSizes);

      typename KokkosFFTPlanHolder<NDim, T>::PlanChain planChain;
      planChain.configSizes = configSizes;
      planChain.fourierSizes = fourierSizes;

      if constexpr (NDim >= 3) {
        for (size_t j = 0; j < NDim / 3; ++j) {
          using c2rType = typename KokkosFFTPlanHolder<NDim, T>::template PlanType_c2r<3>;
          using r2cType = typename KokkosFFTPlanHolder<NDim, T>::template PlanType_r2c<3>;

          auto axes = KokkosFFT::axis_type<3>{};
          for (size_t i = 0; i < 3; ++i)
            axes[2 - i] = -j * 3 - i - 1;

          auto plan_c2r = std::shared_ptr<c2rType>(new c2rType(Kokkos::DefaultExecutionSpace(), fourier_view,
                                                               config_view, KokkosFFT::Direction::backward, axes));
          auto plan_r2c = std::shared_ptr<r2cType>(new r2cType(Kokkos::DefaultExecutionSpace(), config_view,
                                                               fourier_view, KokkosFFT::Direction::forward, axes));
          planChain.c2rPlans_3D.push_back(plan_c2r);
          planChain.r2cPlans_3D.push_back(plan_r2c);
        }
      }

      if constexpr (NDim >= 2) {
        if (NDim % 3 == 2) {
          using c2rType = typename KokkosFFTPlanHolder<NDim, T>::template PlanType_c2r<2>;
          using r2cType = typename KokkosFFTPlanHolder<NDim, T>::template PlanType_r2c<2>;

          auto axes = KokkosFFT::axis_type<2>{};
          axes[1] = 1;
          axes[0] = 0;

          auto plan_c2r = std::shared_ptr<c2rType>(new c2rType(Kokkos::DefaultExecutionSpace(), fourier_view,
                                                               config_view, KokkosFFT::Direction::backward, axes));
          auto plan_r2c = std::shared_ptr<r2cType>(new r2cType(Kokkos::DefaultExecutionSpace(), config_view,
                                                               fourier_view, KokkosFFT::Direction::forward, axes));
          planChain.c2rPlans_2D.push_back(plan_c2r);
          planChain.r2cPlans_2D.push_back(plan_r2c);
        }
      }
      if (NDim % 3 == 1) {
        using c2rType = typename KokkosFFTPlanHolder<NDim, T>::template PlanType_c2r<1>;
        using r2cType = typename KokkosFFTPlanHolder<NDim, T>::template PlanType_r2c<1>;

        auto axes = KokkosFFT::axis_type<1>{};
        axes[0] = 0;

        auto plan_c2r = std::shared_ptr<c2rType>(new c2rType(Kokkos::DefaultExecutionSpace(), fourier_view, config_view,
                                                             KokkosFFT::Direction::backward, axes));
        auto plan_r2c = std::shared_ptr<r2cType>(new r2cType(Kokkos::DefaultExecutionSpace(), config_view, fourier_view,
                                                             KokkosFFT::Direction::forward, axes));
        planChain.c2rPlans_1D.push_back(plan_c2r);
        planChain.r2cPlans_1D.push_back(plan_r2c);
      }

      return std::make_shared<KokkosFFTPlanHolder<NDim, T>>(group, planChain);
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
