#ifndef TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTMEMORYLAYOUT_H
#define TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTMEMORYLAYOUT_H

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
#include "TempLat/fft/external/parafaft/parafaftplanner.h"
#include "TempLat/fft/external/fftw/fftwhermitianpartners.h"
#include "TempLat/lattice/memory/memorylayouts/fftlayoutstruct.h"

#include <numeric>

namespace TempLat {

    MakeException(ParafaftMemoryLayoutException);

    /** \brief Memory layout computation for parafaft.
     *
     * Parafaft uses a (D-1)-dimensional pencil decomposition. For 3D:
     * - Real space: distributed in dimensions [0,1], full in dimension [2]
     * - Complex space: full in dimension [0], distributed in dimensions [1,2]
     *
     * Note: We use the FFTW flag for FFTLayoutStruct compatibility since
     * parafaft uses FFTW conventions for r2c padding.
     *
     * Unit test: make test-parafaftmemorylayout
     **/

    class ParafaftMemoryLayout : public ParafaftPlanner {
    public:
        ParafaftMemoryLayout() { }

        /**
         * @brief Compute local memory sizes for this MPI rank.
         *
         * Creates a temporary parafaft object to query local sizes, then
         * populates FFTLayoutStruct with the configuration and Fourier space layouts.
         */
        virtual FFTLayoutStruct computeLocalSizes(MPICartesianGroup group,
                                                  std::vector<ptrdiff_t> nGridPoints,
                                                  bool forbidTransposition = false) override {

            ptrdiff_t nDimensions = nGridPoints.size();

            if (nDimensions != 3) {
                throw ParafaftMemoryLayoutException("Parafaft integration currently only supports 3D. Got ", nDimensions, " dimensions.");
            }

            // Create FFTLayoutStruct - use FFTW mode for r2c padding compatibility
            // (parafaft uses same padding convention as FFTW)
            FFTLayoutStruct result(nGridPoints, true, false);

            // Initialize vectors for local layout
            std::vector<ptrdiff_t> confLocalSizes(nDimensions);
            std::vector<ptrdiff_t> confLocalStarts(nDimensions, 0);
            std::vector<ptrdiff_t> fourLocalSizes(nDimensions);
            std::vector<ptrdiff_t> fourLocalStarts(nDimensions, 0);
            std::vector<ptrdiff_t> fourTransposition(nDimensions);
            std::iota(fourTransposition.begin(), fourTransposition.end(), 0);

            ptrdiff_t parafaftRequiredMemory = 0;

#ifndef NOMPI
#ifndef NOPARAFAFT
            // Create temporary parafaft object to query sizes
            int globalShape[3] = {
                static_cast<int>(nGridPoints[0]),
                static_cast<int>(nGridPoints[1]),
                static_cast<int>(nGridPoints[2])
            };

            // Use the base communicator - parafaft will create its own Cartesian topology
            parafaft::ParaFaFT_R2C<3> temp(globalShape, group.getBaseComm());

            // Query real (configuration) space layout
            int realShape[3], realStart[3];
            temp.get_local_real_shape(realShape);
            temp.get_real_global_start(realStart);

            for (int i = 0; i < 3; ++i) {
                confLocalSizes[i] = realShape[i];
                confLocalStarts[i] = realStart[i];
            }

            // Apply r2c padding to configuration space (last dimension)
            // FFTW convention: real data is padded to 2*(N/2+1)
            ptrdiff_t fourierLastDim = nGridPoints[2] / 2 + 1;
            confLocalSizes[2] = 2 * fourierLastDim;

            // Query complex (Fourier) space layout
            int complexShape[3], complexStart[3];
            temp.get_local_complex_shape(complexShape);
            temp.get_complex_global_start(complexStart);

            for (int i = 0; i < 3; ++i) {
                fourLocalSizes[i] = complexShape[i];
                fourLocalStarts[i] = complexStart[i];
            }

            // Compute memory requirement
            // Parafaft needs max of real size and complex size
            ptrdiff_t realSize = 1;
            for (int i = 0; i < 3; ++i) realSize *= confLocalSizes[i];

            ptrdiff_t complexSize = 2;  // Complex = 2 reals
            for (int i = 0; i < 3; ++i) complexSize *= fourLocalSizes[i];

            parafaftRequiredMemory = std::max(realSize, complexSize);
#else
            // Non-parafaft fallback (shouldn't happen)
            for (ptrdiff_t i = 0; i < nDimensions; ++i) {
                confLocalSizes[i] = nGridPoints[i];
                fourLocalSizes[i] = nGridPoints[i];
            }
            fourLocalSizes[nDimensions - 1] = nGridPoints[nDimensions - 1] / 2 + 1;
            confLocalSizes[nDimensions - 1] = 2 * fourLocalSizes[nDimensions - 1];
#endif
#else
            // Non-MPI fallback (shouldn't happen since parafaft requires MPI)
            for (ptrdiff_t i = 0; i < nDimensions; ++i) {
                confLocalSizes[i] = nGridPoints[i];
                fourLocalSizes[i] = nGridPoints[i];
            }
            fourLocalSizes[nDimensions - 1] = nGridPoints[nDimensions - 1] / 2 + 1;
            confLocalSizes[nDimensions - 1] = 2 * fourLocalSizes[nDimensions - 1];
#endif

            // Populate result
            result.configurationSpace.setLocalSizes(confLocalSizes);
            result.configurationSpace.setLocalStarts(confLocalStarts);
            result.fourierSpace.setLocalSizes(fourLocalSizes);
            result.fourierSpace.setLocalStarts(fourLocalStarts);
            result.fourierSpace.setTranspositionMap_memoryToGlobalSpace(fourTransposition);

            // Add memory requirement (already in real units)
            result.addExternalMemoryRequest(parafaftRequiredMemory);

            // Set Hermitian partners (same as FFTW/PFFT)
            result.fourierSpace.setHermitianPartners(
                FFTWHermitianPartners::create(result.configurationSpace.getGlobalSizes())
            );

            return result;
        }

    private:

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/external/parafaft/parafaftmemorylayout_test.h"
#endif


#endif
