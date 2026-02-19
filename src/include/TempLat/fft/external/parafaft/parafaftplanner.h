#ifndef TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTPLANNER_H
#define TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTPLANNER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2026

#ifndef NOFFT
#ifdef HAVE_MPI
#ifdef HAVE_PARAFAFT
#include <parafaft_r2c.hpp>
#endif
#endif
#endif

#include "TempLat/util/exception.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/fft/external/parafaft/parafaftplanholder.h"
#include "TempLat/lattice/memory/memoryblock.h"

#include <memory>

namespace TempLat
{
  MakeException(ParafaftPlannerException);

  /** @brief Plan creation for parafaft FFT transforms.
   *
   * Creates parafaft::ParaFaFT_R2C objects wrapped in ParafaftPlanHolder.
   * Currently only supports 3D and double precision.
   *
   * Unit test: ctest -R test-parafaftplanner
   **/
  template <size_t NDim> class ParafaftPlanner : public FFTLibraryInterface<NDim>
  {
  public:
    ParafaftPlanner() {}

    /**
     * @brief Set planner patience level.
     *
     * Parafaft uses FFTW internally with FFTW_ESTIMATE, so this is a no-op.
     * The parameter is accepted for interface compatibility.
     */
    virtual void setPlannerPatience(int level) override
    {
      // Parafaft uses FFTW_ESTIMATE internally - no patience setting
      (void)level;
    }

    /**
     * @brief Create float-precision plans.
     *
     * Not supported - parafaft only supports double precision.
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, float>> getPlans_float(const MPICartesianGroup &group,
                                                                          const FFTLayoutStruct<NDim> &layout) override
    {
#ifndef HAVE_FFTFLOAT
      throw ParafaftCompiledWithoutSinglePrecisionSupport("CosmoLattice compiled without float FFT support.");
#else
      throw ParafaftCompiledWithoutSinglePrecisionSupport("Parafaft does not support single precision FFTs.");
#endif
      return std::shared_ptr<FFTPlanInterface<NDim, float>>();
    }

    /**
     * @brief Create double-precision plans.
     *
     * Creates a parafaft::ParaFaFT_R2C<NDim, ParaFaFT_Backend> object and wraps it in ParafaftPlanHolder.
     * Parafaft's in-place API accepts padded buffers matching CosmoLattice's layout.
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, double>>
    getPlans_double(const MPICartesianGroup &group, const FFTLayoutStruct<NDim> &layout) override
    {
#ifdef HAVE_MPI
#ifdef HAVE_PARAFAFT
      // Get global sizes
      auto globalSizes = layout.configurationSpace.getGlobalSizes();

      // if (globalSizes.size() != 3) {
      //   throw ParafaftPlannerException("Parafaft integration currently only supports 3D. Got ", globalSizes.size(),
      //                                  " dimensions.");
      // }

      int globalShape[NDim];
      for (size_t i = 0; i < NDim; ++i)
        globalShape[i] = static_cast<int>(globalSizes[i]);

      // Create parafaft object using base communicator
      auto parafaftObj =
          std::make_shared<parafaft::ParaFaFT_R2C<NDim, ParaFaFT_Backend>>(globalShape, group.getBaseComm());

      return std::make_shared<ParafaftPlanHolder<NDim, double>>(group, parafaftObj);
#else
      throw ParafaftPlannerException("Parafaft is disabled (HAVE_PARAFAFT not defined).");
      return std::shared_ptr<FFTPlanInterface<NDim, double>>();
#endif
#else
      throw ParafaftPlannerException("Parafaft requires MPI.");
      return std::shared_ptr<FFTPlanInterface<NDim, double>>();
#endif
    }
  };
} // namespace TempLat

#endif
