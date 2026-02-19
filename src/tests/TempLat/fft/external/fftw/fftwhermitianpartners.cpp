
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/fft/external/fftw/fftwhermitianpartners.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorylayouts/hermitianpartners.h"

namespace TempLat
{

  struct FFTWHermitianPartnersTester {
    static void Test(TDDAssertion &tdd)
    {
      /* Default is to fail: to remind yourself to implement something here. */
      device::IdxArray<1> grid{};
      for (auto &val : grid)
        val = 128;
      HermitianPartners<1> hermitianPartners = FFTWHermitianPartners<1>::create(grid);

      tdd.verify(hermitianPartners.getMode() == TempLat::HermitianPartnersMode::fftw);
    }
  };

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FFTWHermitianPartnersTester> test1;
  TempLat::TDDContainer<TempLat::FFTWHermitianPartnersTester> test2;
  TempLat::TDDContainer<TempLat::FFTWHermitianPartnersTester> test3;
  TempLat::TDDContainer<TempLat::FFTWHermitianPartnersTester> test4;
  TempLat::TDDContainer<TempLat::FFTWHermitianPartnersTester> test5;
  TempLat::TDDContainer<TempLat::FFTWHermitianPartnersTester> test6;
} // namespace
