/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/util/ndloop.h"

namespace TempLat
{

  template <size_t NDim> struct LatticeLaplacianTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> inline void LatticeLaplacianTester<NDim>::Test(TDDAssertion &tdd)
  {
    const device::Idx nGrid = 4, nGhost = 1;

    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);
    SpatialCoordinate<NDim> coord(toolBox);
    toolBox->setVerbose();

    // Get layout for computing global coordinates (for error reporting)
    auto layout = toolBox->mLayouts.getConfigSpaceLayout();

    // Test 1: Laplacian of linear function sc1 = x[0]
    // Laplacian is computed as: sum_dir (f[i+e_dir] - 2*f[i] + f[i-e_dir])
    {
      Field<NDim, double> sc1("SC1", toolBox);
      sc1 = coord(1_c);
      sc1.updateGhosts();

      Field<NDim, double> result_field("result1", toolBox);
      result_field = LatLapl<NDim>(sc1);

      auto sc1_view = sc1.getFullNDHostView();
      auto result_view = result_field.getLocalNDHostView();

      bool OK = true;
      NDLoop<NDim>(result_view, [&](const auto &...indices) {
        device::IdxArray<NDim> idx_base = {(indices + nGhost)...};

        // Compute expected laplacian from actual field values
        double expected = 0.0;
        for (size_t dir = 0; dir < NDim; ++dir) {
          device::IdxArray<NDim> idx_plus = idx_base;
          device::IdxArray<NDim> idx_minus = idx_base;
          idx_plus[dir] += 1;
          idx_minus[dir] -= 1;

          const double f_plus = device::apply([&](auto... i) { return sc1_view(i...); }, idx_plus);
          const double f_center = device::apply([&](auto... i) { return sc1_view(i...); }, idx_base);
          const double f_minus = device::apply([&](auto... i) { return sc1_view(i...); }, idx_minus);
          expected += (f_plus - 2.0 * f_center + f_minus);
        }

        const double result_val = result_view(indices...);

        if (std::abs(result_val - expected) > 1e-10) {
          OK = false;
          device::IdxArray<NDim> global_idx;
          layout.putSpatialLocationFromMemoryIndexInto(global_idx, indices...);

          std::stringstream ss;
          ss << "Test1 fail at global (";
          for (size_t d = 0; d < NDim; ++d) {
            ss << global_idx[d];
            if (d < NDim - 1) ss << ", ";
          }
          ss << "): got " << result_val << ", expected " << expected << "\n";
          sayMPI << ss.str();
        }
      });
      tdd.verify(OK);
    }

    // Test 2: Laplacian of sc1^2 (quadratic in x[0])
    {
      Field<NDim, double> sc1("SC1_sq", toolBox);
      sc1 = coord(1_c);
      sc1.updateGhosts();

      Field<NDim, double> sc1_sq("SC1_sq_field", toolBox);
      sc1_sq = sc1 * sc1;
      sc1_sq.updateGhosts();

      Field<NDim, double> result_field("result2", toolBox);
      result_field = LatLapl<NDim>(sc1 * sc1);

      auto sc1_sq_view = sc1_sq.getFullNDHostView();
      auto result_view = result_field.getLocalNDHostView();

      bool OK = true;
      NDLoop<NDim>(result_view, [&](const auto &...indices) {
        device::IdxArray<NDim> idx_base = {(indices + nGhost)...};

        double expected = 0.0;
        for (size_t dir = 0; dir < NDim; ++dir) {
          device::IdxArray<NDim> idx_plus = idx_base;
          device::IdxArray<NDim> idx_minus = idx_base;
          idx_plus[dir] += 1;
          idx_minus[dir] -= 1;

          const double f_plus = device::apply([&](auto... i) { return sc1_sq_view(i...); }, idx_plus);
          const double f_center = device::apply([&](auto... i) { return sc1_sq_view(i...); }, idx_base);
          const double f_minus = device::apply([&](auto... i) { return sc1_sq_view(i...); }, idx_minus);
          expected += (f_plus - 2.0 * f_center + f_minus);
        }

        const double result_val = result_view(indices...);

        if (std::abs(result_val - expected) > 1e-10) {
          OK = false;
          device::IdxArray<NDim> global_idx;
          layout.putSpatialLocationFromMemoryIndexInto(global_idx, indices...);

          std::stringstream ss;
          ss << "Test2 fail at global (";
          for (size_t d = 0; d < NDim; ++d) {
            ss << global_idx[d];
            if (d < NDim - 1) ss << ", ";
          }
          ss << "): got " << result_val << ", expected " << expected << "\n";
          sayMPI << ss.str();
        }
      });
      tdd.verify(OK);
    }

    // Test 3: Laplacian of sc1 * sc2 (if NDim >= 2)
    if constexpr (NDim >= 2) {
      Field<NDim, double> sc1("SC1_prod", toolBox);
      sc1 = coord(1_c);
      sc1.updateGhosts();

      Field<NDim, double> sc2("SC2_prod", toolBox);
      sc2 = coord(2_c);
      sc2.updateGhosts();

      Field<NDim, double> sc1_sc2("SC1_SC2_field", toolBox);
      sc1_sc2 = sc1 * sc2;
      sc1_sc2.updateGhosts();

      Field<NDim, double> result_field("result3", toolBox);
      result_field = LatLapl<NDim>(sc1 * sc2);

      auto sc1_sc2_view = sc1_sc2.getFullNDHostView();
      auto result_view = result_field.getLocalNDHostView();

      bool OK = true;
      NDLoop<NDim>(result_view, [&](const auto &...indices) {
        device::IdxArray<NDim> idx_base = {(indices + nGhost)...};

        double expected = 0.0;
        for (size_t dir = 0; dir < NDim; ++dir) {
          device::IdxArray<NDim> idx_plus = idx_base;
          device::IdxArray<NDim> idx_minus = idx_base;
          idx_plus[dir] += 1;
          idx_minus[dir] -= 1;

          const double f_plus = device::apply([&](auto... i) { return sc1_sc2_view(i...); }, idx_plus);
          const double f_center = device::apply([&](auto... i) { return sc1_sc2_view(i...); }, idx_base);
          const double f_minus = device::apply([&](auto... i) { return sc1_sc2_view(i...); }, idx_minus);
          expected += (f_plus - 2.0 * f_center + f_minus);
        }

        const double result_val = result_view(indices...);

        if (std::abs(result_val - expected) > 1e-10) {
          OK = false;
          device::IdxArray<NDim> global_idx;
          layout.putSpatialLocationFromMemoryIndexInto(global_idx, indices...);

          std::stringstream ss;
          ss << "Test3 fail at global (";
          for (size_t d = 0; d < NDim; ++d) {
            ss << global_idx[d];
            if (d < NDim - 1) ss << ", ";
          }
          ss << "): got " << result_val << ", expected " << expected << "\n";
          sayMPI << ss.str();
        }
      });
      tdd.verify(OK);
    }

    // Test 4: Laplacian of (sc1 + sc2)^2 (if NDim >= 2)
    if constexpr (NDim >= 2) {
      Field<NDim, double> sc1("SC1_sum", toolBox);
      sc1 = coord(1_c);

      Field<NDim, double> sc2("SC2_sum", toolBox);
      sc2 = coord(2_c);

      Field<NDim, double> sc_sum_sq("SC_sum_sq_field", toolBox);
      sc_sum_sq = pow<2>(sc1 + sc2);
      sc_sum_sq.updateGhosts();

      Field<NDim, double> result_field("result4", toolBox);
      result_field = LatLapl<NDim>(pow<2>(sc1 + sc2));

      auto sc_sum_sq_view = sc_sum_sq.getFullNDHostView();
      auto result_view = result_field.getLocalNDHostView();

      bool OK = true;
      NDLoop<NDim>(result_view, [&](const auto &...indices) {
        device::IdxArray<NDim> idx_base = {(indices + nGhost)...};

        double expected = 0.0;
        for (size_t dir = 0; dir < NDim; ++dir) {
          device::IdxArray<NDim> idx_plus = idx_base;
          device::IdxArray<NDim> idx_minus = idx_base;
          idx_plus[dir] += 1;
          idx_minus[dir] -= 1;

          const double f_plus = device::apply([&](auto... i) { return sc_sum_sq_view(i...); }, idx_plus);
          const double f_center = device::apply([&](auto... i) { return sc_sum_sq_view(i...); }, idx_base);
          const double f_minus = device::apply([&](auto... i) { return sc_sum_sq_view(i...); }, idx_minus);
          expected += (f_plus - 2.0 * f_center + f_minus);
        }

        const double result_val = result_view(indices...);

        if (std::abs(result_val - expected) > 1e-10) {
          OK = false;
          device::IdxArray<NDim> global_idx;
          layout.putSpatialLocationFromMemoryIndexInto(global_idx, indices...);

          std::stringstream ss;
          ss << "Test4 fail at global (";
          for (size_t d = 0; d < NDim; ++d) {
            ss << global_idx[d];
            if (d < NDim - 1) ss << ", ";
          }
          ss << "): got " << result_val << ", expected " << expected << "\n";
          sayMPI << ss.str();
        }
      });
      tdd.verify(OK);
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::LatticeLaplacianTester<1>> test1;
  // TODO 2-Dim fails due to fourier transform - FIND OUT WHY
  TempLat::TDDContainer<TempLat::LatticeLaplacianTester<2>> test2;
  TempLat::TDDContainer<TempLat::LatticeLaplacianTester<3>> test3;
  TempLat::TDDContainer<TempLat::LatticeLaplacianTester<4>> test4;
  TempLat::TDDContainer<TempLat::LatticeLaplacianTester<5>> test5;
} // namespace
