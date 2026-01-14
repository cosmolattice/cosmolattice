#ifndef TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTPLANNER_H
#define TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTPLANNER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2026

#ifndef NOFFT
#ifndef NOMPI
#ifndef NOPARAFAFT
#include <parafaft_r2c.hpp>
#endif
#endif
#endif

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/fft/external/parafaft/parafaftplanholder.h"
#include "TempLat/lattice/memory/memoryblock.h"

#include <memory>

namespace TempLat {

    MakeException(ParafaftPlannerException);

    /** \brief Plan creation for parafaft FFT transforms.
     *
     * Creates parafaft::ParaFaFT_R2C objects wrapped in ParafaftPlanHolder.
     * Currently only supports 3D and double precision.
     *
     * Unit test: make test-parafaftplanner
     **/

    class ParafaftPlanner : public FFTLibraryInterface {
    public:
        ParafaftPlanner() {
        }

        /**
         * @brief Set planner patience level.
         *
         * Parafaft uses FFTW internally with FFTW_ESTIMATE, so this is a no-op.
         * The parameter is accepted for interface compatibility.
         */
        virtual void setPlannerPatience(int level) override {
            // Parafaft uses FFTW_ESTIMATE internally - no patience setting
            (void)level;
        }

        /**
         * @brief Create float-precision plans.
         *
         * Not supported - parafaft only supports double precision.
         */
        virtual std::shared_ptr<PlanInterface<float>> getPlans_float(
            const MPICartesianGroup& group,
            const FFTLayoutStruct& layout) override {

#ifdef NOFFTFLOAT
            throw ParafaftCompiledWithoutSinglePrecisionSupport(
                "CosmoLattice compiled without float FFT support.");
#else
            throw ParafaftCompiledWithoutSinglePrecisionSupport(
                "Parafaft does not support single precision FFTs.");
#endif
            return std::shared_ptr<PlanInterface<float>>();
        }

        /**
         * @brief Create double-precision plans.
         *
         * Creates a parafaft::ParaFaFT_R2C<3> object and wraps it in ParafaftPlanHolder.
         */
        virtual std::shared_ptr<PlanInterface<double>> getPlans_double(
            const MPICartesianGroup& group,
            const FFTLayoutStruct& layout) override {

#ifndef NOMPI
#ifndef NOPARAFAFT
            // Get global sizes
            auto globalSizes = layout.configurationSpace.getGlobalSizes();

            if (globalSizes.size() != 3) {
                throw ParafaftPlannerException(
                    "Parafaft integration currently only supports 3D. Got ",
                    globalSizes.size(), " dimensions.");
            }

            int globalShape[3] = {
                static_cast<int>(globalSizes[0]),
                static_cast<int>(globalSizes[1]),
                static_cast<int>(globalSizes[2])
            };

            // Create parafaft object using base communicator
            auto parafaftObj = std::make_shared<parafaft::ParaFaFT_R2C<3>>(
                globalShape, group.getBaseComm()
            );

            return std::make_shared<ParafaftPlanHolder<double>>(group, parafaftObj);
#else
            throw ParafaftPlannerException("Parafaft is disabled (NOPARAFAFT defined).");
            return std::shared_ptr<PlanInterface<double>>();
#endif
#else
            throw ParafaftPlannerException("Parafaft requires MPI.");
            return std::shared_ptr<PlanInterface<double>>();
#endif
        }

    private:

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/external/parafaft/parafaftplanner_test.h"
#endif


#endif
