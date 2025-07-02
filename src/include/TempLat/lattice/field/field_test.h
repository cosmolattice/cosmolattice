#ifndef TEMPLAT_LATTICE_FIELD_FIELD_TEST_H
#define TEMPLAT_LATTICE_FIELD_FIELD_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

// #include "TempLat/lattice/algebra/gettergetoffset.h"
// #include "TempLat/lattice/algebra/coordinates/wavenumber.h"
#include "TempLat/lattice/algebra/random/randomgaussianfield.h"
#include "TempLat/lattice/algebra/helpers/getjumps.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"

template <size_t NDim, typename T> inline void TempLat::Field<NDim, T>::Test(TempLat::TDDAssertion &tdd)
{
  ptrdiff_t nGrid = 4, nGhost = 1;

  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

  toolBox->setVerbose();

  Field<NDim, T> phi("phi", toolBox);
  Field<NDim, T> chi("chi", toolBox);
  Field<NDim, T> psi("psi", toolBox);

  std::cout << "Layout info: " << toolBox->mLayouts.getConfigSpaceLayout() << "\n";

  phi.inFourierSpace();
  tdd.verify(phi.mManager->isFourierSpace());

  phi = 5;
  tdd.verify(!phi.mManager->isFourierSpace());

  chi = phi;
  tdd.verify(!chi.mManager->isFourierSpace());

  chi = LatticeLaplacian<NDim, decltype(phi)>(phi);
  phi.inFourierSpace() = 2;
  // WaveNumber k(toolBox);
  //  phi.inFourierSpace() = k.norm2(); // * RandomGaussianField<NDim, T>("Hoi", toolBox);
  tdd.verify(phi.mManager->isFourierSpace());

  // phi.inFourierSpace() = k.norm2() * RandomGaussianField<NDim, T>("Hoi", toolBox);

  // just manipulated phi(k), so it must still be in Fourier space, and ghosts are stale.
  // tdd.verify(phi.mManager->isFourierSpace());
  // tdd.verify(phi.mManager->areGhostsStale());

  // alternatively, put the result of getNorm in a variable.
  // SpatialCoordinate x;
  // auto r = x.getNorm();

  chi = 1; // pow(r, 3);
  // phi = 4;
  // psi = 5;
  std::array<size_t, NDim> localSizes;
  std::array<std::pair<size_t, size_t>, NDim> slices;
  for (size_t d = 0; d < NDim; ++d) {
    localSizes[d] = nGrid + 2 * nGhost;
    slices[d] = std::make_pair(nGhost, nGhost + nGrid);
  }

  auto field_tester = [&](Field<NDim, T> &f, const auto &op, double expected) {
    f = op;

    auto view = f.getLocalNDHostView();

    size_t total_size = 1;
    std::array<size_t, NDim> extents;
    for (size_t i = 0; i < NDim; ++i) {
      extents[i] = view.extent(i);
      total_size *= extents[i];
    }

    bool all_correct = true;
    std::array<size_t, NDim> cIdx{};
    for (size_t i = 0; i < total_size; ++i) {
      // Linear index to cartesian index
      size_t lsize = 1;
      size_t remainder = i;
      for (size_t j = 0; j < NDim; ++j) {
        lsize = extents[NDim - 1 - j];
        cIdx[NDim - 1 - j] = remainder % lsize;
        remainder = (remainder - cIdx[NDim - 1 - j]) / extents[NDim - 1 - j];
      }
      // std::cout << "View(";
      // for (size_t l = 0; l < NDim; ++l) {
      //   std::cout << cIdx[l];
      //   if (l != NDim - 1) std::cout << ", ";
      // }
      std::apply(
          [&](const auto &...args) {
            //      std::cout << ") = " << view(args...) << std::endl;
            all_correct = AlmostEqual(view(args...), expected);
            if (!AlmostEqual(view(args...), expected))
              sayMPI << "expected: " << expected << " got " << view(args...) << "\n";
          },
          cIdx);
    }
    tdd.verify(all_correct);
  };

  chi = 2;
  field_tester(chi, chi, 2);

  chi + chi + chi;

  field_tester(phi, chi + chi, 4);
  field_tester(phi, chi * chi, 2 * 2);
  // field_tester(^hi, chi * chi * chi, 2 * 2 * 2);
  //   field_tester(phi, pow<4>(chi), 2 * 2 * 2 * 2);
  //   field_tester(phi, chi + chi * chi + chi * chi * chi, 2 + 2 * 2 + 2 * 2 * 2);
  //   field_tester(phi, chi - chi, 0);
  //   field_tester(phi, chi / chi, 1);

  field_tester(phi, cos(chi), cos(2));

  return;
  /*
    // just manipulated chi(x), so it must still be in configuration space, and ghosts are stale.
    tdd.verify(!chi.mManager->isFourierSpace());
    tdd.verify(chi.mManager->isConfigSpace());
    tdd.verify(chi.mManager->areGhostsStale());

    // neat consequence of the implementation: an expression actually evaluates to a specific type. Keeping that
    instance,
    //     without passing it to an assignment operator, is simply the compiled expression. So we can do stuff with
    it. auto potential = 0.5 * phi * phi + 42 * chi * chi * phi * phi - chi + (-chi);

    // Stuff we can do:
    say << "Potential2: " << potential.toString() << "\n";

    // and awesomer:
    auto dVdPhi = potential.d(phi);

    say << "dPotential/dphi: " << dVdPhi.toString() << "\n";

    // didn't touch ghost cells of phi, so ghosts must still be stale. Also, didn't touch phi at all, so must be in
    // fourier space.
    tdd.verify(phi.mManager->isFourierSpace());
    tdd.verify(phi.mManager->areGhostsStale());
  */
  /*
  psi = psi + chi;

  psi = potential;

  // didn't touch ghost cells of phi, so ghosts must still be stale. Did read phi, so must be in configuration space.
  tdd.verify(phi.mManager->isConfigSpace());
  tdd.verify(phi.mManager->areGhostsStale());

  chi = phi + psi;

  chi.set(26,{3,4,5});

  bool bTest;

  auto tmp = chi(bTest, 3, 4, 5);

  if(bTest)
    tdd.verify(tmp == 26);
  */

  // still didn't touch chi's ghost cells.
  /*  tdd.verify(chi.mManager->areGhostsStale());
    phi = chi.getShifted({{1, 1, -1}});
    // now did touch the ghost cells, must be up to date
    tdd.verify(!chi.mManager->areGhostsStale());

    phi = chi.getShifted({{1, 1, -1}});

    tdd.verify(phi.mManager->areGhostsStale());
// an early optimization attempt.
//    for (int i = 0; i < 1000; ++i ) {
        TimeSpent ts;
        psi = Laplacian(phi);
        std::cerr << ts << " spent.\n";
//    }
    tdd.verify(!phi.mManager->areGhostsStale());
*/
}

#endif
