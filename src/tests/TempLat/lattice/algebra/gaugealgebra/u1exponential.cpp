
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/gaugealgebra/u1exponential.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/ndloop.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldsubtract.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/algebra/operators/operators.h"

namespace TempLat
{

  struct U1ExponentialTester {
    static void Test(TDDAssertion &tdd);
  };

  void U1ExponentialTester::Test(TDDAssertion &tdd)
  {
    static constexpr size_t NDim = 3;
    using T = double;
    const ptrdiff_t nGrid = 16, nGhost = 2;

    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

    Field<NDim, T> psi("psi", toolBox);
    SpatialCoordinate x(toolBox);
    psi = getVectorComponent(x, Tag<0>{});
    psi.updateGhosts();

    auto U1 = complexPhase(psi);

    ComplexField<NDim, T> expected("expected", toolBox);
    expected.ComplexFieldGet(Tag<0>()) = cos(psi);
    expected.ComplexFieldGet(Tag<1>()) = sin(psi);
    expected.updateGhosts();

    ComplexField<NDim, T> diff("diff", toolBox);
    diff = U1;
    diff.updateGhosts();

    auto view1Diff = diff.ComplexFieldGet(Tag<0>()).getLocalNDHostView();
    auto view2Diff = diff.ComplexFieldGet(Tag<1>()).getLocalNDHostView();

    auto view1expected = expected.ComplexFieldGet(Tag<0>()).getLocalNDHostView();
    auto view2expected = expected.ComplexFieldGet(Tag<1>()).getLocalNDHostView();

    bool allCorrect = true;
    NDLoop<NDim>(view1Diff, [&](const auto &...idx) {
      allCorrect = allCorrect && AlmostEqual(view1Diff(idx...), view1expected(idx...));
      allCorrect = allCorrect && AlmostEqual(view2Diff(idx...), view2expected(idx...));
      if (!AlmostEqual(view1Diff(idx...), view1expected(idx...)) ||
          !AlmostEqual(view2Diff(idx...), view2expected(idx...))) {
        std::cout << "U1 exponential test failed at index (";
        ((std::cout << idx << ", "), ...);
        std::cout << ") got (" << view1Diff(idx...) << ", " << view2Diff(idx...) << ")\n";
      }
    });
    tdd.verify(allCorrect);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::U1ExponentialTester> test;
}
