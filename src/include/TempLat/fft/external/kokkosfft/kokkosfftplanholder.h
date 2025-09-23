#ifndef TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTPLANHOLDER_H
#define TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTPLANHOLDER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#ifndef NOFFT
#ifdef KOKKOS_FFT
#include <KokkosFFT.hpp>
#endif
#endif

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"

#include "TempLat/parallel/device_iteration.h"

namespace TempLat
{
  /** \brief A class which implements part of FFTLibraryInterface::PlanInterface, holding the KokkosFFT plans in
   *shared_ptr's
   *- only freed when the last instance of this class is destructed.
   *
   * Unit test: make test-kokkosfftplanholder
   **/

  template <size_t NDim, typename T> class KokkosFFTPlanHolder : public FFTPlanInterface<NDim, T>
  {
  public:
    using PlanType = void;

    /* Put public methods here. These should change very little over time. */
    KokkosFFTPlanHolder(MPICartesianGroup group, std::shared_ptr<PlanType> plan) : mGroup(group), mPlan(plan) {}

    virtual ~KokkosFFTPlanHolder() {}

    virtual void c2r(MemoryBlock<NDim, T> &mBlock) { execute_c2r(mPlan, mBlock); };
    virtual void r2c(MemoryBlock<NDim, T> &mBlock) { execute_r2c(mPlan, mBlock); };

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    MPICartesianGroup mGroup;
    std::shared_ptr<PlanType> mPlan;

    void execute_r2c(std::shared_ptr<PlanType> fft, MemoryBlock<NDim, T> &mBlock)
    {
      device::iteration::fence();
      // fft->forward((T *)mBlock.data(), (std::complex<T> *)mBlock.data(), heffte::scale::none);
    }

    void execute_c2r(std::shared_ptr<PlanType> fft, MemoryBlock<NDim, T> &mBlock)
    {
      device::iteration::fence();
      // fft->backward((std::complex<T> *)mBlock.data(), (T *)mBlock.data(), heffte::scale::full);
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
