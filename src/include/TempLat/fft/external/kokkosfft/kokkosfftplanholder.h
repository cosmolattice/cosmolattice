#ifndef TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTPLANHOLDER_H
#define TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTPLANHOLDER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#ifndef NOFFT
#ifdef HAVE_KOKKOSFFT
#include <KokkosFFT.hpp>
#endif
#endif

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

    using PlanType_c2r =
        typename KokkosFFT::Plan<Kokkos::DefaultExecutionSpace, device::memory::NDViewUnmanaged<NDim, complex<T>>,
                                 device::memory::NDViewUnmanaged<NDim, T>, NDim>;
    using PlanType_r2c =
        typename KokkosFFT::Plan<Kokkos::DefaultExecutionSpace, device::memory::NDViewUnmanaged<NDim, T>,
                                 device::memory::NDViewUnmanaged<NDim, complex<T>>, NDim>;

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
    struct Plans {
      std::shared_ptr<PlanType_c2r> c2rPlan;
      std::shared_ptr<PlanType_r2c> r2cPlan;

      void execute_c2r(const auto &src, const auto &dest)
      {
        KokkosFFT::execute(*c2rPlan, src, dest, KokkosFFT::Normalization::none);
      }

      void execute_r2c(const auto &src, const auto &dest)
      {
        KokkosFFT::execute(*r2cPlan, src, dest, KokkosFFT::Normalization::none);
      }

      device::array<int, NDim> configSizes;
      device::array<int, NDim> fourierSizes;
    };

    KokkosFFTPlanHolder(MPICartesianGroup group, const Plans &plans) : mGroup(group), mPlans(plans) {}

    virtual ~KokkosFFTPlanHolder() = default;

    virtual void c2r(MemoryBlock<NDim, T> &mBlock) { execute_c2r(mBlock); };
    virtual void r2c(MemoryBlock<NDim, T> &mBlock) { execute_r2c(mBlock); };

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    MPICartesianGroup mGroup;
    Plans mPlans;

    void execute_r2c(MemoryBlock<NDim, T> &mBlock)
    {
      device::iteration::fence();
      auto fourier_view = device::apply(
          [&](auto &&...args) {
            return device::memory::NDViewUnmanaged<NDim, complex<T>>(reinterpret_cast<complex<T> *>(mBlock.data()),
                                                                     args...);
          },
          mPlans.fourierSizes);
      auto config_view = device::apply(
          [&](auto &&...args) { return device::memory::NDViewUnmanaged<NDim, T>(mBlock.data(), args...); },
          mPlans.configSizes);

      mPlans.execute_r2c(config_view, fourier_view);
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
          mPlans.fourierSizes);
      auto config_view = device::apply(
          [&](auto &&...args) { return device::memory::NDViewUnmanaged<NDim, T>(mBlock.data(), args...); },
          mPlans.configSizes);

      mPlans.execute_c2r(fourier_view, config_view);
      device::iteration::fence();
    }
  };

} // namespace TempLat

#endif
