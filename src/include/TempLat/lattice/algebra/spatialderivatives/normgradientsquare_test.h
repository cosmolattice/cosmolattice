#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_NORMGRADIENTSQUARE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_NORMGRADIENTSQUARE_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/random/randomgaussianfield.h"

inline void TempLat::NormGradientSquareTester::Test(TempLat::TDDAssertion &tdd)
{
  constexpr size_t nd = 2;
  const ptrdiff_t nGrid = 32, nGhost = 1;

  auto toolBox = MemoryToolBox<nd>::makeShared(nGrid, nGhost);

  /* create the random field once, keep in memory. Trade off between RAM use and redundant computations. */
  Field<nd, double> gaussian("gaussian", toolBox);
  gaussian.inFourierSpace() = 1 * RandomGaussianField<nd, double>("hoi", toolBox);
  gaussian += 0;

  Field<nd, double> normGradSq("normGradSq", toolBox);
  normGradSq = Grad2<nd>(gaussian);
  Field<nd, double> LatForwardGradNorm2("LatForwardGradNorm2", toolBox);
  LatForwardGradNorm2 = LatForwardGrad<nd>(gaussian).norm2();

  bool allGood = true;

  // Check element-wise equality between LatForwardGradNorm2 and normGradSq
  auto normGradSq_view = normGradSq.getLocalNDHostView();
  auto LatForwardGradNorm2_view = LatForwardGradNorm2.getLocalNDHostView();

  for (size_t i = 0; i < nGrid; ++i) {
    for (size_t j = 0; j < nGrid; ++j) {
      if (std::abs(LatForwardGradNorm2_view(i, j) - normGradSq_view(i, j)) > 1e-14) { // TODO
        allGood = false;
        std::cout << "Mismatch at (" << i << ", " << j << "): "
                  << "LatForwardGradNorm2 = " << LatForwardGradNorm2_view(i, j)
                  << ", normGradSq = " << normGradSq_view(i, j) << std::endl;
        break;
      }
    }
    if (!allGood) break;
  }

  tdd.verify(allGood);
}

#endif
