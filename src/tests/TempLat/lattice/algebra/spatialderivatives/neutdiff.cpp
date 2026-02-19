
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/spatialderivatives/neutdiff.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/util/ndloop.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{

  template <size_t NDim> struct NeutDiffTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> inline void NeutDiffTester<NDim>::Test(TDDAssertion &tdd)
  {
    const device::Idx nGrid = 8, nGhost = 1;

    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);
    SpatialCoordinate<NDim> x(toolBox);
    toolBox->setVerbose();

    // Get layout for computing global coordinates (for error reporting)
    auto layout = toolBox->mLayouts.getConfigSpaceLayout();

    // Test neutral difference in each dimension
    // NeutDiff<dir> computes: (f[idx + e_dir] - f[idx - e_dir]) / (2 * dx)
    constexpr_for<1, NDim + 1>([&](auto dirTag) {
      constexpr int dir = decltype(dirTag)::value;
      constexpr size_t d = static_cast<size_t>(dir) - 1;

      Field<NDim, double> sc("SC_" + std::to_string(d), toolBox);
      sc = x(dirTag);
      sc.updateGhosts();

      Field<NDim, double> ndsc("ndSC_" + std::to_string(d), toolBox);
      ndsc = neutDiff(sc, Tag<dir>{});

      bool OK = true;
      auto sc_view = sc.getFullNDHostView();
      auto ndsc_view = ndsc.getLocalNDHostView();

      // Use NDLoop to iterate
      // For neutral diff in dir: ndsc[idx] = (sc[idx+nGhost+e_dir] - sc[idx+nGhost-e_dir]) / (2*dx)
      NDLoop<NDim>(ndsc_view, [&](const auto &...indices) {
        // Build index arrays for accessing full view (with ghosts)
        device::IdxArray<NDim> idx_base = {(indices + nGhost)...};
        device::IdxArray<NDim> idx_next = idx_base;
        device::IdxArray<NDim> idx_prev = idx_base;
        idx_next[d] += 1;
        idx_prev[d] -= 1;

        // Get values
        const double val_next = device::apply([&](auto... i) { return sc_view(i...); }, idx_next);
        const double val_prev = device::apply([&](auto... i) { return sc_view(i...); }, idx_prev);
        const double val_ndsc = ndsc_view(indices...);

        const double dx = 1.0; // dx = 1 by default in these tests
        const double expect = (val_next - val_prev) / (2 * dx);

        if (std::abs(expect - val_ndsc) > 1e-14) {
          OK = false;

          // Compute global coordinates for error message
          device::IdxArray<NDim> global_idx;
          layout.putSpatialLocationFromMemoryIndexInto(global_idx, indices...);

          std::stringstream ss;
          ss << "NeutDiff mismatch in dir " << d << " at global (";
          for (size_t i = 0; i < NDim; ++i) {
            ss << global_idx[i];
            if (i < NDim - 1) ss << ", ";
          }
          ss << "): expect = " << expect << ", ndSC = " << val_ndsc << "\n";
          sayMPI << ss.str();
        }
      });
      tdd.verify(OK);
    });

    // Test 2: NeutDiff of quadratic function sc^2
    // For f(x) = x^2, neutral difference is ((x+1)^2 - (x-1)^2) / (2*dx) = 4x / (2*dx) = 2x / dx
    {
      Field<NDim, double> sc("SC_sq", toolBox);
      sc = x(1_c);
      sc.updateGhosts();

      Field<NDim, double> sc_sq("SC_sq_field", toolBox);
      sc_sq = sc * sc;
      sc_sq.updateGhosts();

      Field<NDim, double> ndsc_sq("ndSC_sq", toolBox);
      ndsc_sq = neutDiff(sc * sc, Tag<1>{});

      bool OK = true;
      auto sc_sq_view = sc_sq.getFullNDHostView();
      auto ndsc_sq_view = ndsc_sq.getLocalNDHostView();

      constexpr size_t d = 0;

      NDLoop<NDim>(ndsc_sq_view, [&](const auto &...indices) {
        device::IdxArray<NDim> idx_base = {(indices + nGhost)...};
        device::IdxArray<NDim> idx_next = idx_base;
        device::IdxArray<NDim> idx_prev = idx_base;
        idx_next[d] += 1;
        idx_prev[d] -= 1;

        const double val_next = device::apply([&](auto... i) { return sc_sq_view(i...); }, idx_next);
        const double val_prev = device::apply([&](auto... i) { return sc_sq_view(i...); }, idx_prev);
        const double val_ndsc_sq = ndsc_sq_view(indices...);

        const double dx = 1.0;
        const double expect = (val_next - val_prev) / (2 * dx);

        if (std::abs(expect - val_ndsc_sq) > 1e-14) {
          OK = false;

          device::IdxArray<NDim> global_idx;
          layout.putSpatialLocationFromMemoryIndexInto(global_idx, indices...);

          std::stringstream ss;
          ss << "NeutDiff sq mismatch at global (";
          for (size_t i = 0; i < NDim; ++i) {
            ss << global_idx[i];
            if (i < NDim - 1) ss << ", ";
          }
          ss << "): expect = " << expect << ", ndSC_sq = " << val_ndsc_sq << "\n";
          sayMPI << ss.str();
        }
      });
      tdd.verify(OK);
    }

    // Test 3: NeutDiff of product of coordinates (if NDim >= 2)
    if constexpr (NDim >= 2) {
      Field<NDim, double> sc1("SC1_prod", toolBox);
      sc1 = x(1_c);
      sc1.updateGhosts();

      Field<NDim, double> sc2("SC2_prod", toolBox);
      sc2 = x(2_c);
      sc2.updateGhosts();

      Field<NDim, double> sc_prod("SC_prod_field", toolBox);
      sc_prod = sc1 * sc2;
      sc_prod.updateGhosts();

      // Test neutral diff in direction 1 (index 0)
      Field<NDim, double> nd_prod_dir1("nd_prod_dir1", toolBox);
      nd_prod_dir1 = neutDiff(sc1 * sc2, Tag<1>{});

      bool OK = true;
      auto sc_prod_view = sc_prod.getFullNDHostView();
      auto nd_prod_dir1_view = nd_prod_dir1.getLocalNDHostView();

      constexpr size_t d = 0;

      NDLoop<NDim>(nd_prod_dir1_view, [&](const auto &...indices) {
        device::IdxArray<NDim> idx_base = {(indices + nGhost)...};
        device::IdxArray<NDim> idx_next = idx_base;
        device::IdxArray<NDim> idx_prev = idx_base;
        idx_next[d] += 1;
        idx_prev[d] -= 1;

        const double val_next = device::apply([&](auto... i) { return sc_prod_view(i...); }, idx_next);
        const double val_prev = device::apply([&](auto... i) { return sc_prod_view(i...); }, idx_prev);
        const double val_nd = nd_prod_dir1_view(indices...);

        const double dx = 1.0;
        const double expect = (val_next - val_prev) / (2 * dx);

        if (std::abs(expect - val_nd) > 1e-14) {
          OK = false;

          device::IdxArray<NDim> global_idx;
          layout.putSpatialLocationFromMemoryIndexInto(global_idx, indices...);

          std::stringstream ss;
          ss << "NeutDiff prod dir1 mismatch at global (";
          for (size_t i = 0; i < NDim; ++i) {
            ss << global_idx[i];
            if (i < NDim - 1) ss << ", ";
          }
          ss << "): expect = " << expect << ", nd = " << val_nd << "\n";
          sayMPI << ss.str();
        }
      });
      tdd.verify(OK);
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::NeutDiffTester<1>> test1;
  TempLat::TDDContainer<TempLat::NeutDiffTester<2>> test2;
  TempLat::TDDContainer<TempLat::NeutDiffTester<3>> test3;
  TempLat::TDDContainer<TempLat::NeutDiffTester<4>> test4;
  TempLat::TDDContainer<TempLat::NeutDiffTester<5>> test5;
} // namespace
