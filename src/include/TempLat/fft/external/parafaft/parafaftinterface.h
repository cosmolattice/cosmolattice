#if !defined(TempLat_FFT_EXTERNAL_PARAFAFT_PARAFAFTINTERFACE_H) && defined(HAVE_PARAFAFT)
#define TempLat_FFT_EXTERNAL_PARAFAFT_PARAFAFTINTERFACE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2026

#include "TempLat/fft/external/parafaft/parafaftmemorylayout.h"
#include "TempLat/fft/external/parafaft/parafaftguard.h"

namespace TempLat
{
  /** @brief Top-level interface for parafaft FFT backend.
   *
   * Parafaft uses pencil decomposition to parallelize FFTs across D-1 dimensions.
   * For 3D, this means 2D parallelization, but using MPI_Alltoallw
   * instead of local transposes.
   *
   * Inheritance hierarchy:
   * ParafaftInterface -> ParafaftMemoryLayout -> ParafaftPlanner -> FFTLibraryInterface
   *
   * Unit test: ctest -R test-parafaftinterface
   **/
  template <size_t NDim> class ParafaftInterface : public ParafaftMemoryLayout<NDim>
  {
  public:
    ParafaftInterface() {}

    /**
     * @brief Returns maximum number of dimensions that can be parallelized.
     *
     * Parafaft supports D-1 dimensional decomposition (pencil decomposition).
     * For 3D data, this returns 2 (distribute over 2 dimensions).
     */
    virtual ptrdiff_t getMaximumNumberOfDimensionsToDivide(ptrdiff_t nDimensions) override
    {
      // Pencil decomposition: all but one dimension can be distributed
      return std::max((ptrdiff_t)1, nDimensions - 1);
    }

    /**
     * @brief Returns intrinsic rescaling factors.
     *
     * Parafaft (via FFTW) produces unnormalized FFTs, same as FFTW.
     * Returns default IntrinsicScales (1.0, 1.0).
     */
    virtual IntrinsicScales getIntrinsicRescaleToGetUnnormalizedFFT(ptrdiff_t nGridPoints) override
    {
      return IntrinsicScales();
    }
  };
} // namespace TempLat

#endif
