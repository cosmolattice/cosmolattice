#ifndef TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/field/field.h"

inline void TempLat::RandomGaussianFieldTester::Test(TempLat::TDDAssertion &tdd)
{
  /* test the stability of the getter at various coordinates. */

  const ptrdiff_t nGrid = 32, nGhost = 2;
  const ptrdiff_t fourierGridPoints = nGrid * nGrid * (nGrid / 2 + 1); // +1 for the zero frequency.
  auto toolBox = MemoryToolBox<3>::makeShared(nGrid, nGhost);
  const ptrdiff_t localFourierGridPoints = fourierGridPoints / toolBox->getNProcesses();

  RandomGaussianField<3, double> myField("Hello world", toolBox);

  Field<3, double> a("a", toolBox);
  Field<3, double> b("b", toolBox);

  // Get random values
  a.inFourierSpace() = myField;

  // Get more random values
  b.inFourierSpace() = myField;

  {
    auto a_host = a.inFourierSpace().getRawHostView();
    auto b_host = b.inFourierSpace().getRawHostView();

    // Check that the values are different
    bool different = true;
    for (ptrdiff_t i = 0; i < localFourierGridPoints; ++i) {
      // show the first few values for debugging
      if (i < 8) std::cout << "index " << i << " a: " << a_host(i) << " b: " << b_host(i) << "\n";
      bool local = !AlmostEqual(a_host(i), b_host(i)) && std::isfinite(abs(a_host(i))) && std::isfinite(abs(b_host(i)));
      different &= local;
      if (!local) sayMPI << "Error at index " << i << ", a: " << a_host(i) << ", b: " << b_host(i) << "\n ";
    }
    tdd.verify(different);
  }

  // test rewinding
  myField.reset(); // reset the field to the initial generation.
  b.inFourierSpace() = myField;

  {
    auto a_host = a.inFourierSpace().getRawHostView();
    auto b_host = b.inFourierSpace().getRawHostView();

    // Check that the values are identical
    bool rewinding = true;
    for (size_t i = 0; i < localFourierGridPoints; ++i)
      rewinding &= AlmostEqual(a_host(i), b_host(i)) && std::isfinite(abs(a_host(i))) && std::isfinite(abs(b_host(i)));
    tdd.verify(rewinding);
  }
}

#endif
