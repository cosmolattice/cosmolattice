
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/su2algebra/su2liealgebrafield.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct SU2LieAlgebraFieldTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2LieAlgebraFieldTester::Test(TDDAssertion &tdd)
  {
    static constexpr size_t NDim = 3;

    auto toolBox = MemoryToolBox<NDim>::makeShared(32, 1);

    Field<NDim, double> f0("myField0", toolBox);
    Field<NDim, double> f1("myField1", toolBox);
    Field<NDim, double> f2("myField2", toolBox);
    Field<NDim, double> f3("myField3", toolBox);

    auto res = SU2Field<NDim, double>(f0, f1, f2, f3);

    tdd.verify(res.SU2Get(2_c).toString() == "myField2(x)");

    res(3_c) = 24;

    auto ff3 = res(3_c);

    auto ff3_view = ff3.getLocalNDHostView();
    tdd.verify(ff3_view(0, 0, 0) == 24);

    SU2Field<NDim, double> mySU2("pimpin", toolBox, LatticeParameters<double>());
    tdd.verify(mySU2(3_c).toString() == "pimpin_3(x)");
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2LieAlgebraFieldTester> test;
}
