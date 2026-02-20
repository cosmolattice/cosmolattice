
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2024

#include "TempLat/lattice/algebra/su2algebra/su2expmapinv.h"
#include "TempLat/lattice/algebra/su2algebra/su2expmap.h"
#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/almostequal.h"

namespace TempLat
{

  struct SU2ExpMapInvTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2ExpMapInvTester::Test(TDDAssertion &tdd)
  {
    static constexpr size_t NDim = 3;
    auto toolBox = MemoryToolBox<NDim>::makeShared(16, 1);
    LatticeParameters<double> latPar;

    SU2Field<NDim, double> U("test_U", toolBox, latPar);
    U(1_c) = 0.23;
    U(2_c) = 0.11;
    U(3_c) = 0.4;
    U.unitarize();

    SU2Field<NDim, double> roundtrip("test_rt", toolBox, latPar);
    roundtrip = exp(expinv(U));

    auto u0view = U(0_c).getLocalNDHostView();
    auto u1view = U(1_c).getLocalNDHostView();
    auto u2view = U(2_c).getLocalNDHostView();
    auto u3view = U(3_c).getLocalNDHostView();

    auto r0view = roundtrip(0_c).getLocalNDHostView();
    auto r1view = roundtrip(1_c).getLocalNDHostView();
    auto r2view = roundtrip(2_c).getLocalNDHostView();
    auto r3view = roundtrip(3_c).getLocalNDHostView();

    tdd.verify(AlmostEqual(r0view(0, 0, 0), u0view(0, 0, 0)));
    tdd.verify(AlmostEqual(r1view(0, 0, 0), u1view(0, 0, 0)));
    tdd.verify(AlmostEqual(r2view(0, 0, 0), u2view(0, 0, 0)));
    tdd.verify(AlmostEqual(r3view(0, 0, 0), u3view(0, 0, 0)));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2ExpMapInvTester> test;
}
