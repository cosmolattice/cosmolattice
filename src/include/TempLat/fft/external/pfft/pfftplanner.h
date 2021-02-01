#ifndef TEMPLAT_FFT_EXTERNAL_PFFT_PFFTPLANNER_H
#define TEMPLAT_FFT_EXTERNAL_PFFT_PFFTPLANNER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/fft/external/pfft/pfftplanholder.h"
#include "TempLat/fft/external/pfft/pffttranspositionflags.h"
#include "TempLat/lattice/memory/memoryblock.h"

namespace TempLat {

    MakeException(PFFTPlannerException);
    MakeException(PFFTCompiledWithoutSinglePrecisionSupport);

    /** \brief A class which implements part of FFTLibraryInterface, the planner part. Here all calls to PFFT_plan ... are made.
     * 
     * 
     * Unit test: make test-pfftplanner
     **/

    class PFFTPlanner : public FFTLibraryInterface {
    public:
    /* Put public methods here. These should change very little over time. */
        PFFTPlanner() : patienceFlag(PFFT_ESTIMATE) {

        }

        virtual void setPlannerPatience(int level) {
            if (level > 4 ) level = 4;
            switch (level) {
                case 4:
                    patienceFlag = PFFT_TUNE | PFFT_EXHAUSTIVE;
                    break;
                case 3:
                    patienceFlag = PFFT_EXHAUSTIVE;
                    break;
                case 2:
                    patienceFlag = PFFT_PATIENT;
                    break;
                case 1:
                    patienceFlag = PFFT_MEASURE;
                    break;
                case 0:
                default:
                    patienceFlag = PFFT_ESTIMATE;
                    break;
            }
        }

        /** \brief Create fully working plans, which must self-destruct in the PlanInterface's destructor. Use shared_ptr's. */
        virtual std::shared_ptr<PlanInterface<float>> getPlans_float(const MPICartesianGroup& group, const FFTLayoutStruct& layout) {
#ifdef NOFFTFLOAT
            throw PFFTCompiledWithoutSinglePrecisionSupport();
            return std::shared_ptr<PlanInterface<float>>();
#else
            PFFTTranspositionFlags trFlags(layout);

            MemoryBlock<float> temp(layout.getMinimalMemorySize());

            auto c2r =
            pfftf_plan_dft_c2r(
                                  layout.getNDimensions(),
                                  layout.configurationSpace.getGlobalSizes().data(),
                                  (pfftf_complex *) (float*) temp,
                                  (float*) temp,
                                  group.getComm_onlyDividedDimensions(),
                                  PFFT_BACKWARD,
                                  PFFT_PADDED_C2R | PFFT_MEASURE | trFlags.c2r()
            );
            auto r2c =

            pfftf_plan_dft_r2c(
                                  layout.getNDimensions(),
                                  layout.configurationSpace.getGlobalSizes().data(),
                                  (float*) temp,
                                  (pfftf_complex *) (float*) temp,
                                  group.getComm_onlyDividedDimensions(),
                                  PFFT_FORWARD,
                                  PFFT_PADDED_R2C | PFFT_MEASURE | trFlags.r2c()
            );
            
            if ( r2c == NULL || c2r == NULL ) {
                throw PFFTPlannerException("Got NULL plans from pfft.", "c2r", c2r, "r2c", r2c);
            }

            /* damned, get the order of the plans right here! Perhaps should have use structs in order to make the compiler throw an error when these are swapped... */
            return std::make_shared<PFFTPlanHolder<float>>(group, r2c, c2r);
#endif
        };

        /** \brief Create fully working plans, which must self-destruct in the PlanInterface's destructor. Use shared_ptr's. */
        /** \brief Create fully working plans, which must self-destruct in the PlanInterface's destructor. Use shared_ptr's. */
        virtual std::shared_ptr<PlanInterface<double>> getPlans_double(const MPICartesianGroup& group, const FFTLayoutStruct& layout) {

            PFFTTranspositionFlags trFlags(layout);

            MemoryBlock<double> temp(layout.getMinimalMemorySize());
            //say << "Making plans for PFFT. Sizes: " << layout.configurationSpace.getGlobalSizes() << " Flags: " << trFlags << "\n";
            auto c2r =
            pfft_plan_dft_c2r(
                                  layout.getNDimensions(),
                                  layout.configurationSpace.getGlobalSizes().data(),
                                  (pfft_complex *) (double*) temp,
                                  (double*) temp,
                                  group.getComm_onlyDividedDimensions(),
                                  PFFT_BACKWARD,
                                  PFFT_PADDED_C2R | PFFT_MEASURE | trFlags.c2r()
            );
            auto r2c =
            pfft_plan_dft_r2c(
                                  layout.getNDimensions(),
                                  layout.configurationSpace.getGlobalSizes().data(),
                                  (double*) temp,
                                  (pfft_complex *) (double*) temp,
                                  group.getComm_onlyDividedDimensions(),
                                  PFFT_FORWARD,
                                  PFFT_PADDED_R2C | PFFT_MEASURE | trFlags.r2c()
            );

            if ( r2c == NULL || c2r == NULL ) {
                throw PFFTPlannerException("Got NULL plans from pfft.", "c2r", c2r, "r2c", r2c, "layout", layout, "memory", temp);
            }

            return std::make_shared<PFFTPlanHolder<double>>(group, r2c, c2r);
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
#include "TempLat/fft/external/pfft/pfftplanner_test.h"
#endif


#endif
