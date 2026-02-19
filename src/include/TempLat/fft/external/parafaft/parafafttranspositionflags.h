#ifndef TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTTRANSPOSITIONFLAGS_H
#define TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTTRANSPOSITIONFLAGS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2026

#ifdef HAVE_PARAFAFT

#include "TempLat/lattice/memory/memorylayouts/fftlayoutstruct.h"

namespace TempLat
{
  /** @brief Transposition flags for parafaft.
   *
   * Parafaft does not support transposition - it always returns data in the
   * canonical order without local transposes. This class returns 0 for both
   * directions (no transposition flags).
   *
   * Unit test: ctest -R test-parafafttranspositionflags
   **/
  class ParafaftTranspositionFlags
  {
  public:
    ParafaftTranspositionFlags(const FFTLayoutStruct &layout)
    {
      // Parafaft doesn't support transposition - always uses canonical order
      (void)layout; // unused, but kept for interface compatibility
      mFlagC2R = 0u;
      mFlagR2C = 0u;
    }

    unsigned int c2r() const { return mFlagC2R; }
    unsigned int r2c() const { return mFlagR2C; }

    friend std::ostream &operator<<(std::ostream &ostream, const ParafaftTranspositionFlags &tr)
    {
      (void)tr; // unused
      ostream << "C2R : not transposed (parafaft does not support transposition).\n";
      ostream << "R2C : not transposed (parafaft does not support transposition).\n";
      return ostream;
    }

  private:
    unsigned int mFlagC2R;
    unsigned int mFlagR2C;
  };
} // namespace TempLat

#endif // HAVE_PARAFAFT

#endif
