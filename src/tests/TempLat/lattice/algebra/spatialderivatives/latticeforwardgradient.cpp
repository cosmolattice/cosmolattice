/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/lattice/algebra/spatialderivatives/latticeforwardgradient.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/util/ndloop.h"

namespace TempLat
{

  template <size_t NDim> struct LatticeForwardGradientTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> inline void LatticeForwardGradientTester<NDim>::Test(TDDAssertion &tdd)
  {
    const device::Idx nGrid = 8, nGhost = 1;

    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);
    SpatialCoordinate<NDim> x(toolBox);
    toolBox->setVerbose();

    // Get layout for computing global coordinates (for error reporting)
    auto layout = toolBox->mLayouts.getConfigSpaceLayout();

    // Test forward gradient in each dimension
    constexpr_for<1, NDim + 1>([&](auto dirTag) {
      constexpr int dir = decltype(dirTag)::value;
      constexpr size_t d = static_cast<size_t>(dir) - 1;

      Field<NDim, double> sc("SC_" + std::to_string(dir), toolBox);
      sc = x(dirTag);
      sc.updateGhosts();

      Field<NDim, double> fgsc("fgSC_" + std::to_string(dir), toolBox);
      fgsc = getVectorComponent(LatForwardGrad<NDim>(sc), dirTag - Tag<1>{});

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
        idx_next[d] += 1;

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

          std::stringstream ss;
          ss << "Mismatch in dir " << dir << " at global (";
          for (size_t i = 0; i < NDim; ++i) {
            ss << global_idx[i];
            if (i < NDim - 1) ss << ", ";
          }
          ss << "): expect = " << expect << ", fgSC = " << val_fgsc << "\n";
          sayMPI << ss.str();
        }
      });
      tdd.verify(OK);
    });
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::LatticeForwardGradientTester<1>> test1;
  TempLat::TDDContainer<TempLat::LatticeForwardGradientTester<2>> test2;
  TempLat::TDDContainer<TempLat::LatticeForwardGradientTester<3>> test3;
  TempLat::TDDContainer<TempLat::LatticeForwardGradientTester<4>> test4;
  TempLat::TDDContainer<TempLat::LatticeForwardGradientTester<5>> test5;
} // namespace
