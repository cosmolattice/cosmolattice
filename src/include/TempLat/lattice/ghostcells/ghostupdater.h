#ifndef TEMPLAT_LATTICE_MANIPULATION_GHOSTUPDATER_H
#define TEMPLAT_LATTICE_MANIPULATION_GHOSTUPDATER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include <cstring>
#include <stdexcept>
#include <utility>
#include <ranges>

#include "TempLat/parallel/kokkos/lambdawrapper.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/mpi/mpitypeconstants.h"
#include "TempLat/parallel/mpi/mpitags.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesianexchange.h"
#include "TempLat/lattice/memory/jumpsholder.h"
#include "TempLat/lattice/memory/memoryblock.h"
#include "TempLat/lattice/ghostcells/ghostsubarraymap.h"

// #define IEXCH

namespace TempLat
{
  MakeException(GhostUpdaterException);

  /** \brief A class which updates the ghost cells in our total memory block.
   * By having the JumpsHolder, this class knows what is the ghostDepth.
   *
   * Has one public method, update<T>(T* ptr), which, based on JumpsHolder,
   * uses the associated subarrays and performs the exchange up and down
   * in all dimensions, through calls to MPICartesianExchange with the
   * appriate datatypes for the subarrays.
   *
   *
   * Unit test: make test-ghostupdater
   **/
  template <size_t NDim> class GhostUpdater
  {
  public:
    /* Put public methods here. These should change very little over time. */
    GhostUpdater(MPICartesianExchange exchange, JumpsHolder<NDim> jumpsHolder)
        : mExchange(exchange), mJumpsHolder(jumpsHolder), mGhostDepth(mJumpsHolder.getPadding()[0][0]),
          mGhostSubarrayMap(mJumpsHolder, mGhostDepth)
    {
      /* verify that */
      bool allSame = true;
      for (auto &&it : mJumpsHolder.getPadding()) {
        allSame = allSame && mGhostDepth == it[0] && mGhostDepth == it[1];
      }
      if (!allSame)
        throw GhostUpdaterException(
            "Can only work with identical padding at start and end of each dimension, not this.", allSame);
    }

    template <typename T> void update(MemoryBlock<NDim, T> &block)
    {
#ifndef NOMPI
      pUpdate(block.data());
#else
      pUpdate_NOMPI(block);
#endif
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    MPICartesianExchange mExchange;
    JumpsHolder<NDim> mJumpsHolder;
    ptrdiff_t mGhostDepth;
    GhostSubarrayMap<NDim> mGhostSubarrayMap;

    template <typename T> void pUpdate(T *ptr)
    {
      /* iterate dimensions */
      for (ptrdiff_t d = 0; d < NDim; ++d) {
        update_forDimension(ptr, d);
      }
    }

    template <typename T> void update_forDimension(T *ptr, ptrdiff_t dimension)
    {
      /* get neighbours */
#ifndef NOMPI
#ifndef IEXCH
      mExchange.exchangeUp(mGhostSubarrayMap.template getSubArray<T>(dimension), dimension,
                           /* base ptr is lower corner of all memory, including ghosts. */
                           /* send:
                            Don't jump to origin, but jump along the edge of dimension
                            to the point where we still have mGhostDepth until the end of
                            our *owned* memory (before the mGhostDepth hyper slices start) */
                           ptr + (mJumpsHolder.getSizesInMemory()[dimension]) *
                                     mJumpsHolder.getJumpsInMemoryOrder()[dimension],
                           /* receive: in origin, including ghosts. */
                           ptr);

      /* pointers: the same as above, but shifted by ghostDepth and ordering swapped. Yes. */
      mExchange.exchangeDown(mGhostSubarrayMap.template getSubArray<T>(dimension), dimension,
                             ptr + mGhostDepth * mJumpsHolder.getJumpsInMemoryOrder()[dimension],
                             ptr + (mGhostDepth + mJumpsHolder.getSizesInMemory()[dimension]) *
                                       mJumpsHolder.getJumpsInMemoryOrder()[dimension]);
#else
      mExchange.IrecvUp(mGhostSubarrayMap.getSubArray<T>(dimension), dimension,
                        /* base ptr is lower corner of all memory, including ghosts. */
                        /* send:
                         Don't jump to origin, but jump along the edge of dimension
                         to the point where we still have mGhostDepth until the end of
                         our *owned* memory (before the mGhostDepth hyper slices start) */
                        ptr + (mJumpsHolder.getSizesInMemory()[dimension]) *
                                  mJumpsHolder.getJumpsInMemoryOrder()[dimension],
                        /* receive: in origin, including ghosts. */
                        ptr);
      /* pointers: the same as above, but shifted by ghostDepth and ordering swapped. Yes. */
      mExchange.IrecvDown(mGhostSubarrayMap.getSubArray<T>(dimension), dimension,
                          ptr + mGhostDepth * mJumpsHolder.getJumpsInMemoryOrder()[dimension],
                          ptr + (mGhostDepth + mJumpsHolder.getSizesInMemory()[dimension]) *
                                    mJumpsHolder.getJumpsInMemoryOrder()[dimension]);
      /*Same as above*/
      mExchange.IsendUp(
          mGhostSubarrayMap.getSubArray<T>(dimension), dimension,
          ptr + (mJumpsHolder.getSizesInMemory()[dimension]) * mJumpsHolder.getJumpsInMemoryOrder()[dimension], ptr);
      mExchange.IsendDown(mGhostSubarrayMap.getSubArray<T>(dimension), dimension,
                          ptr + mGhostDepth * mJumpsHolder.getJumpsInMemoryOrder()[dimension],
                          ptr + (mGhostDepth + mJumpsHolder.getSizesInMemory()[dimension]) *
                                    mJumpsHolder.getJumpsInMemoryOrder()[dimension]);
      mExchange.waitall();
#endif
#endif
    }

  public:
    template <typename T> void pUpdate_NOMPI(MemoryBlock<NDim, T> &block, ptrdiff_t dimension = 0)
    {
      // Get View to the full data
      const auto padding = mJumpsHolder.getPadding();
      const auto sizes = mJumpsHolder.getSizesInMemory();
      std::array<ptrdiff_t, NDim> full_sizes{};
      for (size_t i = 0; i < NDim; ++i)
        full_sizes[i] = padding[i][0] + sizes[i] + padding[i][1];
      auto View = block.getNDView(full_sizes);

      // Create subviews for the from and to views
      // We need to create slices for each dimension, taking into account the padding
      // and the layout of the views
      std::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> btf_slicesFrom{};
      std::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> btf_slicesTo{};
      std::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> ftb_slicesFrom{};
      std::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> ftb_slicesTo{};

      for (size_t dim = 0; dim < NDim; ++dim) {
        for (size_t depth = 1; depth <= (size_t)mGhostDepth; ++depth) {

          if constexpr (NDim == 1) {
            // For NDim == 1, we just need to copy the corners.
            Kokkos::parallel_for(
                "GhostUpdater", Kokkos::RangePolicy(0, 1), KOKKOS_LAMBDA(const size_t) {
                  View(padding[0][0] - depth) = View(padding[0][0] + sizes[0] - depth);
                  View(padding[0][0] + sizes[0] + (depth - 1)) = View(padding[0][0] + (depth - 1));
                });
          } else {

            // so we copy a (NDim- 1)-dimensional slice. Include the padding, which leads to a copy of all corners, too!
            for (size_t i = 0; i < NDim; ++i) {
              btf_slicesFrom[i] =
                  (i == dim) ? std::make_pair<ptrdiff_t, ptrdiff_t>(padding[i][0] + sizes[i] - depth,
                                                                    padding[i][0] + sizes[i] - depth + 1)
                             : std::make_pair<ptrdiff_t, ptrdiff_t>(0, padding[i][0] + sizes[i] + padding[i][1]);
              btf_slicesTo[i] =
                  (i == dim) ? std::make_pair<ptrdiff_t, ptrdiff_t>(padding[i][0] - depth, padding[i][0] - depth + 1)
                             : std::make_pair<ptrdiff_t, ptrdiff_t>(0, padding[i][0] + sizes[i] + padding[i][1]);
              ftb_slicesFrom[i] =
                  (i == dim) ? std::make_pair<ptrdiff_t, ptrdiff_t>(padding[i][0] + (depth - 1),
                                                                    padding[i][0] + (depth - 1) + 1)
                             : std::make_pair<ptrdiff_t, ptrdiff_t>(0, padding[i][0] + sizes[i] + padding[i][1]);
              ftb_slicesTo[i] = (i == dim)
                                    ? std::make_pair<ptrdiff_t, ptrdiff_t>(padding[i][0] + sizes[i] + (depth - 1),
                                                                           padding[i][0] + sizes[i] + (depth - 1) + 1)
                                    : std::make_pair<ptrdiff_t, ptrdiff_t>(0, padding[i][0] + sizes[i] + padding[i][1]);
            }
            auto btf_fromSubView =
                std::apply([&](const auto &...args) { return Kokkos::subview(View, args...); }, btf_slicesFrom);
            auto btf_toSubView =
                std::apply([&](const auto &...args) { return Kokkos::subview(View, args...); }, btf_slicesTo);
            auto ftb_fromSubView =
                std::apply([&](const auto &...args) { return Kokkos::subview(View, args...); }, ftb_slicesFrom);
            auto ftb_toSubView =
                std::apply([&](const auto &...args) { return Kokkos::subview(View, args...); }, ftb_slicesTo);

            auto btf_functor = KOKKOS_LAMBDA(const std::array<size_t, NDim> &idx)
            {
              std::apply([&](auto &&...args) { btf_toSubView(args...) = btf_fromSubView(args...); }, idx);
            };
            auto ftb_functor = KOKKOS_LAMBDA(const std::array<size_t, NDim> &idx)
            {
              std::apply([&](auto &&...args) { ftb_toSubView(args...) = ftb_fromSubView(args...); }, idx);
            };

            const Kokkos::Array<size_t, NDim> it_start{};
            Kokkos::Array<size_t, NDim> it_stop{};
            for (size_t k = 0; k < NDim; ++k)
              it_stop[k] = btf_fromSubView.extent(k);
            Kokkos::parallel_for("GhostUpdater", Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(it_start, it_stop),
                                 KokkosNDLambdaWrapper<NDim, decltype(btf_functor)>(btf_functor));
            for (size_t k = 0; k < NDim; ++k)
              it_stop[k] = ftb_fromSubView.extent(k);
            Kokkos::parallel_for("GhostUpdater", Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(it_start, it_stop),
                                 KokkosNDLambdaWrapper<NDim, decltype(ftb_functor)>(ftb_functor));
          }
        }
      }
      return;
      /* for each dimension, walk all steps.
       When at the start of a ghost block in that dimension, copy that whole block,
       and jump past it.
       When in the 'owned memory' range of that dimension, recurse into deeper dimensions.
       */
      // ptrdiff_t blockSize = mGhostDepth * mJumpsHolder.getJumpsInMemoryOrder()[dimension];

      /* front to back */
      // std::memmove(ptr + (mGhostDepth + mJumpsHolder.getSizesInMemory()[dimension]) *
      //                        mJumpsHolder.getJumpsInMemoryOrder()[dimension],
      //              ptr + mGhostDepth * mJumpsHolder.getJumpsInMemoryOrder()[dimension], blockSize * sizeof(T));

      /* back to front */
      // std::memmove(ptr,
      //              ptr + mJumpsHolder.getSizesInMemory()[dimension] *
      //              mJumpsHolder.getJumpsInMemoryOrder()[dimension], blockSize * sizeof(T));

      // if (dimension < NDim - 1) {
      /* already copied blocks which end up in this dimension's ghosting, but do not skip that:
       * Just as in the MPI case, we need to copy all blocks including the ghosting,
       * such that after a full cycle, also the corner blocks are properly copies into place.
       */
      //  for (ptrdiff_t i = -mGhostDepth; i < mJumpsHolder.getSizesInMemory()[dimension] + mGhostDepth; ++i) {
      /* so yes, i starts at mGhostDepth, *and* we add another mGhostDepth here:
       we iterate our owned memory, which starts at ptr + mGhostDepth, and then
       we skip mGhostDepth slices, because we already copied those entirely. */
      //    pUpdate_NOMPI(ptr + (mGhostDepth + i) * mJumpsHolder.getJumpsInMemoryOrder()[dimension], dimension + 1);
      //  }
      //}
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
