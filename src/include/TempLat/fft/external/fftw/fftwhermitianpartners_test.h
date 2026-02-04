#ifndef TEMPLAT_FFT_EXTERNAL_FFTW_FFTWHERMITIANPARTNERS_TEST_H
#define TEMPLAT_FFT_EXTERNAL_FFTW_FFTWHERMITIANPARTNERS_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/memory/memorylayouts/hermitianpartners.h"

template <size_t NDim> inline void TempLat::FFTWHermitianPartners<NDim>::Test(TempLat::TDDAssertion &tdd)
{
  /* Default is to fail: to remind yourself to implement something here. */
  device::IdxArray<NDim> grid{};
  for (auto &val : grid)
    val = 128;
  HermitianPartners<NDim> hermitianPartners = FFTWHermitianPartners<NDim>::create(grid);

  tdd.verify(hermitianPartners.getMode() == TempLat::HermitianPartnersMode::fftw);
}

#endif
