#ifndef TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTPLANHOLDER_H
#define TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTPLANHOLDER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#ifndef NOFFT
#ifdef KOKKOSFFT
#include <KokkosFFT.hpp>
#endif
#endif

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"

#include "TempLat/parallel/device_iteration.h"

namespace TempLat
{
  /** @brief A class which implements part of FFTLibraryInterface::PlanInterface, holding the KokkosFFT plans in
   *shared_ptr's
   *- only freed when the last instance of this class is destructed.
   *
   * Unit test: ctest -R test-kokkosfftplanholder
   **/
  template <size_t NDim, typename T> class KokkosFFTPlanHolder : public FFTPlanInterface<NDim, T>
  {
  public:
    // Put public methods here. These should change very little over time.

    template <size_t _NDim = NDim>
    using PlanType_c2r =
        typename KokkosFFT::Plan<Kokkos::DefaultExecutionSpace, device::memory::NDViewUnmanaged<NDim, complex<T>>,
                                 device::memory::NDViewUnmanaged<NDim, T>, _NDim>;
    template <size_t _NDim = NDim>
    using PlanType_r2c =
        typename KokkosFFT::Plan<Kokkos::DefaultExecutionSpace, device::memory::NDViewUnmanaged<NDim, T>,
                                 device::memory::NDViewUnmanaged<NDim, complex<T>>, _NDim>;

    /**
     * @brief What's the intention here? Well, KokkosFFT does not support multi-dimensional FFTs directly, but only 1D,
     * 2D and 3D FFTs. So for higher dimensions, we need to chain multiple FFTs together. This struct holds the plans
     * for these FFTs, and provides methods to execute them in sequence.
     *
     * However.
     *
     * We currently do not use this, as KokkosFFT does not support in-place transpositions. Therefore, we just fill a
     * 1D, 2D or 3D plan and call that. I'm leaving this here for future reference, in case KokkosFFT adds in-place
     * transposition or I (or someone else) implement it themselves.
     *
     */
    struct PlanChain {
      std::vector<std::shared_ptr<PlanType_c2r<3>>> c2rPlans_3D;
      std::vector<std::shared_ptr<PlanType_r2c<3>>> r2cPlans_3D;
      std::vector<std::shared_ptr<PlanType_c2r<2>>> c2rPlans_2D;
      std::vector<std::shared_ptr<PlanType_r2c<2>>> r2cPlans_2D;
      std::vector<std::shared_ptr<PlanType_c2r<1>>> c2rPlans_1D;
      std::vector<std::shared_ptr<PlanType_r2c<1>>> r2cPlans_1D;

      void execute_c2r(const auto &src, const auto &dest)
      {
        for (const auto &plan : c2rPlans_3D) {
          KokkosFFT::execute(*plan, src, dest, KokkosFFT::Normalization::none);
        }
        for (const auto &plan : c2rPlans_2D) {
          KokkosFFT::execute(*plan, src, dest, KokkosFFT::Normalization::none);
        }
        for (const auto &plan : c2rPlans_1D) {
          KokkosFFT::execute(*plan, src, dest, KokkosFFT::Normalization::none);
        }
      }

      void execute_r2c(const auto &src, const auto &dest)
      {
        for (const auto &plan : r2cPlans_3D) {
          KokkosFFT::execute(*plan, src, dest, KokkosFFT::Normalization::none);
        }
        for (const auto &plan : r2cPlans_2D) {
          KokkosFFT::execute(*plan, src, dest, KokkosFFT::Normalization::none);
        }
        for (const auto &plan : r2cPlans_1D) {
          KokkosFFT::execute(*plan, src, dest, KokkosFFT::Normalization::none);
        }
      }

      device::array<int, NDim> configSizes;
      device::array<int, NDim> fourierSizes;
    };

    KokkosFFTPlanHolder(MPICartesianGroup group, const PlanChain &planChain) : mGroup(group), mPlanChain(planChain) {}

    virtual ~KokkosFFTPlanHolder() {}

    virtual void c2r(MemoryBlock<NDim, T> &mBlock) { execute_c2r(mBlock); };
    virtual void r2c(MemoryBlock<NDim, T> &mBlock) { execute_r2c(mBlock); };

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    MPICartesianGroup mGroup;
    PlanChain mPlanChain;

    void execute_r2c(MemoryBlock<NDim, T> &mBlock)
    {
      device::iteration::fence();
      auto fourier_view = device::apply(
          [&](auto &&...args) {
            return device::memory::NDViewUnmanaged<NDim, complex<T>>(reinterpret_cast<complex<T> *>(mBlock.data()),
                                                                     args...);
          },
          mPlanChain.fourierSizes);
      auto config_view = device::apply(
          [&](auto &&...args) { return device::memory::NDViewUnmanaged<NDim, T>(mBlock.data(), args...); },
          mPlanChain.configSizes);

      mPlanChain.execute_r2c(config_view, fourier_view);
      device::iteration::fence();
    }

    void execute_c2r(MemoryBlock<NDim, T> &mBlock)
    {
      device::iteration::fence();
      auto fourier_view = device::apply(
          [&](auto &&...args) {
            return device::memory::NDViewUnmanaged<NDim, complex<T>>(reinterpret_cast<complex<T> *>(mBlock.data()),
                                                                     args...);
          },
          mPlanChain.fourierSizes);
      auto config_view = device::apply(
          [&](auto &&...args) { return device::memory::NDViewUnmanaged<NDim, T>(mBlock.data(), args...); },
          mPlanChain.configSizes);

      mPlanChain.execute_c2r(fourier_view, config_view);
      device::iteration::fence();
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
