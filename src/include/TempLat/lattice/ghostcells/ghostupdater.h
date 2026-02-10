#ifndef TEMPLAT_LATTICE_MANIPULATION_GHOSTUPDATER_H
#define TEMPLAT_LATTICE_MANIPULATION_GHOSTUPDATER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/mpi/mpitypeconstants.h"
#include "TempLat/parallel/mpi/mpitags.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesianexchange.h"
#include "TempLat/lattice/memory/jumpsholder.h"
#include "TempLat/lattice/memory/memoryblock.h"
#include "TempLat/lattice/ghostcells/ghostsubarraymap.h"

#include "TempLat/parallel/device_iteration.h"
#include "TempLat/parallel/device_memory.h"

// #define IEXCH

namespace TempLat
{
  MakeException(GhostUpdaterException);

  /** @brief A class which updates the ghost cells in our total memory block.
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
    // Put public methods here. These should change very little over time.
    GhostUpdater(MPICartesianExchange exchange, JumpsHolder<NDim> jumpsHolder)
        : mExchange(exchange), mJumpsHolder(jumpsHolder), mGhostDepth(mJumpsHolder.getPadding()[0][0]),
          mGhostSubarrayMap(mJumpsHolder, mGhostDepth)
    {
      auto full_sizes = mJumpsHolder.getSizesInMemory();
      for (uint i = 0; i < NDim; ++i) {
        if (mGhostDepth > full_sizes[i]) {
          throw GhostUpdaterException("Ghost depth is larger than local size in dimension " + std::to_string(i) + ":",
                                      mGhostDepth, " > ", full_sizes[i]);
        }
      }
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
      // There is no MPI splitting in one dimension. Also, when we have only a single node, there is no need to do MPI
      // communication.
      if (mExchange.getMPICartesianGroup().size() > 1 && NDim > 1) {
        pUpdate(block);
      } else
#endif
      {
        pUpdate_NOMPI(block);
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    MPICartesianExchange mExchange;
    JumpsHolder<NDim> mJumpsHolder;
    ptrdiff_t mGhostDepth;
    GhostSubarrayMap<NDim> mGhostSubarrayMap;

    template <typename T> void pUpdate(MemoryBlock<NDim, T> &block)
    {
      /* iterate dimensions */
      for (size_t d = 0; d < NDim; ++d) {
#if defined(KOKKOS_ENABLE_CUDA) || defined(KOKKOS_ENABLE_HIP) || defined(KOKKOS_ENABLE_SYCL)
        update_forDimension_device(block, d);
#else
        update_forDimension(block, d);
#endif
      }
    }

  public:
    template <typename T> void update_forDimension_device(MemoryBlock<NDim, T> &block, size_t dimension)
    {
      // We will copy slabs of thickness ghostDepth in the dimension 'dimension'.
      device::IdxArray<NDim> full_sizes = mJumpsHolder.getSizesInMemory();
      for (size_t i = 0; i < NDim; ++i)
        full_sizes[i] += 2 * mGhostDepth;
      device::IdxArray<NDim> slab_sizes = mJumpsHolder.getSizesInMemory();
      for (size_t i = 0; i < NDim; ++i)
        slab_sizes[i] += 2 * mGhostDepth;
      slab_sizes[dimension] = mGhostDepth;
      size_t total_size = 1;
      for (size_t i = 0; i < NDim; ++i)
        total_size *= slab_sizes[i];

      // We need two slabs of thickness ghostDepth
      auto sendSlab = device::apply(
          [&](const auto &...args) { return device::memory::NDView<NDim, T>("sendSlab", args...); }, slab_sizes);
      auto receiveSlab = device::apply(
          [&](const auto &...args) { return device::memory::NDView<NDim, T>("receiveSlab", args...); }, slab_sizes);

      // To get the right subviews of the full data, we need to create slices for each dimension
      device::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> send_slices{};
      device::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> receive_slices{};

      // UP
      {
        for (size_t i = 0; i < NDim; ++i) {
          // we send the end of the dimension
          send_slices[i] = (i == dimension) ? std::pair<ptrdiff_t, ptrdiff_t>(full_sizes[i] - 2 * mGhostDepth,
                                                                              full_sizes[i] - mGhostDepth)
                                            : std::pair<ptrdiff_t, ptrdiff_t>(0, slab_sizes[i]);
          // we receive at the origin of the dimension
          receive_slices[i] = (i == dimension) ? std::pair<ptrdiff_t, ptrdiff_t>(0, mGhostDepth)
                                               : std::pair<ptrdiff_t, ptrdiff_t>(0, slab_sizes[i]);
        }

        // Get Subviews to the full data
        auto sendSubView = device::apply(
            [&](const auto &...args) { return device::memory::subview(block.getNDView(full_sizes), args...); },
            send_slices);
        auto receiveSubView = device::apply(
            [&](const auto &...args) { return device::memory::subview(block.getNDView(full_sizes), args...); },
            receive_slices);

        // Copy the data to the send slab
        auto copy_to_slab_functor = DEVICE_LAMBDA(const device::IdxArray<NDim> &idx)
        {
          device::apply([&](auto &&...args) { sendSlab(args...) = sendSubView(args...); }, idx);
        };
        device::iteration::foreach ("copy_to_slab", {}, slab_sizes, copy_to_slab_functor);
        // We must fence the operation, as we need the data to be in the slab before we call MPI.
        device::iteration::fence();

        // Exchange the slabs
        MPI_Datatype dataType = MPITypeSelect<T>();

        mExchange.exchangeUp(dataType, dimension, sendSlab.data(), receiveSlab.data(), total_size);

        // Copy the data from the receive slab
        auto copy_from_slab_functor = DEVICE_LAMBDA(const device::IdxArray<NDim> &idx)
        {
          device::apply([&](auto &&...args) { receiveSubView(args...) = receiveSlab(args...); }, idx);
        };
        device::iteration::foreach ("copy_from_slab", {}, slab_sizes, copy_from_slab_functor);
        // We must fence the operation, as we need the data to be in the slab before we call MPI.
        device::iteration::fence();
      }

      // DOWN
      {
        for (size_t i = 0; i < NDim; ++i) {
          // we send the origin of the dimension
          send_slices[i] = (i == dimension) ? std::pair<ptrdiff_t, ptrdiff_t>(mGhostDepth, 2 * mGhostDepth)
                                            : std::pair<ptrdiff_t, ptrdiff_t>(0, slab_sizes[i]);
          // we receive at the end of the dimension
          receive_slices[i] = (i == dimension)
                                  ? std::pair<ptrdiff_t, ptrdiff_t>(full_sizes[i] - mGhostDepth, full_sizes[i])
                                  : std::pair<ptrdiff_t, ptrdiff_t>(0, slab_sizes[i]);
        }

        // Get Subviews to the full data
        auto sendSubView = device::apply(
            [&](const auto &...args) { return device::memory::subview(block.getNDView(full_sizes), args...); },
            send_slices);
        auto receiveSubView = device::apply(
            [&](const auto &...args) { return device::memory::subview(block.getNDView(full_sizes), args...); },
            receive_slices);

        // Copy the data to the send slab
        auto copy_to_slab_functor = DEVICE_LAMBDA(const device::IdxArray<NDim> &idx)
        {
          device::apply([&](auto &&...args) { sendSlab(args...) = sendSubView(args...); }, idx);
        };
        device::iteration::foreach ("copy_to_slab", {}, slab_sizes, copy_to_slab_functor);
        // We must fence the operation, as we need the data to be in the slab before we call MPI.
        device::iteration::fence();

        // Exchange the slabs
        mExchange.exchangeDown(MPITypeSelect<T>(), dimension, sendSlab.data(), receiveSlab.data(), total_size);

        // Copy the data from the receive slab
        auto copy_from_slab_functor = DEVICE_LAMBDA(const device::IdxArray<NDim> &idx)
        {
          device::apply([&](auto &&...args) { receiveSubView(args...) = receiveSlab(args...); }, idx);
        };
        device::iteration::foreach ("copy_from_slab", {}, slab_sizes, copy_from_slab_functor);
        // We must fence the operation, as we need the data to be in the slab before we call MPI.
        device::iteration::fence();
      }
    }

  private:
    template <typename T> void update_forDimension(MemoryBlock<NDim, T> &block, ptrdiff_t dimension)
    {
      auto *ptr = block.data();
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
      mExchange.IrecvUp(mGhostSubarrayMap.template getSubArray<T>(dimension), dimension,
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
      mExchange.IrecvDown(mGhostSubarrayMap.template getSubArray<T>(dimension), dimension,
                          ptr + mGhostDepth * mJumpsHolder.getJumpsInMemoryOrder()[dimension],
                          ptr + (mGhostDepth + mJumpsHolder.getSizesInMemory()[dimension]) *
                                    mJumpsHolder.getJumpsInMemoryOrder()[dimension]);
      /*Same as above*/
      mExchange.IsendUp(
          mGhostSubarrayMap.template getSubArray<T>(dimension), dimension,
          ptr + (mJumpsHolder.getSizesInMemory()[dimension]) * mJumpsHolder.getJumpsInMemoryOrder()[dimension], ptr);
      mExchange.IsendDown(mGhostSubarrayMap.template getSubArray<T>(dimension), dimension,
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
      const auto ghostDepth = mJumpsHolder.getPadding()[0][0];
      for (size_t i = 0; i < NDim; ++i)
        if (ghostDepth != mJumpsHolder.getPadding()[i][0] || ghostDepth != mJumpsHolder.getPadding()[i][1]) {
          throw GhostUpdaterException(
              "Can only work with identical padding at start and end of each dimension, not this.");
        }
      device::IdxArray<NDim> sizes;
      for (size_t i = 0; i < NDim; ++i)
        sizes[i] = mJumpsHolder.getSizesInMemory()[i];
      device::IdxArray<NDim> full_sizes{};
      for (size_t i = 0; i < NDim; ++i)
        full_sizes[i] = ghostDepth + sizes[i] + ghostDepth;
      auto View = block.getNDView(full_sizes);

      // Create subviews for the from and to views
      // We need to create slices for each dimension, taking into account the padding
      // and the layout of the views
      device::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> btf_slicesFrom{};
      device::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> btf_slicesTo{};
      device::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> ftb_slicesFrom{};
      device::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> ftb_slicesTo{};

      for (size_t dim = 0; dim < NDim; ++dim) {
        for (size_t depth = 1; depth <= (size_t)mGhostDepth; ++depth) {

          if constexpr (NDim == 1) {
            // For NDim == 1, we just need to copy the corners.
            device::iteration::foreach (
                "GhostUpdater", device::IdxArray<1>{0}, device::IdxArray<1>{1},
                DEVICE_LAMBDA(const device::IdxArray<1> &i) {
                  View(ghostDepth - depth) = View(ghostDepth + sizes[0] - depth);
                  View(ghostDepth + sizes[0] + (depth - 1)) = View(ghostDepth + (depth - 1));
                });
          } else {
            // so we copy a (NDim- 1)-dimensional slice. Include the padding, which leads to a copy of all corners, too!
            for (size_t i = 0; i < NDim; ++i) {
              btf_slicesFrom[i] = (i == dim)
                                      ? std::make_pair<ptrdiff_t, ptrdiff_t>(ghostDepth + sizes[i] - depth,
                                                                             ghostDepth + sizes[i] - depth + 1)
                                      : std::make_pair<ptrdiff_t, ptrdiff_t>(0, ghostDepth + sizes[i] + ghostDepth);
              btf_slicesTo[i] = (i == dim)
                                    ? std::make_pair<ptrdiff_t, ptrdiff_t>(ghostDepth - depth, ghostDepth - depth + 1)
                                    : std::make_pair<ptrdiff_t, ptrdiff_t>(0, ghostDepth + sizes[i] + ghostDepth);
              ftb_slicesFrom[i] =
                  (i == dim)
                      ? std::make_pair<ptrdiff_t, ptrdiff_t>(ghostDepth + (depth - 1), ghostDepth + (depth - 1) + 1)
                      : std::make_pair<ptrdiff_t, ptrdiff_t>(0, ghostDepth + sizes[i] + ghostDepth);
              ftb_slicesTo[i] = (i == dim)
                                    ? std::make_pair<ptrdiff_t, ptrdiff_t>(ghostDepth + sizes[i] + (depth - 1),
                                                                           ghostDepth + sizes[i] + (depth - 1) + 1)
                                    : std::make_pair<ptrdiff_t, ptrdiff_t>(0, ghostDepth + sizes[i] + ghostDepth);
            }
            auto btf_fromSubView = device::apply(
                [&](const auto &...args) { return device::memory::subview(View, args...); }, btf_slicesFrom);
            auto btf_toSubView = device::apply(
                [&](const auto &...args) { return device::memory::subview(View, args...); }, btf_slicesTo);
            auto ftb_fromSubView = device::apply(
                [&](const auto &...args) { return device::memory::subview(View, args...); }, ftb_slicesFrom);
            auto ftb_toSubView = device::apply(
                [&](const auto &...args) { return device::memory::subview(View, args...); }, ftb_slicesTo);

            auto btf_functor = DEVICE_LAMBDA(const device::IdxArray<NDim> &idx)
            {
              device::apply([&](auto &&...args) { btf_toSubView(args...) = btf_fromSubView(args...); }, idx);
            };
            auto ftb_functor = DEVICE_LAMBDA(const device::IdxArray<NDim> &idx)
            {
              device::apply([&](auto &&...args) { ftb_toSubView(args...) = ftb_fromSubView(args...); }, idx);
            };

            const device::IdxArray<NDim> it_start{};
            device::IdxArray<NDim> it_stop{};
            for (size_t k = 0; k < NDim; ++k)
              it_stop[k] = btf_fromSubView.extent(k);
            device::iteration::foreach ("GhostUpdater", it_start, it_stop, btf_functor);
            for (size_t k = 0; k < NDim; ++k)
              it_stop[k] = ftb_fromSubView.extent(k);
            device::iteration::foreach ("GhostUpdater", it_start, it_stop, ftb_functor);
          }
        }
      }
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
