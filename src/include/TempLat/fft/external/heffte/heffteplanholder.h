#ifndef TEMPLAT_FFT_EXTERNAL_HEFFTE_HEFFTEPLANHOLDER_H
#define TEMPLAT_FFT_EXTERNAL_HEFFTE_HEFFTEPLANHOLDER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#ifndef NOFFT
#ifndef NOMPI
#ifndef NOHEFFTE
#include <heffte.h>
// Use the default_backend trait with the tag::gpu for the location
namespace TempLat
{
  using heffte_backend_tag = heffte::backend::default_backend<heffte::tag::gpu>::type;
}
#endif
#endif
#endif

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"

namespace TempLat
{
  /** \brief A class which implements part of FFTLibraryInterface::PlanInterface, holding the heffte plans in
   *shared_ptr's
   *- only freed when the last instance of this class is destructed.
   *
   * Unit test: make test-heffteplanholder
   **/

  template <size_t NDim, typename T> class HEFFTEPlanHolder : public FFTPlanInterface<NDim, T>
  {
  public:
    // typedef heffte_plan plan;
    using PlanType = heffte::fft3d_r2c<heffte_backend_tag, int>;

    /* Put public methods here. These should change very little over time. */
    HEFFTEPlanHolder(MPICartesianGroup group, std::shared_ptr<PlanType> plan) : mGroup(group), mPlan(plan) {}

    virtual ~HEFFTEPlanHolder() {}

    virtual void c2r(MemoryBlock<NDim, T> &mBlock) { execute_c2r(mPlan, mBlock); };
    virtual void r2c(MemoryBlock<NDim, T> &mBlock) { execute_r2c(mPlan, mBlock); };

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    MPICartesianGroup mGroup;
    std::shared_ptr<PlanType> mPlan;

    void execute_r2c(std::shared_ptr<PlanType> fft, MemoryBlock<NDim, T> &mBlock)
    {
      device::iteration::fence();
      fft->forward((T *)mBlock.data(), (std::complex<T> *)mBlock.data(), heffte::scale::none);
    }

    void execute_c2r(std::shared_ptr<PlanType> fft, MemoryBlock<NDim, T> &mBlock)
    {
      device::iteration::fence();
      fft->backward((std::complex<T> *)mBlock.data(), (T *)mBlock.data(), heffte::scale::full);
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
