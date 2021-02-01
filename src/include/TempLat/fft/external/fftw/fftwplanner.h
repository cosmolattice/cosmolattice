#ifndef TEMPLAT_FFT_EXTERNAL_FFTW_FFTWPLANNER_H
#define TEMPLAT_FFT_EXTERNAL_FFTW_FFTWPLANNER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/fft/external/fftw/fftwplanholder.h"
#include "TempLat/fft/external/fftw/fftwtranspositionflags.h"
#include "TempLat/lattice/memory/memoryblock.h"

namespace TempLat {

    MakeException(FFTWCompiledWithoutSinglePrecisionSupport);

    /** \brief A class which partially implements FFTLibraryInterface, namely the getPlans parts. That is, here the calls to the FFTW planner are made.
     *
     * Unit test: make test-fftwplanner
     **/

    class FFTWPlanner : public FFTLibraryInterface {
    public:
    /* Put public methods here. These should change very little over time. */
        FFTWPlanner() : patienceFlag(FFTW_ESTIMATE) {

        }

        virtual void setPlannerPatience(int level) {
            if (level > 3 ) level = 3;
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

        /** \brief Create fully working plans, which must self-destruct in the PlanInterface's destructor. Use shared_ptr's. */
        virtual std::shared_ptr<PlanInterface<float>> getPlans_float(const MPICartesianGroup& group, const FFTLayoutStruct& layout) {
#ifdef NOFFTFLOAT
            throw FFTWCompiledWithoutSinglePrecisionSupport();
            return std::shared_ptr<PlanInterface<float>>();
#else
            FFTWTranspositionFlags trFlags(layout);
            
            MemoryBlock<float> temp(layout.getMinimalMemorySize());
            std::vector<int> globalSizes_INT(layout.configurationSpace.getGlobalSizes().begin(), layout.configurationSpace.getGlobalSizes().end());

            auto c2r =
#ifndef NOMPI
            fftwf_mpi_plan_dft_c2r(
#else
            fftwf_plan_dft_c2r(
#endif
                                  layout.getNDimensions(),
#ifndef NOMPI
                                  layout.configurationSpace.getGlobalSizes().data(),
#else
                                  globalSizes_INT.data(),
#endif
                                  (fftwf_complex *) (float*) temp,
                                  (float*) temp,
#ifndef NOMPI
                                  group.getBaseComm(),
#endif
                                  patienceFlag | trFlags.c2r()
            );
            auto r2c =
#ifndef NOMPI
            fftwf_mpi_plan_dft_r2c(
#else
            fftwf_plan_dft_r2c(
#endif
                                  layout.getNDimensions(),
#ifndef NOMPI
                                  layout.configurationSpace.getGlobalSizes().data(),
#else
                                  globalSizes_INT.data(),
#endif
                                  (float*) temp,
                                  (fftwf_complex *) (float*) temp,
#ifndef NOMPI
                                  group.getBaseComm(),
#endif
                                  patienceFlag | trFlags.r2c()
            );

            return std::make_shared<FFTWPlanHolder<float>>(group, r2c, c2r);
#endif
        };
        /** \brief Create fully working plans, which must self-destruct in the PlanInterface's destructor. Use shared_ptr's. */
        /** \brief Create fully working plans, which must self-destruct in the PlanInterface's destructor. Use shared_ptr's. */
        virtual std::shared_ptr<PlanInterface<double>> getPlans_double(const MPICartesianGroup& group, const FFTLayoutStruct& layout) {

            FFTWTranspositionFlags trFlags(layout);

            MemoryBlock<double> temp(layout.getMinimalMemorySize());

            std::vector<int> globalSizes_INT(layout.configurationSpace.getGlobalSizes().begin(), layout.configurationSpace.getGlobalSizes().end());

            auto c2r =
#ifndef NOMPI
            fftw_mpi_plan_dft_c2r(
#else
            fftw_plan_dft_c2r(
#endif
                                  layout.getNDimensions(),
#ifndef NOMPI
                                  layout.configurationSpace.getGlobalSizes().data(),
#else
                                  globalSizes_INT.data(),
#endif
                                  (fftw_complex *) (double*) temp,
                                  (double*) temp,
#ifndef NOMPI
                                  group.getBaseComm(),
#endif
                                  patienceFlag | trFlags.c2r()
            );

            auto r2c =
#ifndef NOMPI
            fftw_mpi_plan_dft_r2c(
#else
            fftw_plan_dft_r2c(
#endif
                                  layout.getNDimensions(),
#ifndef NOMPI
                                  layout.configurationSpace.getGlobalSizes().data(),
#else
                                  globalSizes_INT.data(),
#endif
                                  (double*) temp,
                                  (fftw_complex *) (double*) temp,
#ifndef NOMPI
                                  group.getBaseComm(),
#endif
                                  patienceFlag | trFlags.r2c()
            );

            /* damned, get the order of the plans right here! Perhaps should have use structs in order to make the compiler throw an error when these are swapped... */
            return std::make_shared<FFTWPlanHolder<double>>(group, r2c, c2r);
        };


    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        unsigned int patienceFlag;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/external/fftw/fftwplanner_test.h"
#endif


#endif
