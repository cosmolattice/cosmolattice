
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/fft/external/fftw/fftwinterface.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct FFTWInterfaceTester {
    static void Test(TDDAssertion &tdd)
    {
      FFTWInterface<3> someInterface;
      tdd.verify(true);
    }
  };

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FFTWInterfaceTester> test;
}
