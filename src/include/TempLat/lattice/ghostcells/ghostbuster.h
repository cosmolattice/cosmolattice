#ifndef TEMPLAT_LATTICE_MEMORY_GHOSTBUSTER_H
#define TEMPLAT_LATTICE_MEMORY_GHOSTBUSTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <functional>
#include <cstring>
#include <stdexcept>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/util/timer.h"
#include "TempLat/lattice/memory/jumpsholder.h"
#include "TempLat/lattice/memory/memoryblock.h"

namespace TempLat
{
  MakeException(GhostBusterOrderException);
  MakeException(GhostBusterBoundsException);

  /** \brief A class which adds and removes (busts!) ghost cells to your layout.
   * Pass the original layout and the target layout to the constructor.
   * Then pass any pointer which satisfies for origin layout to the function call operator.
   * And the rest will be history.
   *
   * ```
   *  GhostBuster egon(JumpsHolder from, JumpsHolder to);
   *  egon(ptr);
   * ```
   *
   * Since the ghost cells are ghost cells, they will be overwritten arbitrarily. The non-ghost
   * cells are guaranteed to port their value from the old to the new layout.
   *
   * Memory allocation of the pointer is NOT affected. This is a memmove-like routine.
   *
   * Unit test: make test-ghostbuster
   **/
  template <size_t NDim> class GhostBuster
  {
  public:
    /* Put public methods here. These should change very little over time. */

    GhostBuster(const JumpsHolder<NDim> &from, const JumpsHolder<NDim> &to, bool verbose = false)
        : mFrom(from), mTo(to),
          // mDirection is positive, if we contract (i.e. remove ghosts)
          // and negative if we expand (i.e. add ghosts) the memory.
          mDirection(mFrom.toOrigin() + mFrom.getJumpsInMemoryOrder()[0] - mTo.toOrigin() -
                     mTo.getJumpsInMemoryOrder()[0])
    {
      // In-place ghostbusting only works if the motion
      // is in the same direction for all dimensions.

      if (mFrom.getJumpsInMemoryOrder()[NDim - 1] != 1)
        throw GhostBusterOrderException(
            "GhostBuster only works for memory layouts with jumps of 1 in the last dimension, not with this:",
            mFrom.getJumpsInMemoryOrder(), " to ", mTo.getJumpsInMemoryOrder());

      bool consistent = true;
      for (ptrdiff_t i = 0, iEnd = mFrom.getJumpsInMemoryOrder().size(); i < iEnd; ++i) {
        ptrdiff_t thisSign = mFrom.getJumpsInMemoryOrder()[i] - mTo.getJumpsInMemoryOrder()[i];
        if (mDirection == 0) mDirection = thisSign;
        consistent = consistent && (thisSign * mDirection >= 0);
      }
      if (verbose) say << "from " << from << " to " << to << "\n";
      if (!consistent)
        throw GhostBusterOrderException(
            "Can only work with jumps that change in the same direction for all dimensions, not with this:",
            mFrom.getJumpsInMemoryOrder(), " to ", mTo.getJumpsInMemoryOrder());
    }

    /** \brief Do the transformation. Size is passed on, only used when compiled CHECKBOUNDS defined. */
    template <typename T> void operator()(T *ptr, ptrdiff_t memSize) { bustTheGhosts(ptr, memSize); }

    /** \brief overload for passing objects which have a data() and a size() method, like std::vector<T> */
    template <template <size_t _NDim, typename S, typename... MArgs> class M, typename T, typename... Args>
    void operator()(M<NDim, T, Args...> &obj)
    {
      // Timer timer;
      bustTheGhostsWithViews(obj);
      // std::cout << "GhostBuster took " << timer << std::endl;
    }

    /** \brief overload for passing objects which have a data() and a size() method, like std::vector<T> */
    template <template <typename S, typename... MArgs> class M, typename T, typename... Args>
    void operator()(M<T, Args...> &obj)
    {
      operator()((T *)obj.data(), obj.size());
    }

#ifdef __INTEL_COMPILER
    // for some reasons, icc does not understand the default nested template for zero arguments.

    /** \brief overload for passing objects which have a data() and a size() method, like std::vector<T> */
    template <template <size_t _NDim, typename S> class M, typename T> void operator()(M<NDim, T> &obj)
    {
      operator()((T *)obj.data(), obj.size());
    }
#endif

  public:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    JumpsHolder<NDim> mFrom, mTo;
    ptrdiff_t mDirection;

    template <typename T> void bustTheGhostsWithViews(MemoryBlock<NDim, T> &block)
    {
      const auto from_padding = mFrom.getPadding();
      const auto to_padding = mTo.getPadding();
      const auto from_sizes = mFrom.getSizesInMemory();
      const auto to_sizes = mTo.getSizesInMemory();

      // Get Views to the data
      std::array<ptrdiff_t, NDim> from_full_sizes{};
      std::array<ptrdiff_t, NDim> to_full_sizes{};
      for (size_t i = 0; i < NDim; ++i) {
        from_full_sizes[i] = from_padding[i][0] + from_sizes[i] + from_padding[i][1];
        to_full_sizes[i] = to_padding[i][0] + to_sizes[i] + to_padding[i][1];
      }

      // say << "GhostBuster: Padding is " << from_padding << " to " << to_padding << ", sizes are " << from_sizes
      //     << " to " << to_sizes << "\n";
      auto fromView = block.template getNDView<T>(from_full_sizes);
      auto toView = block.template getNDView<T>(to_full_sizes);

      using LayoutType = typename decltype(fromView)::array_layout;

      // Create subviews for the from and to views
      // We need to create slices for each dimension, taking into account the padding
      // and the layout of the views
      std::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> slicesFrom{};
      std::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> slicesTo{};
      for (size_t i = 0; i < NDim; ++i) {
        size_t to_i{};
        if constexpr (std::is_same_v<LayoutType, Kokkos::LayoutRight>)
          to_i = i;
        else if constexpr (std::is_same_v<LayoutType, Kokkos::LayoutLeft>)
          to_i = NDim - 1 - i;
        slicesFrom[to_i] = std::make_pair(from_padding[i][0], from_padding[i][0] + from_sizes[i]);
        slicesTo[to_i] = std::make_pair(to_padding[i][0], to_padding[i][0] + to_sizes[i]);
      }

      auto fromSubView =
          std::apply([&](const auto &...args) { return Kokkos::subview(fromView, args...); }, slicesFrom);
      auto toSubView = std::apply([&](const auto &...args) { return Kokkos::subview(toView, args...); }, slicesTo);

      // sanity check: the sizes of the subviews should match,
      // except for the last dimension, which can be padded for the FFT
      if constexpr (NDim > 1)
        for (size_t i = 0; i < NDim - 1; ++i) {
          if (fromSubView.extent(i) != toSubView.extent(i)) {
            throw GhostBusterBoundsException("GhostBuster: Subview sizes in dimension ", i,
                                             " do not match after padding: ", fromSubView.extent(i),
                                             " != ", toSubView.extent(i));
          }
        }

      std::array<ptrdiff_t, NDim> copy_sizes{};
      for (size_t i = 0; i < NDim; ++i)
        copy_sizes[i] = std::min(fromSubView.extent(i), toSubView.extent(i));

      device::array<ptrdiff_t, NDim - 1> curIdx{};
      if (mDirection < 0) {
        // if we are moving in the negative direction, we
        // - have larger indices in the to view
        // - thus have to start moving from the end of from (we expand the memory)
        // - we can move blocks of the smallest dimension (i.e. last dimension) first, and then
        //   go to the larger dimensions.
        if constexpr (NDim > 1)
          for (size_t i = 0; i < NDim - 1; ++i) {
            curIdx[i] = fromSubView.extent(i) - 1; // start from the last index in each dimension
          }
        // std::cout << "Starting from the end of the from view: " << curIdx << std::endl;
      } else {
        // if we are moving in the positive direction, we
        // - have smaller indices in the to view
        // - thus have to start moving from the start of from (we compress the memory)
        // - we move blocks of the smallest dimension (i.e. last dimension) first, and then
        //   go to the larger dimensions.
        // std::cout << "Starting from the beginning of the from view: " << curIdx << std::endl;
      }

      if constexpr (std::is_same_v<LayoutType, Kokkos::LayoutRight>) {
        // A temporary is necessary as the source and the destination may have overlap
        Kokkos::View<T *, typename decltype(fromSubView)::execution_space> temp("temp_copy", copy_sizes[NDim - 1]);

        // iterate over all "large indices"
        bool hasNext = true;
        if constexpr (NDim > 1) {
          while (hasNext) {
            // copy to the temporary
            Kokkos::parallel_for(
                Kokkos::RangePolicy<typename decltype(fromSubView)::execution_space>(0, copy_sizes[NDim - 1]),
                DEVICE_LAMBDA(const size_t i) {
                  device::apply([&](const auto &...args) { temp(i) = fromSubView(args..., i); }, curIdx);
                });
            // copy from the temporary to the destination
            Kokkos::parallel_for(
                Kokkos::RangePolicy<typename decltype(fromSubView)::execution_space>(0, copy_sizes[NDim - 1]),
                DEVICE_LAMBDA(const size_t i) {
                  device::apply([&](const auto &...args) { toSubView(args..., i) = temp(i); }, curIdx);
                });
            // Check if we have a next index to go to
            hasNext =
                (mDirection < 0) ? lowerDimN(NDim - 2, curIdx, copy_sizes) : raiseDimN(NDim - 2, curIdx, copy_sizes);
          }
        } else if constexpr (NDim == 1) {
          Kokkos::parallel_for(
              Kokkos::RangePolicy<typename decltype(fromSubView)::execution_space>(0, copy_sizes[0]),
              DEVICE_LAMBDA(const size_t i) { temp(i) = fromSubView(i); });
          Kokkos::parallel_for(
              Kokkos::RangePolicy<typename decltype(fromSubView)::execution_space>(0, copy_sizes[0]),
              DEVICE_LAMBDA(const size_t i) { toSubView(i) = temp(i); });
        }
      } else {
        throw GhostBusterOrderException("GhostBuster only works for memory layouts with LayoutRight, not with this");
      }

      // We need the fence only at the very end, as consecutive kernel launches happen in order.
      Kokkos::fence();
      block.flagHostMirrorOutdated();
    }

  private:
    static inline bool lowerDimN(const ptrdiff_t DimN, device::array<ptrdiff_t, NDim - 1> &nextIdx,
                                 const std::array<ptrdiff_t, NDim> &copy_sizes)
    {
      if (DimN < 0) {
        // We are done, we have iterated through all dimensions
        return false;
      }
      if (nextIdx[DimN] > 0) {
        --nextIdx[DimN];
        return true;
      } else {
        nextIdx[DimN] = copy_sizes[DimN] - 1;            // reset to the last index
        return lowerDimN(DimN - 1, nextIdx, copy_sizes); // carry to the next dimension
      }
    };

    static inline bool raiseDimN(const ptrdiff_t DimN, device::array<ptrdiff_t, NDim - 1> &nextIdx,
                                 const std::array<ptrdiff_t, NDim> &copy_sizes)
    {
      if (DimN < 0) {
        // We are done, we have iterated through all dimensions
        return false;
      }
      if (nextIdx[DimN] < copy_sizes[DimN] - 1) {
        ++nextIdx[DimN];
        return true;
      } else {
        nextIdx[DimN] = 0;                               // reset to the first index
        return raiseDimN(DimN - 1, nextIdx, copy_sizes); // carry to the next dimension
      }
    };

    template <typename T> void bustTheGhosts(T *ptr, ptrdiff_t memSize)
    {
      recursor<T>(mFrom.getSizesInMemory().data(), ptr + mFrom.toOrigin(), mFrom.getJumpsInMemoryOrder().data(),
                  ptr + mTo.toOrigin(), mTo.getJumpsInMemoryOrder().data(), 0, ptr + memSize);
    }

    template <typename T>
    void recursor(const ptrdiff_t *layout, T *fromPtr, const ptrdiff_t *fromJumps, T *toPtr, const ptrdiff_t *toJumps,
                  ptrdiff_t thisDim, T *endPtr)
    {
      if (thisDim < (ptrdiff_t)NDim - 1) {
        ptrdiff_t iStart = mDirection < 0 ? (*layout) - 1 : 0;
        ptrdiff_t iEnd = mDirection < 0 ? -1 : *layout;
        ptrdiff_t di = mDirection < 0 ? -1 : 1;

        for (ptrdiff_t i = iStart; mDirection * i < mDirection * iEnd; i += di) {
          // say << "recursor dim " << thisDim << " i " << i << " jump: " << i * (*fromJumps) << "\n";
          recursor<T>(layout + 1, fromPtr + i * (*fromJumps), fromJumps + 1, toPtr + i * (*toJumps), toJumps + 1,
                      thisDim + 1, endPtr);
        }
      } else { /* move the rods */
               /* who're you gonna call? */
        // say << "forward - Moving " << *layout << " to " << toPtr << " end point " << (toPtr + *layout * sizeof(T))
        //     << " from " << fromPtr << " end point " << (fromPtr + *layout * sizeof(T)) << " Correct? "
        //     << (mDirection * (ptrdiff_t)toPtr <= mDirection * (ptrdiff_t)fromPtr ? "YES" : "NO") << "\n";
#ifdef CHECKBOUNDS
        if (mDirection * (ptrdiff_t)toPtr > mDirection * (ptrdiff_t)fromPtr)
          throw GhostBusterOrderException("recursor: Overwriting valid memory in GhostBuster - the orders are wrong.",
                                          toPtr, mDirection < 0 ? " < " : " > ", fromPtr);

        if (toPtr + *layout > endPtr || fromPtr + *layout > endPtr) {
          throw GhostBusterBoundsException("Detected memory access out of bounds. End pointer: ", endPtr,
                                           ", but write at end", toPtr + *layout, "and read at end", fromPtr + *layout);
        }
#endif
        std::memmove(toPtr, fromPtr, *layout * sizeof(T));
      }
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
