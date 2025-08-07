#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_WAVENUMBER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/util/log/saycomplete.h"

inline void TempLat::WaveNumberTester::Test(TempLat::TDDAssertion &tdd)
{
  static constexpr size_t NDim = 2;
  ptrdiff_t nGrid = 8, nGhost = 0;

  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

  Field<NDim, double> phix("phix", toolBox);
  Field<NDim, double> phiy("phiy", toolBox);

  WaveNumber<NDim> x(toolBox);
  phix.inFourierSpace() = getVectorComponent(x, 0);
  phiy.inFourierSpace() = getVectorComponent(x, 1);

  auto phix_view = phix.inFourierSpace().getLocalNDHostView();
  auto phiy_view = phiy.inFourierSpace().getLocalNDHostView();

  // Check that the fourier coordinate is correct
  bool correct = true;
  for (ptrdiff_t i = 0; i < nGrid; ++i) {
    for (ptrdiff_t j = 0; j < nGrid / 2 + 1; ++j) {
      const ptrdiff_t x_val = i > nGrid / 2 ? i - nGrid : i;
      correct &= phix_view(i, j) == x_val;
      const ptrdiff_t y_val = j > nGrid / 2 ? j - nGrid : j;
      correct &= phiy_view(i, j) == y_val;
    }
  }
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
