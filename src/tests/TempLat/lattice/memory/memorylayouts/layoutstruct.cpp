
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  template <size_t NDim> struct LayoutStructTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> void LayoutStructTester<NDim>::Test(TDDAssertion &tdd)
  {
    const ptrdiff_t nGhost = 0;
    /* test the operator== */
    LayoutStruct<3> a({0, 0, 0}, nGhost);
    LayoutStruct<3> b({0, 0, 0}, nGhost);
    LayoutStruct<2> c({0, 0}, nGhost);
    LayoutStruct<3> d({0, 0, 0}, nGhost);

    d.getLocalSizes()[1] = 2;

    tdd.verify(!(a == c));
    tdd.verify((a == b));
    tdd.verify(!(a == d));

    /* test the transposition */
    a = LayoutStruct<3>({16, 16, 16}, nGhost); // reset
    device::IdxArray<3> newLocalSizes{{4, 5, 6}};
    a.setLocalSizes(newLocalSizes);

    /* test that these propagate correctly */
    tdd.verify(a.getLocalSizes()[0] == a.getSizesInMemory()[0]);
    tdd.verify(a.getLocalSizes()[1] == a.getSizesInMemory()[1]);
    tdd.verify(a.getLocalSizes()[2] == a.getSizesInMemory()[2]);

    std::cout << "a.getLocalSizes() = " << a.getLocalSizes() << "   |   "
              << "a.getSizesInMemory() = " << a.getSizesInMemory() << std::endl;

    device::IdxArray<3> newLocalStarts{{7, 8, 9}};
    a.setLocalStarts(newLocalStarts);

    a.setTranspositionMap_memoryToGlobalSpace({{2, 0, 1}});

    device::IdxArray<3> memVec{}, posVec{}, memVec2{};

    memVec[0] = 1;
    memVec[1] = 2;
    memVec[2] = 3;

    device::apply([&](const auto... idx) { a.putSpatialLocationFromMemoryIndexInto(posVec, idx...); }, memVec);

    say << "memVec " << memVec << " -> posVec " << posVec << "\n";

    /* mem pos 2 at mem dim 1 -> global dim 0 -> global pos 2 + 7 = 9 -> 9 - 16 = -7 */
    tdd.verify(posVec[0] == -7);

    /* mem pos 3 at mem dim 2 -> global dim 1 -> global pos 3 + 8 = 11 -> 11 - 16 = -5 */
    tdd.verify(posVec[1] == -5);

    /* mem pos 1 at mem dim 0 -> global dim 2 -> global pos 1 + 9 = 10 -> 10 - 16 = -6 */
    tdd.verify(posVec[2] == -6);

    device::apply([&](const auto... idx) { a.putMemoryIndexFromSpatialLocationInto(memVec2, idx...); }, posVec);

    say << "posVec " << posVec << " -> memVec2 " << memVec2 << "\n";
    say << "memVec " << memVec << " -> memVec2 " << memVec2 << "\n";
    say << " from layout:\n" << a << "\n";
    tdd.verify(memVec[0] == memVec2[0]);
    tdd.verify(memVec[1] == memVec2[1]);
    tdd.verify(memVec[2] == memVec2[2]);

    say << "layout: " << a << "\n";
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::LayoutStructTester<3>> test;
}
