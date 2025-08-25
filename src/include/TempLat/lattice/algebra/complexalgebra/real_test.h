#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_REAL_TEST_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_REAL_TEST_H
#include <Kokkos_Macros.hpp>

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"

inline void TempLat::RealTester::Test(TempLat::TDDAssertion &tdd)
{
  struct MyStruct {
    KOKKOS_FORCEINLINE_FUNCTION
    int ComplexFieldGet(Tag<0> t) { return 87; };
    KOKKOS_FORCEINLINE_FUNCTION
    double ComplexFieldGet(Tag<1> t) { return 870; };
  };

  MyStruct ms;
  tdd.verify(Real(ms) == 87);

  // Test whether addition of two complex fields works.
  constexpr size_t NDim = 2;
  using T = double;
  ptrdiff_t nGrid = 16, nGhost = 2;
  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);
  toolBox->setVerbose();

  ComplexField<NDim, T> fa("a", toolBox);
  ComplexField<NDim, T> fb("b", toolBox);

  fb = ComplexFieldWrapper(Real(fa), ZeroType());
}

#endif
