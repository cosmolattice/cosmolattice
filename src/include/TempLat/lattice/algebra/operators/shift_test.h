#ifndef TEMPLAT_LATTICE_ALGEBRA_SHIFT_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_SHIFT_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien,  Year: 2019

#include "TempLat/util/ndloop.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/algebra/operators/subtract.h"

template <size_t NDim> inline void TempLat::ExpressionShifterTester<NDim>::Test(TempLat::TDDAssertion &tdd)
{
  const ptrdiff_t nGrid = 8, nGhost = 1;
  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

  SpatialCoordinate<NDim> x(toolBox);

  // Create fields for each wavenumber component
  std::vector<Field<NDim, double>> phi_components;
  phi_components.reserve(NDim);
  for (size_t d = 0; d < NDim; ++d) {
    phi_components.emplace_back("phi_" + std::to_string(d), toolBox);
  }

  // Set the fields to the components of x
  for (size_t d = 0; d < NDim; ++d) {
    phi_components[d] = getVectorComponent(x, d);
  }
  // ExpressionShifter
  {
  }
  // ExpressionShifterByOne
  {
    constexpr_for<0, NDim>([&](auto _i) {
      constexpr size_t i = decltype(_i)::value;
      constexpr_for<0, NDim>([&](auto _j) {
        constexpr size_t j = decltype(_j)::value;
        Field<NDim, double> result("result", toolBox);
        result = shift(phi_components[i], _j + Tag<1>()) - phi_components[i];
        auto view = result.getLocalNDHostView();
        bool all_right = true;
        NDLoop<NDim>(view, [&](const auto... idx) {
          if (i == j) {
            bool local = (view(idx...) == 1) || (view(idx...) == -(nGrid - 1));
            all_right &= local;
            if (!local)
              std::cout << "val at " << device::IdxArray<NDim>{idx...} << " is " << view(idx...) << ", should be "
                        << "1" << std::endl;
          } else {
            bool local = (view(idx...) == 0);
            all_right &= local;
            if (!local)
              std::cout << "val at " << device::IdxArray<NDim>{idx...} << " is " << view(idx...) << ", should be "
                        << "0" << std::endl;
          }
        });
        tdd.verify(all_right);
      });
    });
  }
}

#endif
