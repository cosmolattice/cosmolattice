
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#include "TempLat/util/powr.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct PowrTester {
    static void Test(TDDAssertion &tdd);
  };

  void PowrTester::Test(TDDAssertion &tdd)
  {
    tdd.verify(powr<2>(3) == 9);
    tdd.verify(powr<3>(2) == 8);
    tdd.verify(powr<0>(5) == 1);
    tdd.verify(AlmostEqual(powr<-2>(2.0), 0.25));
    tdd.verify(AlmostEqual(powr<-2>(2), 0));

    tdd.verify(powr<16>(2) == 65536);

    tdd.verify(AlmostEqual(powr<2>(device::complex<double>(0., 1.)), device::complex<double>(-1., 0.)));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::PowrTester> test;
}
