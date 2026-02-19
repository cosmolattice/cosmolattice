
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025
#include "TempLat/lattice/ghostcells/ghostupdater.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/device.h"
#include "TempLat/parallel/device_iteration.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/fft/fftlibraryselector.h"
#include "TempLat/fft/fftmpidomainsplit.h"
#include "TempLat/lattice/memory/triplestatelayouts.h"
#include "TempLat/lattice/algebra/operators/power.h"

#include <sstream>
#include <iomanip>

namespace TempLat
{

  /** @brief A namespace purely for test structures / classes. Quick simple structs for whatever test you need go in
   * here. */
  namespace TestScratch
  {
    /* quick and ugly helper struct */
    template <size_t NDim> struct datumMPITypeHolder {
      MPI_Datatype dType;
      datumMPITypeHolder()
      {
#ifdef HAVE_MPI
        MPI_Type_contiguous(NDim, TempLat::MPITypeSelect<ptrdiff_t>(), &dType);
        MPI_Type_commit(&dType);
#endif
      }
      ~datumMPITypeHolder()
      {
#ifdef HAVE_MPI
        int didFinalize = 0;
        MPI_Finalized(&didFinalize);
        if (!didFinalize) MPI_Type_free(&dType);
#endif
      }
    };

    template <size_t NDim> struct datum {
      device::IdxArray<NDim> data;

      friend std::ostream &operator<<(std::ostream &ostream, const datum &dat)
      {
        ostream << dat.data;
        return ostream;
      }

      static MPI_Datatype getMPIType()
      {
        static datumMPITypeHolder<NDim> holder;
        return holder.dType;
      }
    };

    template <size_t NDim> void datum_initialize(MemoryBlock<NDim, datum<NDim>> &block, const LayoutStruct<NDim> layout)
    {
      const auto localSizes = layout.getLocalSizes();
      const ptrdiff_t nGhost = layout.getNGhosts();

      std::string localSizesStr = "{";
      for (size_t i = 0; i < NDim; ++i) {
        localSizesStr += std::to_string(localSizes[i]);
        if (i != NDim - 1) localSizesStr += ", ";
      }
      localSizesStr += "}";

      if constexpr (NDim == 1) {
        auto view = block.getRawView();
        device::iteration::foreach (
            "DatumInitialize1D", device::IdxArray<1>{0}, device::IdxArray<1>(localSizes[0]),
            DEVICE_LAMBDA(const size_t i) {
              view(nGhost + i) = datum<NDim>{layout.getLocalStarts()[0] + (ptrdiff_t)i + 1};
            });
      } else {
        device::IdxArray<NDim> viewSizes;
        for (size_t k = 0; k < NDim; ++k)
          viewSizes[k] = localSizes[k] + 2 * nGhost;
        auto view = block.getNDView(viewSizes);

        device::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> slices{};
        for (size_t k = 0; k < NDim; ++k)
          slices[k] = std::make_pair(nGhost, nGhost + localSizes[k]);

        auto subView =
            device::apply([&](const auto &...args) { return device::memory::subview(view, args...); }, slices);
        auto functor = DEVICE_LAMBDA(const device::IdxArray<NDim> &idx)
        {
          device::IdxArray<NDim> val;
          for (size_t k = 0; k < NDim; ++k)
            val[k] = idx[k] + layout.getLocalStarts()[k] + 1;
          device::IdxArray<NDim> inc;
          for (size_t k = 0; k < NDim; ++k)
            inc[k] = k;
          device::apply([&](const auto &...args) { subView(idx[args]...) = datum<NDim>{val[args]...}; }, inc);
        };

        device::IdxArray<NDim> it_stop{};
        for (size_t k = 0; k < NDim; ++k)
          it_stop[k] = localSizes[k];

        device::iteration::foreach ("GhostUpdater", {}, it_stop, functor);
      }
    }

    template <size_t nd> bool test_ghost_updater(const ptrdiff_t nGrid, const size_t nGhost)
    {
      auto toolBox = MemoryToolBox<nd>::makeShared(nGrid, nGhost);
      toolBox->unsetVerbose();

      MPICartesianGroup mGroup(FFTMPIDomainSplit<nd>::makeMPIGroup(nd));
      FFTLibrarySelector<nd> fftlib(mGroup, toolBox->mNGridPointsVec);
      TripleStateLayouts fullLayout(fftlib.getLayout(), nGhost);
      GhostUpdater<nd> ghostUpdater(mGroup, fullLayout.getConfigSpaceLayout());

      const auto localSizes = fullLayout.getConfigSpaceLayout().getLocalSizes();
      device::IdxArray<nd> fullLocalSizes{};
      for (size_t i = 0; i < nd; ++i)
        fullLocalSizes[i] = localSizes[i] + 2 * nGhost;

      const bool verbose = (nd == 2) && (nGrid <= 16);

      auto print_it = [&](auto view) {
        if (!verbose) return;
        size_t total_size = 1;
        for (size_t i = 0; i < nd; ++i)
          total_size *= fullLocalSizes[i];
        device::IdxArray<nd> cIdx{};
        std::stringstream gridArrayStr;
        for (size_t i = 0; i < total_size; ++i) {
          // Linear index to cartesian index
          size_t lsize = 1;
          size_t remainder = i;
          for (size_t j = 0; j < nd; ++j) {
            lsize = fullLocalSizes[nd - 1 - j];
            cIdx[nd - 1 - j] = remainder % lsize;
            remainder = (remainder - cIdx[nd - 1 - j]) / fullLocalSizes[nd - 1 - j];
          }
          if (cIdx[nd - 1] == 0) gridArrayStr << "\n";
          gridArrayStr << std::setw(4) << view(i).data[0];
          if ((size_t)cIdx[nd - 1] != (size_t)nGrid - 1) gridArrayStr << " ";
        }
        gridArrayStr << "\n\n";

        sayMPI << "Current view:" << gridArrayStr.str();
      };

      MemoryBlock<nd, TestScratch::datum<nd>> block(pow<nd>(nGrid + 2 * nGhost));
      const auto config_layout = fullLayout.getConfigSpaceLayout();
      TestScratch::datum_initialize<nd>(block, config_layout);

      size_t total_size = 1;
      for (size_t i = 0; i < nd; ++i)
        total_size *= fullLocalSizes[i];
      device::IdxArray<nd> cIdx{};

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
          lsize = fullLocalSizes[nd - 1 - j];
          cIdx[nd - 1 - j] = remainder % lsize;
          remainder = (remainder - cIdx[nd - 1 - j]) / fullLocalSizes[nd - 1 - j];
          cIdx[nd - 1 - j] += config_layout.getLocalStarts()[nd - 1 - j] + 1 - nGhost;
        }

        auto should_value = cIdx;
        for (size_t d = 0; d < nd; ++d) {
          if (cIdx[d] < 1) should_value[d] += nGrid;
          if (cIdx[d] > nGrid) should_value[d] -= nGrid;
        }

        device::IdxArray<nd> is_value;
        for (size_t d = 0; d < nd; ++d)
          is_value[d] = view(i).data[d];

        for (size_t d = 0; d < nd; ++d) {
          all_correct &= (size_t)is_value[d] == (size_t)should_value[d];
          if ((size_t)is_value[d] != (size_t)should_value[d])
            sayMPI << ++ww << " false " << is_value << " (is) | vs | " << should_value << " (should) AT POSITION "
                   << cIdx << "\n";
          // else
          //   sayMPI << ++ww << " true " << is_value << " (is) | vs | " << should_value << " (should) AT POSITION "
          //          << cIdx << "\n";
        }
      }
      return all_correct;
    }
  } // namespace TestScratch

  template <size_t NDim> struct GhostUpdaterTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> void GhostUpdaterTester<NDim>::Test(TDDAssertion &tdd)
  {
    static_assert(NDim > 1, "GhostUpdater test only makes sense in 2 or more dimensions.");
    // I just don't have the patience to do the 1D case, since it involves no MPI communication.

    int size = 1;
#ifdef HAVE_MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif

    // restrict the sizes, dimensionality can lead to some huge tests...

    tdd.verify(TestScratch::test_ghost_updater<NDim>(4, 1));
    if constexpr (NDim < 6) tdd.verify(TestScratch::test_ghost_updater<NDim>(16, 1));
    if constexpr (NDim < 4) {
      tdd.verify(TestScratch::test_ghost_updater<NDim>(32, 1));
      tdd.verify(TestScratch::test_ghost_updater<NDim>(128, 1));
    }

    if (size > 2)
      tdd.verify(Throws<GhostUpdaterException>([&]() { TestScratch::test_ghost_updater<NDim>(4, 2); }));
    else
      tdd.verify(TestScratch::test_ghost_updater<NDim>(4, 2));
    if constexpr (NDim < 6) tdd.verify(TestScratch::test_ghost_updater<NDim>(16, 2));
    if constexpr (NDim < 4) {
      tdd.verify(TestScratch::test_ghost_updater<NDim>(32, 2));
      tdd.verify(TestScratch::test_ghost_updater<NDim>(128, 2));
    }

    if (size > 1)
      tdd.verify(Throws<GhostUpdaterException>([&]() { TestScratch::test_ghost_updater<NDim>(4, 3); }));
    else
      tdd.verify(TestScratch::test_ghost_updater<NDim>(4, 3));
    if constexpr (NDim < 6) tdd.verify(TestScratch::test_ghost_updater<NDim>(16, 3));
    if constexpr (NDim < 4) {
      tdd.verify(TestScratch::test_ghost_updater<NDim>(32, 3));
      tdd.verify(TestScratch::test_ghost_updater<NDim>(128, 3));
    }
  }

} // namespace TempLat

namespace
{
  // TempLat::TDDContainer<TempLat::GhostUpdaterTester<1>> test1;
  TempLat::TDDContainer<TempLat::GhostUpdaterTester<2>> test2;
  TempLat::TDDContainer<TempLat::GhostUpdaterTester<3>> test3;
  TempLat::TDDContainer<TempLat::GhostUpdaterTester<4>> test4;
  TempLat::TDDContainer<TempLat::GhostUpdaterTester<5>> test5;
  TempLat::TDDContainer<TempLat::GhostUpdaterTester<6>> test6;
} // namespace
