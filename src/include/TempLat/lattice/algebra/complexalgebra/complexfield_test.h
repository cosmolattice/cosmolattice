#ifndef TEMPLAT_COMPLEXFIELDALGEBRA_COMPLEXFIELD_TEST_H
#define TEMPLAT_COMPLEXFIELDALGEBRA_COMPLEXFIELD_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/algebra/complexalgebra/complexalgebra.h"

template <size_t NDim, typename T> inline void TempLat::ComplexField<NDim, T>::Test(TempLat::TDDAssertion &tdd)
{
  ptrdiff_t nGrid = 16, nGhost = 2;

  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

  toolBox->setVerbose();

  // Test whether a transformation of the field forward and backward works.
  {
    ComplexField<NDim, T> original("original", toolBox);
    SpatialCoordinate x(toolBox);
    original.ComplexFieldGet(Tag<0>()) = getVectorComponent(x, 0);
    original.ComplexFieldGet(Tag<1>()) = getVectorComponent(x, 1);
    original.updateGhosts();

    ComplexField<NDim, T> copy("copy", toolBox);
    copy = original;

    // force fourier transformation on copy
    copy.ComplexFieldGet(Tag<0>()).getMemoryManager()->confirmFourierSpace();
    copy.ComplexFieldGet(Tag<1>()).getMemoryManager()->confirmFourierSpace();

    // force config transformation on copy
    copy.ComplexFieldGet(Tag<0>()).getMemoryManager()->confirmConfigSpace();
    copy.ComplexFieldGet(Tag<1>()).getMemoryManager()->confirmConfigSpace();

    // update ghosts on copy
    copy.updateGhosts();

    auto original_host_r = original.ComplexFieldGet(Tag<0>()).getRawHostView();
    auto original_host_i = original.ComplexFieldGet(Tag<1>()).getRawHostView();
    auto copy_host_r = copy.ComplexFieldGet(Tag<0>()).getRawHostView();
    auto copy_host_i = copy.ComplexFieldGet(Tag<1>()).getRawHostView();

    bool backforthWorks = true;
    for (ptrdiff_t i = 0; i < pow<NDim>(nGrid + 2 * nGhost); ++i) {
      backforthWorks = backforthWorks && AlmostEqual(original_host_r[i], copy_host_r[i]);
      backforthWorks = backforthWorks && AlmostEqual(original_host_i[i], copy_host_i[i]);
    }
    tdd.verify(backforthWorks);
  }
  /*
    // ------------------------------------------------------------------------------------------

    {

      Field<NDim, T> phi("phi", toolBox);
      Field<NDim, T> chi("chi", toolBox);

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
                  sayShort << "operation " << op << " expected: " << expected << " got " << view(args...) << "\n";
              },
              cIdx);
        }
        tdd.verify(all_correct);
      };

      chi = 2;
      field_tester(chi, chi, 2);
      field_tester(phi, chi + chi, 4);
      field_tester(phi, chi * chi, 2 * 2);
      field_tester(phi, chi * chi * chi, 2 * 2 * 2);
      field_tester(phi, pow<4>(chi), 2 * 2 * 2 * 2);
      field_tester(phi, chi + chi * chi + chi * chi * chi, 2 + 2 * 2 + 2 * 2 * 2);
      field_tester(phi, chi - chi, 0);
      field_tester(phi, chi / chi, 1);

      field_tester(phi, cos(chi), cos(2));
      field_tester(phi, sin(chi), sin(2));
      field_tester(phi, tanh(chi), tanh(2));
      field_tester(phi, sqrt(chi), sqrt(2));
      field_tester(phi, log(chi), log(2));
      field_tester(phi, abs(chi), abs(2));
      field_tester(phi, asinh(chi), asinh(2));
    }

    // ------------------------------------------------------------------------------------------

    {
      Field<NDim, T> phi("phi", toolBox);
      Field<NDim, T> chi("chi", toolBox);
      Field<NDim, T> psi("psi", toolBox);

      std::cout << "Layout info: " << toolBox->mLayouts.getConfigSpaceLayout() << "\n";

      phi.inFourierSpace();
      tdd.verify(phi.mManager->isFourierSpace());

      WaveNumber k(toolBox);
      phi.inFourierSpace() = k.norm2() * RandomGaussianField<NDim, T>("Hoi", toolBox);

      // just manipulated phi(k), so it must still be in Fourier space, and ghosts are stale.
      tdd.verify(phi.mManager->isFourierSpace());
      tdd.verify(phi.mManager->areGhostsStale());

      chi = LatticeLaplacian<NDim, decltype(phi)>(phi);
      // just manipulated chi(x), so it must still be in configuration space, and ghosts are stale.
      tdd.verify(!chi.mManager->isFourierSpace());
      tdd.verify(chi.mManager->isConfigSpace());
      tdd.verify(chi.mManager->areGhostsStale());
      tdd.verify(!phi.mManager->isFourierSpace());
      tdd.verify(!phi.mManager->areGhostsStale());

      // neat consequence of the implementation: an expression actually evaluates to a specific type. Keeping that
      // instance, without passing it to an assignment operator, is simply the compiled expression. So we can do stuff
      // with it.
      auto potential = 0.5 * phi * phi + 42 * chi * chi * phi * phi - chi + (-chi);

      // Stuff we can do:
      say << "Potential2: " << potential.toString() << "\n";

      // and awesomer:
      // auto dVdPhi = potential.d(phi);

      // say << "dPotential/dphi: " << dVdPhi.toString() << "\n";
    }
  */
}

#endif
