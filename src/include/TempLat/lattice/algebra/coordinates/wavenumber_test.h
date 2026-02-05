#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/util/log/saycomplete.h"

#include "TempLat/util/ndloop.h"

#include <vector>

template <size_t NDim> inline void TempLat::WaveNumber<NDim>::Test(TempLat::TDDAssertion &tdd)
{
  const ptrdiff_t nGrid = 16, nGhost = 0;

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
  for (size_t d = 0; d < NDim; ++d) {
    phi_components[d].inFourierSpace() = getVectorComponent(k, d);
  }
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

  // Check that the fourier coordinate is correct
  bool correct = true;

  // Helper to compute expected wavenumber value for a given index in a given dimension
  auto expectedWaveNumber = [nGrid](ptrdiff_t idx, size_t dim, size_t totalDims) -> double {
    // Last dimension in Fourier space is truncated (real-to-complex FFT)
    if (dim == totalDims - 1) {
      // For the last dimension, index is always in range [0, nGrid/2]
      return static_cast<double>(idx);
    }
    // For other dimensions, wrap around at nGrid/2
    return idx > nGrid / 2 ? static_cast<double>(idx - nGrid) : static_cast<double>(idx);
  };

  NDLoop<NDim>(phinorm_view, [&](const auto &...indices) {
    std::array<ptrdiff_t, NDim> idx_arr = {static_cast<ptrdiff_t>(indices)...};

    bool this_correct = true;

    // Check each wavenumber component
    double norm2 = 0.0;
    for (size_t d = 0; d < NDim; ++d) {
      const double expected_val = expectedWaveNumber(idx_arr[d], d, NDim);
      norm2 += expected_val * expected_val;

      this_correct &= AlmostEqual(phi_views[d](indices...).real(), expected_val);
      this_correct &= AlmostEqual(phi_views[d](indices...).imag(), 0.);
    }

    // Check norm2 and norm
    this_correct &= AlmostEqual(phinorm2_view(indices...).real(), norm2);
    this_correct &= AlmostEqual(phinorm2_view(indices...).imag(), 0.);
    this_correct &= AlmostEqual(phinorm_view(indices...).real(), sqrt(norm2));
    this_correct &= AlmostEqual(phinorm_view(indices...).imag(), 0.);

    correct &= this_correct;

    if (!this_correct) {
      sayShort << "Error at (";
      for (size_t d = 0; d < NDim; ++d) {
        sayShort << idx_arr[d];
        if (d < NDim - 1) sayShort << ", ";
      }
      sayShort << "): ";
      for (size_t d = 0; d < NDim; ++d) {
        sayShort << "phi[" << d << "] = " << phi_views[d](indices...);
        if (d < NDim - 1) sayShort << ", ";
      }
      sayShort << ", phinorm = " << phinorm_view(indices...) << ", phinorm2 = " << phinorm2_view(indices...) << "\n";
    }
  });

  tdd.verify(correct);

  // WaveNumber cn;
  //
  ///* should not be happy with configuration space. */
  // tdd.verify( Throws<DimensionCountRecorder_CoordinateSpaceException>(
  //        [&]() {
  //            cn.getNorm().confirmSpace(LayoutStruct(1), SpaceStateType::Configuration);
  //        }
  //) );

  ///* should be happy with fourier space. */
  // tdd.verify( DoesNotThrow<DimensionCountRecorder_CoordinateSpaceException>(
  //        [&]() {
  //            cn.getNorm().confirmSpace(LayoutStruct(1), SpaceStateType::Fourier);
  //        }
  //) );

  /*  constexpr size_t NDim = 2;
    ptrdiff_t nGrid = 8, nGhost = 2;

    auto toolbox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

    Field<NDim, double> field("field", toolbox);

    WaveNumber k(toolbox);
    field.inFourierSpace() = k.norm2(); // * RandomGaussianField<NDim, T>("Hoi", toolBox);

    auto field_view = field.getLocalNDHostView();

    for (uint i = 0; i < nGrid; ++i) {
      for (uint j = 0; j < nGrid; ++j) {
        const ptrdiff_t x_val = i > nGrid / 2 ? i - nGrid : i;
        const ptrdiff_t y_val = j > nGrid / 2 ? j - nGrid : j;

        tdd.verify(k.get(i, j, 0) == x_val);
        tdd.verify(k.get(i, j, 1) == y_val);

        if (!(k.get(i, j, 1) == y_val))
          say << "k.get(" << i << ", " << j << ", 1) = " << k.get(i, j, 1) << " != " << y_val;
      }
    }

    // auto kn = k.norm2() * pow<2>(0.685);
    //  auto knkn = kn * kn;
    //  say << knkn.get(1, 2, 3);
    //  auto kn1 = k.norm() * pow<2>(0.685);
    //   say<<knkn.get(45);
    //  auto kn1kn1 = kn1 * kn1;
    //  say<<kn1.get(45);
    //  say<<kn1kn1.get(45);

    tdd.verify(true); // Test in power spectrum.
  */
}

#endif
