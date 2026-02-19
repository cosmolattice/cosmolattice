#ifndef TEMPLAT_LATTICE_MEMORY_GHOSTBUSTER_H
#define TEMPLAT_LATTICE_MEMORY_GHOSTBUSTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2026

#include "TempLat/util/exception.h"
#include "TempLat/util/timer.h"
#include "TempLat/lattice/memory/memoryblock.h"

#include "TempLat/parallel/device_iteration.h"
#include "TempLat/parallel/device_memory.h"

namespace TempLat
{
  MakeException(GhostBusterOrderException);
  MakeException(GhostBusterBoundsException);

  /** @brief A class which adds and removes (busts!) ghost cells to your layout.
   * Pass the original layout and the target layout to the constructor.
   * Then pass any pointer which satisfies for origin layout to the function call operator.
   * And the rest will be history.
   *
   * ```
   *  GhostBuster egon(LayoutStruct<NDim> from, LayoutStruct<NDim> to);
   *  egon(ptr);
   * ```
   *
   * Since the ghost cells are ghost cells, they will be overwritten arbitrarily. The non-ghost
   * cells are guaranteed to port their value from the old to the new layout.
   *
   * Memory allocation of the pointer is NOT affected. This is a memmove-like routine.
   *
   * Unit test: ctest -R test-ghostbuster
   **/
  template <size_t NDim> class GhostBuster
  {
  public:
    // Put public methods here. These should change very little over time.

    GhostBuster(const LayoutStruct<NDim> &from, const LayoutStruct<NDim> &to, bool verbose = false)
        : mFrom(from), mTo(to)
    {
      // In-place ghostbusting only works if the motion is in the same direction for all dimensions.
      // So, we're going to determine this in the following.

      const auto from_padding = mFrom.getPadding();
      const auto to_padding = mTo.getPadding();

      const size_t originTo = mTo.getOrigin();
      const size_t originFrom = mFrom.getOrigin();

      size_t slab0SizeTo = 1;
      size_t slab0SizeFrom = 1;
      for (size_t i = 1; i < NDim; ++i) {
        slab0SizeTo *= mTo.getSizesInMemory()[i] + to_padding[i][0] + to_padding[i][1];
        slab0SizeFrom *= mFrom.getSizesInMemory()[i] + from_padding[i][0] + from_padding[i][1];
      }
      // We are SHRINKING memory
      if (slab0SizeTo < slab0SizeFrom && originTo <= originFrom) mDirection = 1;
      // We are EXPANDING memory
      else if (slab0SizeTo > slab0SizeFrom && originTo >= originFrom)
        mDirection = -1;
      // We are RESHUFFLING memory
      else if (slab0SizeTo == slab0SizeFrom) {
        // We are reshuffling memory, but in which direction?
        if (originTo <= originFrom)
          mDirection = 1;
        else
          mDirection = -1;
      } else {
        std::stringstream ss;
        ss << "GhostBuster only works for in-place reshuffling of memory, or "
           << "expanding/contracting in the same direction. "
           << "This is not the case for these layouts: "
           << "\norigins  from: " << mFrom.getOrigin()        //
           << "\n           to: " << mTo.getOrigin()          //
           << "\nSlabSize from: " << slab0SizeFrom            //
           << "\n           to: " << slab0SizeTo              //
           << "\npadding  from: " << mFrom.getPadding()       //
           << "\n           to: " << mTo.getPadding()         //
           << "\nsizes    from: " << mFrom.getSizesInMemory() //
           << "\n           to: " << mTo.getSizesInMemory();
        throw GhostBusterOrderException(ss.str());
      }
    }

    /** @brief overload for passing objects which have a data() and a size() method, like std::vector<T> */
    template <template <size_t _NDim, typename S, typename... MArgs> class M, typename T, typename... Args>
    void operator()(M<NDim, T, Args...> &obj)
    {
      Timer timer;
      bustTheGhosts(obj);
    }

    /** @brief overload for passing objects which have a data() and a size() method, like std::vector<T> */
    template <template <typename S, typename... MArgs> class M, typename T, typename... Args>
    void operator()(M<T, Args...> &obj)
    {
      operator()((T *)obj.data(), obj.size());
    }

#ifdef __INTEL_COMPILER
    // for some reasons, icc does not understand the default nested template for zero arguments.

    /** @brief overload for passing objects which have a data() and a size() method, like std::vector<T> */
    template <template <size_t _NDim, typename S> class M, typename T> void operator()(M<NDim, T> &obj)
    {
      operator()((T *)obj.data(), obj.size());
    }
#endif

  public:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    LayoutStruct<NDim> mFrom, mTo;

    /**
     * @brief mDirection is positive, if we contract (i.e. remove ghosts) and negative if we expand (i.e. add ghosts)
     * the memory.
     *
     */
    device::Idx mDirection;

    template <typename T> void bustTheGhosts(MemoryBlock<NDim, T> &block)
    {
      const auto from_padding = mFrom.getPadding();
      const auto to_padding = mTo.getPadding();
      const auto from_sizes = mFrom.getSizesInMemory();
      const auto to_sizes = mTo.getSizesInMemory();

      // Perform a check whether input and output are identical
      bool identical = true;
      for (size_t d = 0; d < NDim; ++d) {
        if (from_padding[d][0] != to_padding[d][0] || from_padding[d][1] != to_padding[d][1] ||
            from_sizes[d] != to_sizes[d]) {
          identical = false;
          break;
        }
      }
      if (identical) return;

      // std::cout << "GhostBuster: busting:\n"
      //           << " from padding: " << from_padding << "\n"
      //           << " from sizes:   " << from_sizes << "\n"
      //           << " to padding:   " << to_padding << "\n"
      //           << " to sizes:     " << to_sizes << "\n"
      //           << " direction:    " << mDirection << "\n"
      //           << std::endl;

      device::IdxArray<NDim> from_full_sizes{};
      device::IdxArray<NDim> to_full_sizes{};
      for (size_t i = 0; i < NDim; ++i) {
        from_full_sizes[i] = from_padding[i][0] + from_sizes[i] + from_padding[i][1];
        to_full_sizes[i] = to_padding[i][0] + to_sizes[i] + to_padding[i][1];
      }
      auto fromView = block.template getNDView<T>(from_full_sizes);
      auto toView = block.template getNDView<T>(to_full_sizes);

      device::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> slicesFrom{};
      device::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> slicesTo{};
      for (size_t i = 0; i < NDim; ++i) {
        slicesFrom[i] = std::make_pair(from_padding[i][0], from_padding[i][0] + from_sizes[i]);
        slicesTo[i] = std::make_pair(to_padding[i][0], to_padding[i][0] + to_sizes[i]);
      }

      // We always do slabs in the first (most striding) dimension.
      const int dim = 0;

      device::IdxArray<NDim> tslab_sizes{};
      for (size_t i = 0; i < NDim; ++i) {
        if (i == dim)
          tslab_sizes[i] = 1;
        else
          tslab_sizes[i] = to_sizes[i];
      }
      auto tslab = device::apply(
          [&](const auto &...args) { return device::memory::NDView<NDim, T>("GhostBusterTSlab", args...); },
          tslab_sizes);

      int _j_lim = mDirection > 0 ? std::min(to_sizes[dim], from_sizes[dim] + from_padding[dim][1])
                                  : std::min(to_sizes[dim], from_sizes[dim] + from_padding[dim][0]);
      for (int _j = 0; _j < _j_lim; ++_j) {
        // If we shrink (mDirection==1), we go from 0 to max. (slabs are dropping to "lower" memory)
        // If we expand (mDirection==-1), we go from max to 0. (slabs are rising to "higher" memory)
        const int j = mDirection > 0 ? _j : to_sizes[dim] - 1 - _j;

        device::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> slabSliceFrom{};
        device::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> slabSliceTo{};
        for (size_t i = 0; i < NDim; ++i) {
          if (i == dim) {
            slabSliceFrom[i] = std::make_pair(from_padding[i][0] + j, from_padding[i][0] + j + 1);
            slabSliceTo[i] = std::make_pair(to_padding[i][0] + j, to_padding[i][0] + j + 1);
          } else {
            slabSliceFrom[i] = std::make_pair(from_padding[i][0], from_padding[i][0] + to_sizes[i]);
            slabSliceTo[i] = std::make_pair(to_padding[i][0], to_padding[i][0] + to_sizes[i]);
          }
        }

        auto fromSubView = device::apply(
            [&](const auto &...args) { return device::memory::subview(fromView, args...); }, slabSliceFrom);
        auto toSubView =
            device::apply([&](const auto &...args) { return device::memory::subview(toView, args...); }, slabSliceTo);

        // Copy to the temporary
        device::memory::copyDeviceToDevice(fromSubView, tslab);
        // Copy to the destination
        device::memory::copyDeviceToDevice(tslab, toSubView);
      }

      // We need the fence only at the very end, as consecutive kernel launches happen in order.
      block.flagHostMirrorOutdated();
    }
  };
} // namespace TempLat

#endif
