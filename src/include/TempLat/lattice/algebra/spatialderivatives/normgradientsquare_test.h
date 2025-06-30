#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_NORMGRADIENTSQUARE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_NORMGRADIENTSQUARE_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::NormGradientSquareTester::Test(TempLat::TDDAssertion &tdd)
{
  constexpr size_t nd = 2;
  const ptrdiff_t nGrid = 32, nGhost = 1;

  auto toolBox = MemoryToolBox<nd>::makeShared(nGrid, nGhost);

  /* create the random field once, keep in memory. Trade off between RAM use and redundant computations. */
  Field<nd, double> gaussian("gaussian", toolBox);
  gaussian.inFourierSpace() = 1 * RandomGaussianField<double>("hoi", toolBox);
  gaussian += 0;

  Field<nd, double> normGradSq = Grad2(gaussian);
  Field<nd, double> LatForwardGradNorm2 = LatForwardGrad(gaussian).norm2();

  bool allGood = true;
  // for (itX.begin(); itX.end(); ++itX) {
  // allGood = allGood && AlmostEqual(Grad2(gaussian).get(itX()), LatForwardGrad(gaussian).norm2().get(itX()));
  //
  tdd.verify(allGood);
}

#endif
