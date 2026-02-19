
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/listoperators/vectordotter.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/util/ndloop.h"
#include "TempLat/util/constexpr_for.h"

namespace TempLat
{

  template <size_t NDim> struct VectorDotterTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> inline void VectorDotterTester<NDim>::Test(TDDAssertion &tdd)
  {
    // Test 1: verify dot(x, x) using DoEval::eval
    {
      const ptrdiff_t nGrid = 32, nGhost = 2;

      auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

      SpatialCoordinate<NDim> x(toolBox);

      Field<NDim, double> fieldX("fieldX", toolBox);

      fieldX = dot(x, x);

      const auto fieldX_view = fieldX.getLocalNDHostView();

      bool correct = true;
      NDLoop<NDim>(fieldX_view, [&](const auto... idx) {
        const device::IdxArray<NDim> localIdx{idx...};
        device::IdxArray<NDim> ghostIdx;
        for (size_t d = 0; d < NDim; ++d)
          ghostIdx[d] = nGhost + localIdx[d];

        const auto expected = device::apply([&](const auto &...gi) { return DoEval::eval(x, gi...); }, ghostIdx);

        double expectedDot = 0;
        for (size_t d = 0; d < NDim; ++d)
          expectedDot += static_cast<double>(expected[d]) * static_cast<double>(expected[d]);

        correct &= (fieldX_view(idx...) == expectedDot);
        if (!(fieldX_view(idx...) == expectedDot)) {
          std::cout << "Mismatch at " << localIdx << ": fieldX_view = " << fieldX_view(idx...)
                    << ", expected = " << expectedDot << std::endl;
        }
      });

      tdd.verify(correct);
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::VectorDotterTester<1>> test1;
  TempLat::TDDContainer<TempLat::VectorDotterTester<2>> test2;
  TempLat::TDDContainer<TempLat::VectorDotterTester<3>> test3;
  TempLat::TDDContainer<TempLat::VectorDotterTester<4>> test4;
  TempLat::TDDContainer<TempLat::VectorDotterTester<5>> test5;
} // namespace
