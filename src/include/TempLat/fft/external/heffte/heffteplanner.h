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
#include <memory>

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
    static_assert(NDim <= 3, "HEFFTE only works in 1, 2 or 3 dimensions.");

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
      return std::make_shared<HEFFTEPlanHolder<NDim, float>>(group, make_plan(group, layout));
    };

    /** \brief Create fully working plans, which must self-destruct in the FFTPlanInterface's destructor. Use
     * shared_ptr's.
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, double>> getPlans_double(const MPICartesianGroup &group,
                                                                            const FFTLayoutStruct<NDim> &layout)
    {
      return std::make_shared<HEFFTEPlanHolder<NDim, double>>(group, make_plan(group, layout));
    };

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    unsigned int patienceFlag;
    using PlanType = typename HEFFTEPlanHolder<NDim, double>::PlanType;

    std::shared_ptr<PlanType> make_plan(const MPICartesianGroup &group, const FFTLayoutStruct<NDim> &layout)
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

      // local box and outbox, i.e. shapes of the local data in memory
      heffte::box3d<int> inbox(configStarts, configSizes);
      heffte::box3d<int> outbox(fourierStarts, fourierSizes);

      // which direction holds only the conjugate pairs
      constexpr int r2c_direction = NDim - 1;

      // MPI group communcation
      MPI_Comm comm = group.getComm();

      // plan options
      heffte::plan_options options = heffte::default_options<heffte_backend_tag>();

      return std::make_shared<PlanType>(inbox, outbox, r2c_direction, comm, options);
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
