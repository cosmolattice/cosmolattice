#ifndef TEMPLAT_FFT_EXTERNAL_PFFT_PFFTPLANHOLDER_H
#define TEMPLAT_FFT_EXTERNAL_PFFT_PFFTPLANHOLDER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#ifndef NOFFT
#ifndef NOMPI
#ifndef NOPPFT
#include "pfft.h"
#endif
#endif
#endif

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"

namespace TempLat
{
  /** @brief A class which implements part of FFTLibraryInterface::PlanInterface, holding the pfft plans in shared_ptr's
   *- only freed when the last instance of this class is destructed.
   *
   * Unit test: ctest -R test-pfftplanholder
   **/
  template <size_t NDim, typename T> class PFFTPlanHolder : public FFTPlanInterface<NDim, T>
  {
  public:
#ifndef NOFFTFLOAT
    using plan = typename std::conditional_t<std::is_same_v<float, T>, pfftf_plan, pfft_plan>;
#else
    using plan = pfft_plan;
#endif
    // Put public methods here. These should change very little over time.
    PFFTPlanHolder(MPICartesianGroup group, plan planR2C, plan planC2R)
        : mGroup(group), /* need a reference to the group, to make sure the group is alive as long as the plans are
                            alive. */
          mPlanR2C(std::make_shared<plan>(planR2C)), mPlanC2R(std::make_shared<plan>(planC2R))
    {
    }

    virtual ~PFFTPlanHolder()
    {
      if (mPlanR2C.use_count() < 2) {
        say << "PFFTPlanHolder is correctly being destructed.\n";
        destroy(*mPlanR2C);
        destroy(*mPlanC2R);
      }
    }

    virtual void c2r(MemoryBlock<NDim, T> &mBlock) { execute_c2r(*mPlanC2R, mBlock); };
    virtual void r2c(MemoryBlock<NDim, T> &mBlock) { execute_r2c(*mPlanR2C, mBlock); };

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    MPICartesianGroup mGroup;
    std::shared_ptr<plan> mPlanR2C, mPlanC2R;

    template <typename S = T>
      requires std::is_same_v<S, double>
    void destroy(plan somePlan)
    {
      pfft_destroy_plan(somePlan);
    }

#ifndef NOFFTFLOAT
    template <typename S = T>
      requires std::is_same_v<S, float>
    void destroy(plan somePlan)
    {
      pfftf_destroy_plan(somePlan);
    }
#endif
    template <typename S = T>
      requires std::is_same_v<S, double>
    void execute_r2c(plan somePlan, MemoryBlock<NDim, S> &mBlock)
    {
      auto block_view = mBlock.getRawHostView();
      pfft_execute_dft_r2c(somePlan, block_view.data(), (pfft_complex *)mBlock.data());
      mBlock.pushHostView(); // make sure the data is pushed to the device
    }

#ifndef NOFFTFLOAT
    template <typename S = T>
      requires std::is_same_v<S, float>
    void execute_r2c(plan somePlan, MemoryBlock<NDim, S> &mBlock)
    {
      auto block_view = mBlock.getRawHostView();
      pfftf_execute_dft_r2c(somePlan, block_view.data(), (pfftf_complex *)block_view.data());
      mBlock.pushHostView(); // make sure the data is pushed to the device
    }
#endif

    template <typename S = T>
      requires std::is_same_v<S, double>
    void execute_c2r(plan somePlan, MemoryBlock<NDim, S> &mBlock)
    {
      auto block_view = mBlock.getRawHostView();
      pfft_execute_dft_c2r(somePlan, (pfft_complex *)block_view.data(), block_view.data());
      mBlock.pushHostView(); // make sure the data is pushed to the device
    }

#ifndef NOFFTFLOAT
    template <typename S = T>
      requires std::is_same_v<S, float>
    void execute_c2r(plan somePlan, MemoryBlock<NDim, S> &mBlock)
    {
      auto block_view = mBlock.getRawHostView();
      pfftf_execute_dft_c2r(somePlan, (pfftf_complex *)block_view.data(), block_view.data());
      mBlock.pushHostView(); // make sure the data is pushed to the device
    }
#endif

#ifdef TEMPLATTEST
  public:
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
