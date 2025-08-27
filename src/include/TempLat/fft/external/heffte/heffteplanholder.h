#ifndef TEMPLAT_FFT_EXTERNAL_HEFFTE_HEFFTEPLANHOLDER_H
#define TEMPLAT_FFT_EXTERNAL_HEFFTE_HEFFTEPLANHOLDER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#ifndef NOFFT
#ifndef NOMPI
#ifndef NOPPFT
#include <heffte.h>
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

    /* Put public methods here. These should change very little over time. */
    HEFFTEPlanHolder(MPICartesianGroup group) : mGroup(group) //,
    /* need a reference to the group, to make sure the group is alive as long as the plans are alive. */
    // mPlanR2C(std::make_shared<plan>(planR2C)), mPlanC2R(std::make_shared<plan>(planC2R))
    {
    }

    virtual ~HEFFTEPlanHolder() {}

    virtual void c2r(MemoryBlock<NDim, T> &mBlock) {
      // execute_c2r(*mPlanC2R, mBlock);
    };
    virtual void r2c(MemoryBlock<NDim, T> &mBlock) {
      // execute_r2c(*mPlanR2C, mBlock);
    };

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    MPICartesianGroup mGroup;
    // std::shared_ptr<plan> mPlanR2C, mPlanC2R;
    /*
        template <typename S = T>
          requires std::is_same_v<S, double>
        void execute_r2c(plan somePlan, MemoryBlock<S> &mBlock)
        {
          heffte_execute_dft_r2c(somePlan, mBlock.ptr(), (heffte_complex *)mBlock.ptr());
        }

        template <typename S = T>
          requires std::is_same_v<S, double>
        void execute_c2r(plan somePlan, MemoryBlock<S> &mBlock)
        {
          // sayMPI << "HEFFTE double c2r starting.\n";
          heffte_execute_dft_c2r(somePlan, (heffte_complex *)mBlock.ptr(), mBlock.ptr());
          // sayMPI << "HEFFTE double c2r done.\n";
        }
        */

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
