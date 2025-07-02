#ifndef TEMPLAT_LATTICE_MANIPULATION_GHOSTUPDATER_TEST_H
#define TEMPLAT_LATTICE_MANIPULATION_GHOSTUPDATER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/fft/fftlibraryselector.h"
#include "TempLat/fft/fftmpidomainsplit.h"
#include "TempLat/lattice/memory/triplestatelayouts.h"
#include "TempLat/lattice/algebra/operators/power.h"

#include <iomanip>

namespace TempLat
{
  /** \brief A namespace purely for test structures / classes. Quick simple structs for whatever test you need go in
   * here. */
  namespace TestScratch
  {
    /* quick and ugly helper struct */
    struct datumMPITypeHolder {
      MPI_Datatype dType;
      datumMPITypeHolder()
      {
#ifndef NOMPI
        MPI_Type_contiguous(4, TempLat::MPITypeSelect<ptrdiff_t>(), &dType);
        MPI_Type_commit(&dType);
#endif
      }
      ~datumMPITypeHolder()
      {
#ifndef NOMPI
        int didFinalize = 0;
        MPI_Finalized(&didFinalize);
        if (!didFinalize) MPI_Type_free(&dType);
#endif
      }
    };

    template <size_t NDim> struct datum {
      std::array<ptrdiff_t, NDim> data;

      friend std::ostream &operator<<(std::ostream &ostream, const datum &dat)
      {
        ostream << dat.data;
        return ostream;
      }

      static MPI_Datatype getMPIType()
      {
        static datumMPITypeHolder holder;
        return holder.dType;
      }
    };

    template <size_t NDim>
    void datum_initialize(MemoryBlock<NDim, datum<NDim>> &block, const size_t nGrid, const size_t nGhost)
    {
      if constexpr (NDim == 1) {
        auto view = block.getRawView();
        Kokkos::parallel_for(
            Kokkos::RangePolicy(0, nGrid),
            KOKKOS_LAMBDA(const size_t i) { view(nGhost + i) = datum<NDim>{(ptrdiff_t)i + 1}; });
      } else {
        std::array<ptrdiff_t, NDim> localSizes;
        for (size_t k = 0; k < NDim; ++k)
          localSizes[k] = nGrid + 2 * nGhost;
        auto view = block.getNDView(localSizes);

        std::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> slices{};
        for (size_t k = 0; k < NDim; ++k)
          slices[k] = std::make_pair(nGhost, nGhost + nGrid);

        auto subView = std::apply([&](const auto &...args) { return Kokkos::subview(view, args...); }, slices);
        auto functor = KOKKOS_LAMBDA(const std::array<size_t, NDim> &idx)
        {
          std::apply([&](const auto &...args) { subView(args...) = datum<NDim>{((ptrdiff_t)args + 1)...}; }, idx);
        };

        Kokkos::Array<size_t, NDim> it_start{};
        Kokkos::Array<size_t, NDim> it_stop{};
        for (size_t k = 0; k < NDim; ++k)
          it_stop[k] = nGrid;
        Kokkos::parallel_for("GhostUpdater", Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(it_start, it_stop),
                             KokkosNDLambdaWrapper<NDim, decltype(functor)>(functor));
      }
    }

    template <size_t nd> bool test_ghost_updater(const ptrdiff_t nGrid, const size_t nGhost)
    {
      std::array<ptrdiff_t, nd> gridArray{};
      std::array<ptrdiff_t, nd> gridArrayFull{};
      for (size_t i = 0; i < nd; ++i) {
        gridArray[i] = nGrid;
        gridArrayFull[i] = nGrid + 2 * nGhost;
      }

      constexpr bool verbose = false;

      auto print_it = [&](auto view) {
        if (!verbose) return;
        const size_t total_size = pow<nd>(nGrid + 2 * nGhost);
        std::array<size_t, nd> cIdx{};
        for (size_t i = 0; i < total_size; ++i) {
          // Linear index to cartesian index
          size_t lsize = 1;
          size_t remainder = i;
          for (size_t j = 0; j < nd; ++j) {
            lsize = gridArrayFull[nd - 1 - j];
            cIdx[nd - 1 - j] = remainder % lsize;
            remainder = (remainder - cIdx[nd - 1 - j]) / gridArrayFull[nd - 1 - j];
          }
          if (cIdx[nd - 1] == 0) sayMPI << "\n";
          sayMPI << std::setw(4) << view(i).data[0];
          if ((size_t)cIdx[nd - 1] != (size_t)nGrid - 1) sayMPI << " ";
        }
        sayMPI << "\n\n";
      };

      auto toolBox = MemoryToolBox<nd>::makeShared(nGrid, nGhost);
      toolBox->unsetVerbose();
      MemoryBlock<nd, TestScratch::datum<nd>> block(pow<nd>(nGrid + 2 * nGhost));

      MPICartesianGroup mGroup(FFTMPIDomainSplit<nd>::makeMPIGroup(nd));
      FFTLibrarySelector<nd> fftlib(mGroup, gridArray);
      TripleStateLayouts fullLayout(fftlib.getLayout(), nGhost);
      GhostUpdater<nd> ghostUpdater(mGroup, fullLayout.getConfigSpaceJumps());

      TestScratch::datum_initialize<nd>(block, nGrid, nGhost);

      const size_t total_size = pow<nd>(nGrid + 2 * nGhost);
      std::array<size_t, nd> cIdx{};

      auto view = block.getRawHostView();
      print_it(view);

      ghostUpdater.update(block);

      block.flagHostMirrorOutdated();
      view = block.getRawHostView();
      print_it(view);

      bool all_correct = true;
      size_t ww = 0;
      for (size_t i = 0; i < total_size; ++i) {
        // Linear index to cartesian index
        size_t lsize = 1;
        size_t remainder = i;
        for (size_t j = 0; j < nd; ++j) {
          lsize = gridArrayFull[nd - 1 - j];
          cIdx[nd - 1 - j] = remainder % lsize;
          remainder = (remainder - cIdx[nd - 1 - j]) / gridArrayFull[nd - 1 - j];
        }

        auto should_value = cIdx;
        for (size_t d = 0; d < nd; ++d) {
          if (cIdx[d] < nGhost)
            should_value[d] = nGrid - (nGhost - cIdx[d] - 1);
          else if (cIdx[d] >= nGhost + nGrid)
            should_value[d] = cIdx[d] - nGrid - (nGhost - 1);
          else
            should_value[d] -= nGhost - 1;
        }

        auto is_value = view(i).data;

        for (size_t d = 0; d < nd; ++d) {
          all_correct &= (size_t)is_value[d] == (size_t)should_value[d];
          if ((size_t)is_value[d] != (size_t)should_value[d])
            sayMPI << ++ww << " false " << is_value << " | vs | " << should_value << " AT POSITION " << cIdx << "\n";
        }
      }
      return all_correct;
    }
  } // namespace TestScratch
} // namespace TempLat

template <size_t NDim> inline void TempLat::GhostUpdater<NDim>::Test(TempLat::TDDAssertion &tdd)
{
  // restrict the sizes, dimensionality can lead to some huge tests...

  tdd.verify(TestScratch::test_ghost_updater<NDim>(4, 1));
  if constexpr (NDim < 6) tdd.verify(TestScratch::test_ghost_updater<NDim>(16, 1));
  if constexpr (NDim < 4) {
    tdd.verify(TestScratch::test_ghost_updater<NDim>(32, 1));
    tdd.verify(TestScratch::test_ghost_updater<NDim>(128, 1));
  }

  tdd.verify(TestScratch::test_ghost_updater<NDim>(4, 2));
  if constexpr (NDim < 6) tdd.verify(TestScratch::test_ghost_updater<NDim>(16, 2));
  if constexpr (NDim < 4) {
    tdd.verify(TestScratch::test_ghost_updater<NDim>(32, 2));
    tdd.verify(TestScratch::test_ghost_updater<NDim>(128, 2));
  }

  tdd.verify(TestScratch::test_ghost_updater<NDim>(4, 3));
  if constexpr (NDim < 6) tdd.verify(TestScratch::test_ghost_updater<NDim>(16, 3));
  if constexpr (NDim < 4) {
    tdd.verify(TestScratch::test_ghost_updater<NDim>(32, 3));
    tdd.verify(TestScratch::test_ghost_updater<NDim>(128, 3));
  }
}

#endif
