/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"
#include "TempLat/util/log/saycomplete.h"
#include "TempLat/util/ndloop.h"

namespace TempLat
{

  template <size_t NDim> struct SpatialCoordinateTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> inline void SpatialCoordinateTester<NDim>::Test(TDDAssertion &tdd)
  {
    const ptrdiff_t nGrid = 16, nGhost = 2;

    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

    // Create fields for each spatial coordinate component
    std::vector<Field<NDim, double>> phi_components;
    phi_components.reserve(NDim);
    for (size_t d = 0; d < NDim; ++d) {
      phi_components.emplace_back("phi_" + std::to_string(d), toolBox);
    }

    SpatialCoordinate<NDim> x(toolBox);

    // Assign spatial coordinate components to fields
    constexpr_for<0, NDim>([&](auto d) { phi_components[d] = getVectorComponent(x, d); });

    // Get host views for all component fields
    std::vector<decltype(phi_components[0].getLocalNDHostView())> phi_views;
    phi_views.reserve(NDim);
    for (size_t d = 0; d < NDim; ++d) {
      phi_views.push_back(phi_components[d].getLocalNDHostView());
    }

    auto layout = toolBox->mLayouts.getConfigSpaceLayout();

    // Check that the spatial coordinate is correct
    bool correct = true;

    NDLoop<NDim>(phi_views[0], [&](const auto &...indices) {
      device::IdxArray<NDim> local_idx = {static_cast<device::Idx>(indices)...};

      // Build index array with ghost offset for memory layout functions and vectorGet
      device::IdxArray<NDim> idx_with_ghosts = {(indices + nGhost)...};

      // Use layout to compute global spatial coordinates from memory indices (including ghost offset)
      device::IdxArray<NDim> global_idx;
      device::apply([&](auto... i) { layout.putSpatialLocationFromMemoryIndexInto(global_idx, i...); },
                    idx_with_ghosts);

      bool this_correct = true;

      for (size_t d = 0; d < NDim; ++d) {
        const double expected_val = static_cast<double>(global_idx[d]);

        this_correct &= (phi_views[d](indices...) == expected_val);
        this_correct &=
            (device::apply([&](auto... i) { return DoEval::eval(x, i...)[d]; }, idx_with_ghosts) == expected_val);
      }

      correct &= this_correct;

      if (!this_correct) {
        std::stringstream ss;
        ss << "Error at local (";
        for (size_t d = 0; d < NDim; ++d) {
          ss << local_idx[d];
          if (d < NDim - 1) ss << ", ";
        }
        ss << "), global (";
        for (size_t d = 0; d < NDim; ++d) {
          ss << global_idx[d];
          if (d < NDim - 1) ss << ", ";
        }
        ss << "): ";
        for (size_t d = 0; d < NDim; ++d) {
          ss << "phi[" << d << "] = " << phi_views[d](indices...);
          if (d < NDim - 1) ss << ", ";
        }
        ss << "\n";
        sayMPI << ss.str();
      }
    });

    tdd.verify(correct);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SpatialCoordinateTester<1>> test1;
  TempLat::TDDContainer<TempLat::SpatialCoordinateTester<2>> test2;
  TempLat::TDDContainer<TempLat::SpatialCoordinateTester<3>> test3;
  TempLat::TDDContainer<TempLat::SpatialCoordinateTester<4>> test4;
  TempLat::TDDContainer<TempLat::SpatialCoordinateTester<5>> test5;
} // namespace
