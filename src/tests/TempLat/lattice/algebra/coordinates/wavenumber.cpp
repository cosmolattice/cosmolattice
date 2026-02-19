/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/coordinates/wavenumber.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/util/log/saycomplete.h"
#include "TempLat/util/ndloop.h"

namespace TempLat
{

  template <size_t NDim> struct WaveNumberTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> inline void WaveNumberTester<NDim>::Test(TDDAssertion &tdd)
  {
    const ptrdiff_t nGrid = 32, nGhost = 0;

    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

    // Create fields for each wavenumber component
    std::vector<Field<NDim, double>> phi_components;
    phi_components.reserve(NDim);
    for (size_t d = 0; d < NDim; ++d) {
      phi_components.emplace_back("phi_" + std::to_string(d), toolBox);
    }

    Field<NDim, double> phinorm("phinorm", toolBox);
    Field<NDim, double> phinorm2("phinorm2", toolBox);

    WaveNumber<NDim> k(toolBox);

    // Assign wavenumber components to fields
    constexpr_for<0, NDim>([&](auto d) { phi_components[d].inFourierSpace() = k(d + Tag<1>()); });
    phinorm.inFourierSpace() = k.norm();
    phinorm2.inFourierSpace() = k.norm2();

    // Get host views for all component fields
    std::vector<decltype(phi_components[0].inFourierSpace().getLocalNDHostView())> phi_views;
    phi_views.reserve(NDim);
    for (size_t d = 0; d < NDim; ++d) {
      phi_views.push_back(phi_components[d].inFourierSpace().getLocalNDHostView());
    }
    auto phinorm_view = phinorm.inFourierSpace().getLocalNDHostView();
    auto phinorm2_view = phinorm2.inFourierSpace().getLocalNDHostView();

    // Get Fourier space layout for MPI support (handles transposition)
    auto layout = toolBox->mLayouts.getFourierSpaceLayout();

    // Check that the fourier coordinate is correct
    bool correct = true;

    NDLoop<NDim>(phinorm_view, [&](const auto &...indices) {
      device::IdxArray<NDim> local_idx{indices...};

      // Use layout to compute global spatial coordinates from local memory indices
      // This correctly handles transposition in MPI distributed FFTs
      device::IdxArray<NDim> global_idx;
      layout.putSpatialLocationFromMemoryIndexInto(global_idx, indices...);

      bool this_correct = true;

      // Check each wavenumber component - the value should match the global spatial coordinate
      double norm2 = 0.0;
      for (size_t d = 0; d < NDim; ++d) {
        const double expected_val = static_cast<double>(global_idx[d]);
        norm2 += expected_val * expected_val;

        this_correct &= AlmostEqual(phi_views[d](indices...).real(), expected_val);
        this_correct &= AlmostEqual(phi_views[d](indices...).imag(), 0.);
      }

      std::stringstream ss;
      // Check norm2 and norm
      this_correct &= AlmostEqual(phinorm2_view(indices...).real(), norm2);
      ss << "Fail after 1, should: " << norm2 << " is " << phinorm2_view(indices...).real() << "\n";
      this_correct &= AlmostEqual(phinorm2_view(indices...).imag(), 0.);
      ss << "Fail after 2, should: " << 0. << " is " << phinorm2_view(indices...).imag() << "\n";
      this_correct &= AlmostEqual(phinorm_view(indices...).real(), sqrt(norm2));
      ss << "Fail after 3, should: " << sqrt(norm2) << " is " << phinorm_view(indices...).real() << "\n";
      this_correct &= AlmostEqual(phinorm_view(indices...).imag(), 0.);
      ss << "Fail after 4, should: " << 0. << " is " << phinorm_view(indices...).imag() << "\n";

      correct &= this_correct;

      if (!this_correct) {
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
        ss << ", phinorm = " << phinorm_view(indices...) << ", phinorm2 = " << phinorm2_view(indices...) << "\n";
        sayMPI << ss.str();
      }
    });

    tdd.verify(correct);
  }

} // namespace TempLat

namespace
{
  // TODO: FFT FAILURES ON D=1 !!! (MPI related.)
  // TempLat::TDDContainer<TempLat::WaveNumberTester<1>> test1;
  TempLat::TDDContainer<TempLat::WaveNumberTester<2>> test2;
  TempLat::TDDContainer<TempLat::WaveNumberTester<3>> test3;
  TempLat::TDDContainer<TempLat::WaveNumberTester<4>> test4;
  TempLat::TDDContainer<TempLat::WaveNumberTester<5>> test5;
} // namespace
