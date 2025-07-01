#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_HERMITIANPARTNERS_TEST_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_HERMITIANPARTNERS_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
template <size_t NDim> inline void TempLat::HermitianPartners<NDim>::Test(TempLat::TDDAssertion &tdd)
{
  HermitianPartners<4> hp({16, 16, 16, 16});
  /* simply test the default implementation */
  tdd.verify(hp.qualify({{}}) == HermitianRedundancy::none);

  Kokkos::Array<ptrdiff_t, 4> global{{16, 16, 16, 16}}, hpvec{{0, 0, 0, 0}};
  hp.putHermitianPartner(global, hpvec);
  bool correct = true;
  for (ptrdiff_t i = 0; i < 4; ++i)
    correct = correct && global[i] == hpvec[i];
  tdd.verify(correct);
}

#endif
