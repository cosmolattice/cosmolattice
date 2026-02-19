
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/memory/memorylayouts/layoutstructglobal.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  template <size_t NDim> struct LayoutStructGlobalTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> void LayoutStructGlobalTester<NDim>::Test(TDDAssertion &tdd)
  {
    /* test the operator== */
    LayoutStructGlobal<3> a({0, 0, 0});
    LayoutStructGlobal<3> b({0, 0, 0});
    LayoutStructGlobal<2> c({0, 0});
    LayoutStructGlobal<3> d({0, 0, 0});

    d.getGlobalSizes()[1] = 2;

    tdd.verify(!(a == c));
    tdd.verify((a == b));
    tdd.verify(!(a == d));

    a = LayoutStructGlobal<3>({10, 10, 10});

    device::IdxArray<3> memVec{}, posVec{}, memVec2{};

    memVec[0] = 1;
    memVec[1] = 2;
    memVec[2] = 8;

    for (ptrdiff_t i = 0; i < 3; ++i)
      posVec[i] = a.memoryIndexToSpatialCoordinate(memVec[i], i);

    say << "memVec " << memVec << " -> posVec " << posVec << "\n";

    tdd.verify(posVec[0] == 1);

    tdd.verify(posVec[1] == 2);

    tdd.verify(posVec[2] == -2);

    for (ptrdiff_t i = 0; i < 3; ++i)
      memVec2[i] = a.spatialCoordinateToMemoryIndex(posVec[i], i);

    say << "posVec " << posVec << " -> memVec2 " << memVec2 << "\n";
    say << "memVec " << memVec << " -> memVec2 " << memVec2 << "\n";

    tdd.verify(memVec[0] == memVec2[0]);
    tdd.verify(memVec[1] == memVec2[1]);
    tdd.verify(memVec[2] == memVec2[2]);

    a = LayoutStructGlobal<3>({10, 14, 18});

    memVec[0] = 1;
    memVec[1] = 12;
    memVec[2] = 16;

    for (ptrdiff_t i = 0; i < 3; ++i)
      posVec[i] = a.memoryIndexToSpatialCoordinate(memVec[i], i);

    say << "memVec " << memVec << " -> posVec " << posVec << "\n";

    tdd.verify(posVec[0] == 1);

    tdd.verify(posVec[1] == -2);

    tdd.verify(posVec[2] == -2);

    for (ptrdiff_t i = 0; i < 3; ++i)
      memVec2[i] = a.spatialCoordinateToMemoryIndex(posVec[i], i);

    say << "posVec " << posVec << " -> memVec2 " << memVec2 << "\n";
    say << "memVec " << memVec << " -> memVec2 " << memVec2 << "\n";

    tdd.verify(memVec[0] == memVec2[0]);
    tdd.verify(memVec[1] == memVec2[1]);
    tdd.verify(memVec[2] == memVec2[2]);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::LayoutStructGlobalTester<3>> test;
}
