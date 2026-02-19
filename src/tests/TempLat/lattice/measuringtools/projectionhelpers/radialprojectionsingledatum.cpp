
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionsingledatum.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/almostequal.h"

namespace TempLat
{

  template <typename T> struct RadialProjectionSingleDatumTester {
    static void Test(TDDAssertion &tdd);
  };

  template <typename T> inline void RadialProjectionSingleDatumTester<T>::Test(TDDAssertion &tdd)
  {

    RadialProjectionSingleDatum<T> a(2, 3, 0, 4, 1.25);
    RadialProjectionSingleDatum<T> b(1, 1, 0.5, 5, 2.25);

    auto c = combine(a, b);

    tdd.verify(AlmostEqual(c.multiplicity, 3.5));

    tdd.verify(AlmostEqual(c.average, (a.average * 1.25 + b.average * 2.25) / 3.5));

    tdd.verify(AlmostEqual(c.sampleVariance, (3 + 1) / 3.5 - c.average * c.average));

    tdd.verify(c.minVal == 0);

    tdd.verify(c.maxVal == 5);

    /* test the rescaling */
    c *= 3;

    /* multiplicity does not change */
    tdd.verify(AlmostEqual(c.multiplicity, 3.5));

    tdd.verify(AlmostEqual(c.average, 3 * (a.average * 1.25 + b.average * 2.25) / 3.5));

    tdd.verify(AlmostEqual(c.sampleVariance, 9 * (3 + 1) / 3.5 - c.average * c.average));

    tdd.verify(c.minVal == 0);

    tdd.verify(c.maxVal == 15);

    say << "See TempLat/lattice/measuringtoolsIO/projectionhelpers/radialprojectionresult_test.h\n";
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::RadialProjectionSingleDatumTester<double>> test;
}
