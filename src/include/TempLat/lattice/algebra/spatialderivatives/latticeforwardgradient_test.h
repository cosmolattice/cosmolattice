#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWARDGRADIENTLOCAL_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWARDGRADIENTLOCAL_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler  Year: 2025

#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/util/ndloop.h"

template <size_t NDim> inline void TempLat::LatticeForwardGradientTester<NDim>::Test(TempLat::TDDAssertion &tdd)
{
  const device::Idx nGrid = 8, nGhost = 1;

  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);
  SpatialCoordinate<NDim> x(toolBox);
  toolBox->setVerbose();

  // Get layout for computing global coordinates (for error reporting)
  auto layout = toolBox->mLayouts.getConfigSpaceLayout();

  // Test forward gradient in each dimension
  for (size_t dir = 0; dir < NDim; ++dir) {
    Field<NDim, double> sc("SC_" + std::to_string(dir), toolBox);
    sc = getVectorComponent(x, dir);
    sc.updateGhosts();

    Field<NDim, double> fgsc("fgSC_" + std::to_string(dir), toolBox);
    fgsc = getVectorComponent(LatForwardGrad<NDim>(sc), dir);

    bool OK = true;
    auto sc_view = sc.getFullNDHostView();
    auto fgsc_view = fgsc.getLocalNDHostView();

    // Use NDLoop to iterate, but construct index arrays to access views
    // For forward gradient in dir: fgsc[idx] = sc[idx+nGhost+e_dir] - sc[idx+nGhost]
    // where e_dir is unit vector in direction dir
    NDLoop<NDim>(fgsc_view, [&](const auto &...indices) {
      // Build index arrays for accessing full view (with ghosts)
      device::IdxArray<NDim> idx_base = {(indices + nGhost)...};
      device::IdxArray<NDim> idx_next = idx_base;
      idx_next[dir] += 1;

      // Get values
      double val_current = device::apply([&](auto... i) { return sc_view(i...); }, idx_base);
      double val_next = device::apply([&](auto... i) { return sc_view(i...); }, idx_next);
      double val_fgsc = fgsc_view(indices...);

      const double expect = val_next - val_current;

      if (std::abs(expect - val_fgsc) > 1e-14) {
        OK = false;

        // Compute global coordinates for error message
        device::IdxArray<NDim> global_idx;
        layout.putSpatialLocationFromMemoryIndexInto(global_idx, indices...);

        sayMPI << "Mismatch in dir " << dir << " at global (";
        for (size_t d = 0; d < NDim; ++d) {
          sayMPI << global_idx[d];
          if (d < NDim - 1) sayMPI << ", ";
        }
        sayMPI << "): expect = " << expect << ", fgSC = " << val_fgsc << "\n";
      }
    });
    tdd.verify(OK);
  }
}

#endif
