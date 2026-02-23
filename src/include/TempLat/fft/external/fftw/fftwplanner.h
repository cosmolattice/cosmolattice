#ifndef TEMPLAT_FFT_EXTERNAL_FFTW_FFTWPLANNER_H
#define TEMPLAT_FFT_EXTERNAL_FFTW_FFTWPLANNER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <algorithm>

#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/fft/external/fftw/fftwplanholder.h"
#include "TempLat/fft/external/fftw/fftwtranspositionflags.h"
#include "TempLat/lattice/memory/memoryblock.h"

namespace TempLat
{
  MakeException(FFTWCompiledWithoutSinglePrecisionSupport);

  /** @brief A class which partially implements FFTLibraryInterface, namely the getPlans parts. That is, here the calls
   *to the FFTW planner are made.
   *
   * Unit test: ctest -R test-fftwplanner
   **/
  template <size_t NDim> class FFTWPlanner : public FFTLibraryInterface<NDim>
  {
  public:
    // Put public methods here. These should change very little over time.
    FFTWPlanner() : patienceFlag(FFTW_ESTIMATE) {}

    virtual void setPlannerPatience(int level)
    {
      level = std::min(level, 3);
      switch (level) {
      case 3:
        patienceFlag = FFTW_EXHAUSTIVE;
        break;
      case 2:
        patienceFlag = FFTW_PATIENT;
        break;
      case 1:
        patienceFlag = FFTW_MEASURE;
        break;
      case 0:
      default:
        patienceFlag = FFTW_ESTIMATE;
        break;
      }
    }

    /** @brief Create fully working plans, which must self-destruct in the PlanInterface's destructor. Use shared_ptr's.
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, float>> getPlans_float(const MPICartesianGroup &group,
                                                                          const FFTLayoutStruct<NDim> &layout)
    {
#ifndef HAVE_FFTFLOAT
      throw FFTWCompiledWithoutSinglePrecisionSupport();
      return std::shared_ptr<FFTPlanInterface<NDim, float>>();
#else
      FFTWTranspositionFlags trFlags(layout);

      MemoryBlock<NDim, float> temp(layout.getMinimalMemorySize());
      std::vector<int> globalSizes_INT(layout.configurationSpace.getGlobalSizes().begin(),
                                       layout.configurationSpace.getGlobalSizes().end());
      std::vector<ptrdiff_t> globalSizes_PTRDIFF(layout.configurationSpace.getGlobalSizes().begin(),
                                                 layout.configurationSpace.getGlobalSizes().end());
      auto c2r =
#ifdef HAVE_MPI
          fftwf_mpi_plan_dft_c2r(
#else
          fftwf_plan_dft_c2r(
#endif
              layout.getNDimensions(),
#ifdef HAVE_MPI
              globalSizes_PTRDIFF.data(),
#else
              globalSizes_INT.data(),
#endif
              (fftwf_complex *)(float *)temp, (float *)temp,
#ifdef HAVE_MPI
              group.getBaseComm(),
#endif
              patienceFlag | trFlags.c2r());
      auto r2c =
#ifdef HAVE_MPI
          fftwf_mpi_plan_dft_r2c(
#else
          fftwf_plan_dft_r2c(
#endif
              layout.getNDimensions(),
#ifdef HAVE_MPI
              globalSizes_PTRDIFF.data(),
#else
              globalSizes_INT.data(),
#endif
              (float *)temp, (fftwf_complex *)(float *)temp,
#ifdef HAVE_MPI
              group.getBaseComm(),
#endif
              patienceFlag | trFlags.r2c());

      return std::make_shared<FFTWPlanHolder<float>>(group, r2c, c2r);
#endif
    };
    /** @brief Create fully working plans, which must self-destruct in the PlanInterface's destructor. Use shared_ptr's.
     */
    /** @brief Create fully working plans, which must self-destruct in the PlanInterface's destructor. Use shared_ptr's.
     */
    virtual std::shared_ptr<FFTPlanInterface<NDim, double>> getPlans_double(const MPICartesianGroup &group,
                                                                            const FFTLayoutStruct<NDim> &layout)
    {
      FFTWTranspositionFlags trFlags(layout);

      MemoryBlock<NDim, double> temp(layout.getMinimalMemorySize());

      std::vector<int> globalSizes_INT(layout.configurationSpace.getGlobalSizes().size());
      for (size_t i = 0; i < layout.configurationSpace.getGlobalSizes().size(); ++i)
        globalSizes_INT[i] = (int)layout.configurationSpace.getGlobalSizes()[i];
      std::vector<ptrdiff_t> globalSizes_PTRDIFF(layout.configurationSpace.getGlobalSizes().begin(),
                                                 layout.configurationSpace.getGlobalSizes().end());

      auto c2r =
#ifdef HAVE_MPI
          fftw_mpi_plan_dft_c2r(
#else
          fftw_plan_dft_c2r(
#endif
              layout.getNDimensions(),
#ifdef HAVE_MPI
              globalSizes_PTRDIFF.data(),
#else
              globalSizes_INT.data(),
#endif
              (fftw_complex *)temp.data(), (double *)temp.data(),
#ifdef HAVE_MPI
              group.getBaseComm(),
#endif
              patienceFlag | trFlags.c2r());

      auto r2c =
#ifdef HAVE_MPI
          fftw_mpi_plan_dft_r2c(
#else
          fftw_plan_dft_r2c(
#endif
              layout.getNDimensions(),
#ifdef HAVE_MPI
              globalSizes_PTRDIFF.data(),
#else
              globalSizes_INT.data(),
#endif
              (double *)temp, (fftw_complex *)(double *)temp,
#ifdef HAVE_MPI
              group.getBaseComm(),
#endif
              patienceFlag | trFlags.r2c());

      /* damned, get the order of the plans right here! Perhaps should have use structs in order to make the compiler
       * throw an error when these are swapped... */
      return std::make_shared<FFTWPlanHolder<NDim, double>>(group, r2c, c2r);
    };

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    unsigned int patienceFlag;
  };

} // namespace TempLat

#endif
