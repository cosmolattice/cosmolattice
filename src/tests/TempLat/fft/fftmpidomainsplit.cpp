
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/fft/fftmpidomainsplit.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct FFTMPIDomainSplitTester {
    static void Test(TDDAssertion &tdd) { tdd.verify(true); }
  };

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FFTMPIDomainSplitTester> test2;
  TempLat::TDDContainer<TempLat::FFTMPIDomainSplitTester> test3;
} // namespace
