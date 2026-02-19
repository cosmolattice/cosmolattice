
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/su2algebra/su2shift.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/ndloop.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"

namespace TempLat
{

  struct SU2ShiftTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2ShiftTester::Test(TDDAssertion &tdd)
  {
    constexpr size_t NDim = 3;
    const ptrdiff_t nGrid = 8, nGhost = 1;
    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

    SpatialCoordinate<NDim> x(toolBox);

    // Create an SU2Field and set each component to a spatial coordinate direction.
    // Component 1 -> x_0, Component 2 -> x_1, Component 3 -> x_2
    SU2Field<NDim, double> su2("su2", toolBox, LatticeParameters<double>());
    su2(1_c) = x(1_c);
    su2(2_c) = x(2_c);
    su2(3_c) = x(3_c);

    // Test SU2ShifterByOne: shift the SU2 field along each spatial direction
    // and verify that each component is shifted correctly.
    {
      constexpr_for<0, NDim>([&](auto _dir) {
        constexpr size_t dir = decltype(_dir)::value;

        // For each SU2 component (1_c, 2_c, 3_c):
        //   If the component index matches the shift direction, the difference should be 1
        //   (or -(nGrid-1) at the periodic boundary).
        //   Otherwise, the difference should be 0.
        constexpr_for<0, 3>([&](auto _comp) {
          constexpr size_t comp = decltype(_comp)::value;

          Field<NDim, double> result("result", toolBox);
          result = shift(su2, _dir + Tag<1>()).SU2Get(Tag<comp + 1>()) - su2.SU2Get(Tag<comp + 1>());
          auto view = result.getLocalNDHostView();

          bool all_right = true;
          NDLoop<NDim>(view, [&](const auto... idx) {
            if (comp == dir) {
              bool local = (view(idx...) == 1) || (view(idx...) == -(nGrid - 1));
              all_right &= local;
              if (!local)
                std::cout << "SU2 comp " << comp + 1 << " dir " << dir << " val at " << device::IdxArray<NDim>{idx...}
                          << " is " << view(idx...) << ", should be 1" << std::endl;
            } else {
              bool local = (view(idx...) == 0);
              all_right &= local;
              if (!local)
                std::cout << "SU2 comp " << comp + 1 << " dir " << dir << " val at " << device::IdxArray<NDim>{idx...}
                          << " is " << view(idx...) << ", should be 0" << std::endl;
            }
          });
          tdd.verify(all_right);
        });
      });
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2ShiftTester> test;
}
