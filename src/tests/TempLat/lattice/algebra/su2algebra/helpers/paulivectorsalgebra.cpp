
/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  , Year: 2025
#include "TempLat/lattice/algebra/su2algebra/helpers/paulivectorsalgebra.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/almostequal.h"

namespace TempLat {

struct PauliVectorsAlgebraTester {
  static void Test(TDDAssertion &tdd);
};

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  Adrien Florio, Year: 2025

void PauliVectorsAlgebraTester::Test(TDDAssertion &tdd)
{
  /* Default is to fail: to remind yourself to implement something here. */

  device::array<double, 4> A = {2.4, 0.96, 0.1, 0.01};
  device::array<double, 4> B = {2.4, 0.56, 3.4, 0.01};
  device::array<double, 4> c;

  PauliVectorsAlgebra::multiply_inplace(c, A, B);

  tdd.verify(AlmostEqual(c[0], 4.8823));
  tdd.verify(AlmostEqual(c[1], 3.681));
  tdd.verify(AlmostEqual(c[2], 8.404));
  tdd.verify(AlmostEqual(c[3], -3.16));
}

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::PauliVectorsAlgebraTester> test;
}
