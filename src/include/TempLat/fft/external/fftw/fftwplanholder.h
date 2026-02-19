#ifndef TEMPLAT_FFT_EXTERNAL_FFTW_FFTWPLANHOLDER_H
#define TEMPLAT_FFT_EXTERNAL_FFTW_FFTWPLANHOLDER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <type_traits>

#ifndef NOFFT
#ifdef HAVE_MPI
#include "fftw3-mpi.h"
#endif
#endif

#ifndef NOFFT
#include "fftw3.h"
#endif

#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"

namespace TempLat
{

  /** @brief A class which holds FFT plans, and implements FFTLibraryInterface::PlanInterface<T>.
   * The plans are implemented in std::shared_ptr's, so they are only desctructed when the last instance is destructed.
   *
   * Unit test: ctest -R test-fftwplanholder
   **/
  template <size_t NDim, typename T> class FFTWPlanHolder : public FFTPlanInterface<NDim, T>
  {
  public:
#ifdef HAVE_FFTFLOAT
    using plan = typename std::conditional_t<std::is_same_v<float, T>, fftwf_plan, fftw_plan>;
#else
    using plan = fftw_plan;
#endif
    // Put public methods here. These should change very little over time.
    FFTWPlanHolder(MPICartesianGroup group, plan planR2C, plan planC2R)
        : mGroup(group), /* need a reference to the group, to make sure the group is alive as long as the plans are
                            alive. */
          mPlanR2C(std::make_shared<plan>(planR2C)), mPlanC2R(std::make_shared<plan>(planC2R))
    {
    }

    ~FFTWPlanHolder()
    {
      if (mPlanR2C.use_count() < 2) {
        // say << "FFTWPlanHolder is correctly being destructed.\n";
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
      fftw_destroy_plan(somePlan);
    }

#ifdef HAVE_FFTFLOAT
    template <typename S = T>
      requires std::is_same_v<S, float>
    void destroy(plan somePlan)
    {
      fftwf_destroy_plan(somePlan);
    }
#endif
    template <typename S = T>
      requires std::is_same_v<S, double>
    void execute_r2c(plan somePlan, MemoryBlock<NDim, S> &mBlock)
    {
      auto block_view = mBlock.getRawHostView();
      fftw_execute_dft_r2c(somePlan, block_view.data(), (fftw_complex *)block_view.data());
      mBlock.pushHostView(); // make sure the data is pushed to the device
    }

#ifdef HAVE_FFTFLOAT
    template <typename S = T>
      requires std::is_same_v<S, float>
    void execute_r2c(plan somePlan, MemoryBlock<NDim, S> &mBlock)
    {
      auto block_view = mBlock.getRawHostView();
      fftwf_execute_dft_r2c(somePlan, block_view.data(), (fftwf_complex *)block_view.data());
      mBlock.pushHostView(); // make sure the data is pushed to the device
    }
#endif

    template <typename S = T>
      requires std::is_same_v<S, double>
    void execute_c2r(plan somePlan, MemoryBlock<NDim, S> &mBlock)
    {
      // sayMPI << "FFTW double c2r starting. Plan: " << somePlan << "\n";
      // sayMPI << "with block size: " << mBlock.size() << "\n";
      auto block_view = mBlock.getRawHostView();
      fftw_execute_dft_c2r(somePlan, (fftw_complex *)block_view.data(), block_view.data());
      // sayMPI << "FFTW double c2r done.\n";
      mBlock.pushHostView(); // make sure the data is pushed to the device
    }

#ifdef HAVE_FFTFLOAT
    template <typename S = T>
      requires std::is_same_v<S, float>
    void execute_c2r(plan somePlan, MemoryBlock<NDim, S> &mBlock)
    {
      auto block_view = mBlock.getRawHostView();
      fftwf_execute_dft_c2r(somePlan, (fftwf_complex *)block_view.data(), block_view.data());
      mBlock.pushHostView(); // make sure the data is pushed to the device
    }
#endif
  };

} // namespace TempLat

#endif
