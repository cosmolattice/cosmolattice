
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2024

#include "TempLat/lattice/algebra/su2algebra/su2expmap.h"
#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/lattice/algebra/su2algebra/su2liealgebrafield.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/almostequal.h"

namespace TempLat
{

  struct SU2ExpMapTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2ExpMapTester::Test(TDDAssertion &tdd)
  {
    static constexpr size_t NDim = 3;
    auto toolBox = MemoryToolBox<NDim>::makeShared(16, 1);
    LatticeParameters<double> latPar;

    SU2LieAlgebraField<NDim, double> algebra("test_alg", toolBox, latPar);
    algebra(1_c) = 1.0;
    algebra(2_c) = 2.0;
    algebra(3_c) = 3.0;

    SU2Field<NDim, double> result("test_res", toolBox, latPar);
    result = exp(algebra);

    auto f0view = result(0_c).getLocalNDHostView();
    auto f1view = result(1_c).getLocalNDHostView();
    auto f2view = result(2_c).getLocalNDHostView();
    auto f3view = result(3_c).getLocalNDHostView();

    tdd.verify(AlmostEqual(f0view(0, 0, 0), -0.82529906208));
    tdd.verify(AlmostEqual(f1view(0, 0, 0), -0.15092132722));
    tdd.verify(AlmostEqual(f2view(0, 0, 0), -0.30184265444));
    tdd.verify(AlmostEqual(f3view(0, 0, 0), -0.45276398166));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2ExpMapTester> test;
}
