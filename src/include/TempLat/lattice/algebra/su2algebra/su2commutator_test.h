#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2COMMUTATOR_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2COMMUTATOR_TEST_H

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  Adrien Florio, Year: 2025

#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/lattice/algebra/su2algebra/scalarsu2multiplication.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"

inline void TempLat::SU2CommutatorTester::Test(TempLat::TDDAssertion &tdd)
{
  auto toolBox = std::make_shared<MemoryToolBox<3>>(2, 1);
  SU2Field<3, float> su2_1("testSU2_1", toolBox);
  SU2Field<3, float> su2_2("testSU2_2", toolBox);
  SU2Field<3, float> result("resultSU2", toolBox);

  su2_1(1_c) = 1.0f;
  su2_1(2_c) = -1.5f;
  su2_1(3_c) = 2.0f;

  su2_2(1_c) = 0.5f;
  su2_2(2_c) = 1.0f;
  su2_2(3_c) = -0.5f;

  // Trivial test: commutator of something with itself is zero.
  /*{
    result = commutator(su2_1, su2_1);

    auto f1view = result(1_c).getLocalNDHostView();
    auto f2view = result(2_c).getLocalNDHostView();
    auto f3view = result(3_c).getLocalNDHostView();

    bool all_true = true;
    for (size_t i = 0; i < f1view.extent(0); ++i)
      for (size_t j = 0; j < f1view.extent(1); ++j)
        for (size_t k = 0; k < f1view.extent(2); ++k) {
          all_true = all_true && (f1view(i, j, k) == 0.f);
          all_true = all_true && (f2view(i, j, k) == 0.f);
          all_true = all_true && (f3view(i, j, k) == 0.f);

          if (!all_true) {
            std::cout << "Failed at index " << i << " " << j << " " << k << "\n";
            std::cout << "Values are: " << f1view(i, j, k) << " " << f2view(i, j, k) << " " << f3view(i, j, k) << "\n";
          }
        }
    tdd.verify(all_true);
  }*/
  // Non-trivial test: commutator of two different things. We multiply by omega^2 to make sure that the caching works as
  // intended, i.e. eval is local to each site-thread.
  {
    WaveNumber<3> omega(toolBox);
    result = omega.norm2() * commutator(su2_1, su2_2);

    auto f1view = result(1_c).getLocalNDHostView();
    auto f2view = result(2_c).getLocalNDHostView();
    auto f3view = result(3_c).getLocalNDHostView();

    bool all_true = true;
    for (size_t i = 0; i < f1view.extent(0); ++i)
      for (size_t j = 0; j < f1view.extent(1); ++j)
        for (size_t k = 0; k < f1view.extent(2); ++k) {
          // offset by 1 because of the ghosts.
          const float mult = omega.norm2().get(i + 1, j + 1, k + 1);

          all_true = all_true && (f1view(i, j, k) == 2.5f * mult);
          all_true = all_true && (f2view(i, j, k) == -3.f * mult);
          all_true = all_true && (f3view(i, j, k) == -3.5f * mult);
          // I just checked it in Mathematica.

          if (!all_true) {
            std::cout << "Failed at index " << i << " " << j << " " << k << "\n";
            std::cout << "Values are: " << f1view(i, j, k) << " " << f2view(i, j, k) << " " << f3view(i, j, k) << "\n";
            std::cout << "Should be: " << 2.5f * mult << " " << -3.f * mult << " " << -3.5f * mult << "\n";
          }
        }
    tdd.verify(all_true);
  }
}

#endif
