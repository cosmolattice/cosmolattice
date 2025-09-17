#ifndef TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWCONFIG_TEST_H
#define TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWCONFIG_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/field/field.h"

template <size_t NDim, typename T> inline void TempLat::ConfigView<NDim, T>::Test(TempLat::TDDAssertion &tdd)
{
  const ptrdiff_t nGrid = 16, nGhost = 2;

  auto toolBox = MemoryToolBox<3>::makeShared(nGrid, nGhost);

  Field<NDim, double> a("a", toolBox);
  Field<NDim, double> b("b", toolBox);

  a = 100;
  b = 100;

  // get host views
  auto a_host = a.getRawHostView();
  auto b_host = b.getRawHostView();

  for (size_t i = 0; i < a_host.size(); ++i)
    std::cout << "a = " << a_host[i] << ", b = " << b_host[i] << std::endl;

  bool same = true;
  for (size_t i = 0; i < a_host.size(); ++i)
    same = same && AlmostEqual(a_host[i], b_host[i]) && AlmostEqual(a_host[i], 100.0);
  tdd.verify(same);

  /*ptrdiff_t nGrid = 256, nGhost = 2;

  auto toolBox = MemoryToolBox::makeShared(3, nGrid, nGhost);

  toolBox->setVerbose();

  FieldChainFinal<T> phiBase("phi", toolBox);
  FieldViewFourier<T> phi(phiBase);*/

  /*std::atomic<bool> lastDimPositiveDefinite(true);

  WaveNumber k;

  phi.iterate( [&](IterationCoordinates &pIterCoords) {
      bool localSuccess = pIterCoords[2] >= 0;*/
  /* atomically put the new value in place, but if we had success, it is important
    that we don't overwrite another thread's failure. */
  /*bool expected = true;
  lastDimPositiveDefinite.compare_exchange_weak(expected, localSuccess);
  //        say << offset << " localSuccess: " << localSuccess << " pIterCoords[2]: " << pIterCoords[2] << "\n";
  return localSuccess;
  });

  tdd.verify(lastDimPositiveDefinite == true);*/
}

#endif
