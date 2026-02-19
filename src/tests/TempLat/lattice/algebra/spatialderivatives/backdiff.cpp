
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025
#include "TempLat/lattice/algebra/spatialderivatives/backdiff.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/util/ndloop.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{

  template <size_t NDim> struct BackDiffTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> inline void BackDiffTester<NDim>::Test(TDDAssertion &tdd)
  {
    const device::Idx nGrid = 8, nGhost = 1;

    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);
    SpatialCoordinate<NDim> x(toolBox);
    // toolBox->setVerbose();

    // Get layout for computing global coordinates (for error reporting)
    auto layout = toolBox->mLayouts.getConfigSpaceLayout();

    // Test backward difference in each dimension
    // BackDiff<dir> computes: (f[idx] - f[idx - e_dir]) / dx
    // Use runtime loop like latticeforwardgradient_test
    auto testDir = [&](auto dirTag) {
      constexpr int dir = decltype(dirTag)::value;
      constexpr size_t d = static_cast<size_t>(dir) - 1;

      Field<NDim, double> sc("SC_" + std::to_string(d), toolBox);
      sc = x(dirTag);
      sc.updateGhosts();

      Field<NDim, double> bdsc("bdSC_" + std::to_string(d), toolBox);
      bdsc = backDiff(sc, Tag<dir>{});

      bool OK = true;
      auto sc_view = sc.getFullNDHostView();
      auto bdsc_view = bdsc.getLocalNDHostView();

      // Use NDLoop to iterate
      // For backward diff in dir: bdsc[idx] = (sc[idx+nGhost] - sc[idx+nGhost-e_dir]) / dx
      NDLoop<NDim>(bdsc_view, [&](const auto &...indices) {
        // Build index arrays for accessing full view (with ghosts)
        device::IdxArray<NDim> idx_base = {(indices + nGhost)...};
        device::IdxArray<NDim> idx_prev = idx_base;
        idx_prev[d] -= 1;

        // Get values
        const double val_current = device::apply([&](auto... i) { return sc_view(i...); }, idx_base);
        const double val_prev = device::apply([&](auto... i) { return sc_view(i...); }, idx_prev);
        const double val_bdsc = bdsc_view(indices...);

        const double dx = 1.0; // dx = 1 by default in these tests
        const double expect = (val_current - val_prev) / dx;

        if (std::abs(expect - val_bdsc) > 1e-14) {
          OK = false;

          // Compute global coordinates for error message
          device::IdxArray<NDim> global_idx;
          layout.putSpatialLocationFromMemoryIndexInto(global_idx, indices...);

          std::stringstream ss;
          ss << "BackDiff mismatch in dir " << d << " at global (";
          for (size_t i = 0; i < NDim; ++i) {
            ss << global_idx[i];
            if (i < NDim - 1) ss << ", ";
          }
          ss << "): expect = " << expect << ", bdSC = " << val_bdsc << "\n";
          sayMPI << ss.str();
        }
      });
      tdd.verify(OK);
    };

    // Test all directions using constexpr_for
    constexpr_for<1, NDim + 1>([&](auto _dir) { testDir(Tag<decltype(_dir)::value>{}); });

    // Test 2: BackDiff of quadratic function sc^2
    // For f(x) = x^2, backward difference is (x^2 - (x-1)^2) = 2x - 1
    {
      Field<NDim, double> sc("SC_sq", toolBox);
      sc = x(1_c);
      sc.updateGhosts();

      Field<NDim, double> sc_sq("SC_sq_field", toolBox);
      sc_sq = sc * sc;
      sc_sq.updateGhosts();

      Field<NDim, double> bdsc_sq("bdSC_sq", toolBox);
      bdsc_sq = backDiff(sc * sc, Tag<1>{});

      bool OK = true;
      auto sc_sq_view = sc_sq.getFullNDHostView();
      auto bdsc_sq_view = bdsc_sq.getLocalNDHostView();

      constexpr size_t d = 0;

      NDLoop<NDim>(bdsc_sq_view, [&](const auto &...indices) {
        device::IdxArray<NDim> idx_base = {(indices + nGhost)...};
        device::IdxArray<NDim> idx_prev = idx_base;
        idx_prev[d] -= 1;

        const double val_current = device::apply([&](auto... i) { return sc_sq_view(i...); }, idx_base);
        const double val_prev = device::apply([&](auto... i) { return sc_sq_view(i...); }, idx_prev);
        const double val_bdsc_sq = bdsc_sq_view(indices...);

        const double dx = 1.0;
        const double expect = (val_current - val_prev) / dx;

        if (std::abs(expect - val_bdsc_sq) > 1e-14) {
          OK = false;

          device::IdxArray<NDim> global_idx;
          layout.putSpatialLocationFromMemoryIndexInto(global_idx, indices...);

          std::stringstream ss;
          ss << "BackDiff sq mismatch at global (";
          for (size_t i = 0; i < NDim; ++i) {
            ss << global_idx[i];
            if (i < NDim - 1) ss << ", ";
          }
          ss << "): expect = " << expect << ", bdSC_sq = " << val_bdsc_sq << "\n";
          sayMPI << ss.str();
        }
      });
      tdd.verify(OK);
    }

    // Test 3: BackDiff of product of coordinates (if NDim >= 2)
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

      // Test backward diff in direction 1 (index 0)
      Field<NDim, double> bd_prod_dir1("bd_prod_dir1", toolBox);
      bd_prod_dir1 = backDiff(sc1 * sc2, Tag<1>{});

      bool OK = true;
      auto sc_prod_view = sc_prod.getFullNDHostView();
      auto bd_prod_dir1_view = bd_prod_dir1.getLocalNDHostView();

      constexpr size_t d = 0;

      NDLoop<NDim>(bd_prod_dir1_view, [&](const auto &...indices) {
        device::IdxArray<NDim> idx_base = {(indices + nGhost)...};
        device::IdxArray<NDim> idx_prev = idx_base;
        idx_prev[d] -= 1;

        const double val_current = device::apply([&](auto... i) { return sc_prod_view(i...); }, idx_base);
        const double val_prev = device::apply([&](auto... i) { return sc_prod_view(i...); }, idx_prev);
        const double val_bd = bd_prod_dir1_view(indices...);

        const double dx = 1.0;
        const double expect = (val_current - val_prev) / dx;

        if (std::abs(expect - val_bd) > 1e-14) {
          OK = false;

          device::IdxArray<NDim> global_idx;
          layout.putSpatialLocationFromMemoryIndexInto(global_idx, indices...);

          std::stringstream ss;
          ss << "BackDiff prod dir1 mismatch at global (";
          for (size_t i = 0; i < NDim; ++i) {
            ss << global_idx[i];
            if (i < NDim - 1) ss << ", ";
          }
          ss << "): expect = " << expect << ", bd = " << val_bd << "\n";
          sayMPI << ss.str();
        }
      });
      tdd.verify(OK);
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::BackDiffTester<1>> test1;
  TempLat::TDDContainer<TempLat::BackDiffTester<2>> test2;
  TempLat::TDDContainer<TempLat::BackDiffTester<3>> test3;
  TempLat::TDDContainer<TempLat::BackDiffTester<4>> test4;
  TempLat::TDDContainer<TempLat::BackDiffTester<5>> test5;
} // namespace
