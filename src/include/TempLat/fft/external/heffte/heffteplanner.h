#ifndef TEMPLAT_FFT_EXTERNAL_HEFFTE_HEFFTEPLANNER_H
#define TEMPLAT_FFT_EXTERNAL_HEFFTE_HEFFTEPLANNER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/fft/external/heffte/heffteplanholder.h"
#include "TempLat/fft/external/heffte/hefftetranspositionflags.h"
#include "TempLat/lattice/memory/memoryblock.h"

namespace TempLat
{
  MakeException(HEFFTEPlannerException);

  /** \brief A class which implements part of FFTLibraryInterface, the planner part. Here all calls to HEFFTE_plan ...
   *are made.
   *
   *
   * Unit test: make test-heffteplanner
   **/
  template <size_t NDim> class HEFFTEPlanner : public FFTLibraryInterface<NDim>
  {
  public:
    /* Put public methods here. These should change very little over time. */
    HEFFTEPlanner() {}

    virtual void setPlannerPatience(int level) {}

    /** \brief Create fully working plans, which must self-destruct in the FFTPlanInterface's destructor. Use
     * shared_ptr's.
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, float>> getPlans_float(const MPICartesianGroup &group,
                                                                          const FFTLayoutStruct<NDim> &layout)
    {
      return std::make_shared<HEFFTEPlanHolder<NDim, float>>(group);
    };

    /** \brief Create fully working plans, which must self-destruct in the FFTPlanInterface's destructor. Use
     * shared_ptr's.
     */
    /** \brief Create fully working plans, which must self-destruct in the FFTPlanInterface's destructor. Use
     * shared_ptr's.
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, double>> getPlans_double(const MPICartesianGroup &group,
                                                                            const FFTLayoutStruct<NDim> &layout)
    {
      return std::make_shared<HEFFTEPlanHolder<NDim, double>>(group);
    };

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    unsigned int patienceFlag;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
