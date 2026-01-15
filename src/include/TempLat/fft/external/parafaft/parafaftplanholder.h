#ifndef TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTPLANHOLDER_H
#define TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTPLANHOLDER_H

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
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"
#include "TempLat/lattice/memory/memoryblock.h"

#include <memory>
#include <complex>
#include <cstring>
#include <array>
#include <vector>

namespace TempLat {

    MakeException(ParafaftPlanHolderException);
    MakeException(ParafaftCompiledWithoutSinglePrecisionSupport);

    /** \brief Plan holder for parafaft FFT transforms.
     *
     * This class holds a parafaft::ParaFaFT_R2C object and implements the
     * PlanInterface<T> methods for r2c and c2r transforms.
     *
     * Note: Parafaft only supports double precision currently.
     *
     * Unit test: make test-parafaftplanholder
     **/

    template <typename T>
    class ParafaftPlanHolder : public FFTLibraryInterface::PlanInterface<T> {
    public:

        using Complex = std::complex<double>;

#ifndef NOMPI
#ifndef NOPARAFAFT
        /**
         * @brief Constructor - takes ownership of the parafaft object.
         *
         * We store the MPICartesianGroup to keep the MPI communicator alive.
         * Also stores layout information for padded <-> unpadded conversion.
         *
         * @note TEMPORARY: Allocates extra buffers for memory layout conversion.
         *       See member variable comments for optimization opportunities.
         */
        ParafaftPlanHolder(MPICartesianGroup group,
                           std::shared_ptr<parafaft::ParaFaFT_R2C<3>> parafaftObj,
                           std::array<ptrdiff_t, 3> paddedShape,
                           std::array<ptrdiff_t, 3> unpaddedShape)
            : mGroup(group),
              mParafaft(parafaftObj),
              mPaddedShape(paddedShape),
              mUnpaddedShape(unpaddedShape) {
            // TEMPORARY: Allocate buffers for padded <-> unpadded conversion
            // These allocations can be eliminated by adding stride support to parafaft
            ptrdiff_t unpaddedSize = mUnpaddedShape[0] * mUnpaddedShape[1] * mUnpaddedShape[2];
            mTempRealBuffer.resize(unpaddedSize);

            // Get complex buffer size from parafaft
            int complexShape[3];
            mParafaft->get_local_complex_shape(complexShape);
            ptrdiff_t complexSize = complexShape[0] * complexShape[1] * complexShape[2];
            mTempComplexBuffer.resize(complexSize);
        }
#endif
#endif

        virtual ~ParafaftPlanHolder() {
say << "ParafaftPlanHolder is correctly being destructed.\n";
            // Parafaft cleans up internally via its destructor
        }

        /**
         * @brief Complex-to-real backward transform (in-place).
         *
         * Parafaft's backward() copies input to internal buffers before processing,
         * so using the same memory for both is safe.
         */
        virtual void c2r(MemoryBlock<T> &mBlock) override {
            execute_c2r(mBlock);
        }

        /**
         * @brief Real-to-complex forward transform (in-place).
         *
         * Parafaft's forward() copies input to internal buffers before processing,
         * so using the same memory for both is safe.
         */
        virtual void r2c(MemoryBlock<T> &mBlock) override {
            execute_r2c(mBlock);
        }

    private:
#ifndef NOMPI
#ifndef NOPARAFAFT
        // Keep group alive for MPI communicator lifetime
        MPICartesianGroup mGroup;

        // Shared pointer to parafaft object
        std::shared_ptr<parafaft::ParaFaFT_R2C<3>> mParafaft;

        // =========================================================================
        // TEMPORARY WORKAROUND: Memory layout conversion buffers
        // =========================================================================
        // CosmoLattice uses FFTW/PFFT r2c convention with PADDED real arrays
        // (last dimension is 2*(N/2+1)), while parafaft expects UNPADDED
        // contiguous real data (last dimension is N).
        //
        // This mismatch requires copying data between layouts before/after FFTs.
        // These temporary buffers add memory overhead and copy operations.
        //
        // FUTURE OPTIMIZATION: Add stride support to parafaft library to handle
        // padded input directly, eliminating these copies. See Option B in:
        // thoughts/shared/plans/2026-01-15-parafaft-memory-layout-copy-fix.md
        // =========================================================================

        // Layout information for padded <-> unpadded conversion
        std::array<ptrdiff_t, 3> mPaddedShape;    // CosmoLattice layout [N0_local, N1_local, 2*(N2/2+1)]
        std::array<ptrdiff_t, 3> mUnpaddedShape;  // Parafaft layout [N0_local, N1_local, N2]

        // Temporary buffers for data conversion (EXTRA MEMORY ALLOCATION)
        std::vector<double> mTempRealBuffer;       // Unpadded real buffer for parafaft input
        std::vector<Complex> mTempComplexBuffer;   // Complex buffer for parafaft output
#endif
#endif

        // =========================================================================
        // TEMPORARY WORKAROUND: Copy functions for memory layout conversion
        // =========================================================================
        // These functions copy data between CosmoLattice's padded layout and
        // parafaft's expected unpadded layout. This adds overhead but is necessary
        // until parafaft gains native stride support.
        //
        // FUTURE OPTIMIZATION: Remove these functions once parafaft supports
        // strided input data. See Option B in:
        // thoughts/shared/plans/2026-01-15-parafaft-memory-layout-copy-fix.md
        // =========================================================================

        /**
         * @brief Copy data from CosmoLattice's padded layout to unpadded layout.
         *
         * CosmoLattice layout: [N0_local][N1_local][2*(N2/2+1)] (padded)
         * Parafaft layout: [N0_local][N1_local][N2] (unpadded)
         *
         * Copies row by row, skipping the padding at the end of each row.
         *
         * @note TEMPORARY: This copy can be eliminated by adding stride support
         *       to the parafaft library.
         */
        void copyPaddedToUnpadded(const double* padded, double* unpadded) {
            for (ptrdiff_t i = 0; i < mUnpaddedShape[0]; ++i) {
                for (ptrdiff_t j = 0; j < mUnpaddedShape[1]; ++j) {
                    // Source: padded layout with stride mPaddedShape[2]
                    const double* src = padded +
                        (i * mPaddedShape[1] + j) * mPaddedShape[2];
                    // Destination: unpadded layout with stride mUnpaddedShape[2]
                    double* dst = unpadded +
                        (i * mUnpaddedShape[1] + j) * mUnpaddedShape[2];
                    // Copy only actual data (not padding)
                    std::memcpy(dst, src, mUnpaddedShape[2] * sizeof(double));
                }
            }
        }

        /**
         * @brief Copy data from unpadded layout to CosmoLattice's padded layout.
         *
         * Reverse of copyPaddedToUnpadded. Used for c2r backward transform.
         * Padding is left unchanged (typically zeros or garbage - not used by CosmoLattice).
         *
         * @note TEMPORARY: This copy can be eliminated by adding stride support
         *       to the parafaft library.
         */
        void copyUnpaddedToPadded(const double* unpadded, double* padded) {
            for (ptrdiff_t i = 0; i < mUnpaddedShape[0]; ++i) {
                for (ptrdiff_t j = 0; j < mUnpaddedShape[1]; ++j) {
                    // Source: unpadded layout with stride mUnpaddedShape[2]
                    const double* src = unpadded +
                        (i * mUnpaddedShape[1] + j) * mUnpaddedShape[2];
                    // Destination: padded layout with stride mPaddedShape[2]
                    double* dst = padded +
                        (i * mPaddedShape[1] + j) * mPaddedShape[2];
                    // Copy actual data
                    std::memcpy(dst, src, mUnpaddedShape[2] * sizeof(double));
                }
            }
        }

        // =========================================================================
        // TEMPORARY WORKAROUND: execute_r2c with memory layout conversion
        // =========================================================================
        // This implementation copies data between padded (CosmoLattice) and
        // unpadded (parafaft) layouts because parafaft doesn't support strided input.
        //
        // PERFORMANCE IMPACT: Two extra memory copies per FFT:
        //   1. copyPaddedToUnpadded: CosmoLattice -> temp real buffer
        //   2. memcpy: temp complex buffer -> CosmoLattice output
        //
        // FUTURE OPTIMIZATION: Add stride support to parafaft library to handle
        // padded input directly, eliminating copies. See Option B in:
        // thoughts/shared/plans/2026-01-15-parafaft-memory-layout-copy-fix.md
        // =========================================================================

        // Double precision implementation with padded-to-unpadded copy
        template <typename S = T>
        typename std::enable_if<std::is_same<S, double>::value, void>::type
        execute_r2c(MemoryBlock<S> &mBlock) {
#ifndef NOMPI
#ifndef NOPARAFAFT
            // TEMPORARY: Copy from padded (CosmoLattice) to unpadded (parafaft) layout
            copyPaddedToUnpadded(mBlock.ptr(), mTempRealBuffer.data());

            // Execute forward FFT with unpadded buffer
            mParafaft->forward(mTempRealBuffer.data(), mTempComplexBuffer.data());

            // Copy complex result back to CosmoLattice's memory block
            // The complex data is stored at the same location as real data (in-place)
            Complex* output_ptr = reinterpret_cast<Complex*>(mBlock.ptr());
            std::memcpy(output_ptr, mTempComplexBuffer.data(),
                        mTempComplexBuffer.size() * sizeof(Complex));
#endif
#endif
        }

        // =========================================================================
        // TEMPORARY WORKAROUND: execute_c2r with memory layout conversion
        // =========================================================================
        // This implementation copies data between padded (CosmoLattice) and
        // unpadded (parafaft) layouts because parafaft doesn't support strided output.
        //
        // PERFORMANCE IMPACT: Two extra memory copies per FFT:
        //   1. memcpy: CosmoLattice input -> temp complex buffer
        //   2. copyUnpaddedToPadded: temp real buffer -> CosmoLattice output
        //
        // FUTURE OPTIMIZATION: Add stride support to parafaft library to handle
        // padded output directly, eliminating copies. See Option B in:
        // thoughts/shared/plans/2026-01-15-parafaft-memory-layout-copy-fix.md
        // =========================================================================

        // Double precision implementation with unpadded-to-padded copy
        template <typename S = T>
        typename std::enable_if<std::is_same<S, double>::value, void>::type
        execute_c2r(MemoryBlock<S> &mBlock) {
#ifndef NOMPI
#ifndef NOPARAFAFT
            // TEMPORARY: Copy complex data from CosmoLattice's memory block to temp buffer
            Complex* input_ptr = reinterpret_cast<Complex*>(mBlock.ptr());
            std::memcpy(mTempComplexBuffer.data(), input_ptr,
                        mTempComplexBuffer.size() * sizeof(Complex));

            // Execute backward FFT
            mParafaft->backward(mTempComplexBuffer.data(), mTempRealBuffer.data());

            // TEMPORARY: Copy from unpadded (parafaft) to padded (CosmoLattice) layout
            copyUnpaddedToPadded(mTempRealBuffer.data(), mBlock.ptr());
#endif
#endif
        }

        // Float precision - not supported
#ifndef NOFFTFLOAT
        template <typename S = T>
        typename std::enable_if<std::is_same<S, float>::value, void>::type
        execute_r2c(MemoryBlock<S> &mBlock) {
            throw ParafaftCompiledWithoutSinglePrecisionSupport("Parafaft does not support single precision.");
        }

        template <typename S = T>
        typename std::enable_if<std::is_same<S, float>::value, void>::type
        execute_c2r(MemoryBlock<S> &mBlock) {
            throw ParafaftCompiledWithoutSinglePrecisionSupport("Parafaft does not support single precision.");
        }
#endif

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/external/parafaft/parafaftplanholder_test.h"
#endif


#endif
