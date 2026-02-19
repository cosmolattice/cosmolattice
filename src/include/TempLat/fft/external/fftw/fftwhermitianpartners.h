#ifndef TEMPLAT_FFT_EXTERNAL_FFTW_FFTWHERMITIANPARTNERS_H
#define TEMPLAT_FFT_EXTERNAL_FFTW_FFTWHERMITIANPARTNERS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <memory>

#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorylayouts/hermitianredundancy.h"
#include "TempLat/lattice/memory/memorylayouts/hermitianpartners.h"

namespace TempLat
{

  MakeException(FFTWHermitianPartnersWrongSizeException);

  /** @brief A class which holds all information about the redundancy in
   *  the complex values from a r2c fft. The redundancy comes from
   *  hermitian symmetry, so certain elements must be each other's
   *  complex conjugate. This class tells you which elements are each
   *  other's
   *
   *
   * Unit test: ctest -R test-fftwhermitianpartners
   **/
  template <size_t NDim> class FFTWHermitianPartners : public HermitianPartners<NDim>
  {
  public:
    /** @brief Construct a std::shared_ptr to a new instance. */
    static HermitianPartners<NDim> create(const device::IdxArray<NDim> &globalSizes)
    {
      auto instance = HermitianPartners<NDim>(globalSizes);
      instance.setMode(HermitianPartnersMode::fftw);
      return instance;
    }

    FFTWHermitianPartners() = delete;
  };

} // namespace TempLat

#endif
