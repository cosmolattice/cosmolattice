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
#include "TempLat/lattice/memory/jumpsholder.h"

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
      if constexpr (NDim > 1) bustTheGhostsWithViews(obj);
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

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    JumpsHolder<NDim> mFrom, mTo;
    ptrdiff_t mDirection;

    template <typename T> void bustTheGhostsWithViews(MemoryBlock<NDim, T> &block)
    {
      // this method should be never instantiated for NDim == 1
      static_assert(NDim > 1);

      const auto from_padding = mFrom.getPadding();
      const auto to_padding = mTo.getPadding();
      const auto from_sizes = mFrom.getSizesInMemory();
      const auto to_sizes = mTo.getSizesInMemory();

      std::cout << "from_sizes: " << from_sizes << std::endl;
      std::cout << "to_sizes: " << to_sizes << std::endl;

      std::array<std::pair<size_t, size_t>, NDim> slicesFrom;
      std::array<std::pair<size_t, size_t>, NDim> slicesTo;

      std::array<size_t, NDim> from_full_sizes;
      std::array<size_t, NDim> to_full_sizes;

      for (size_t i = 0; i < NDim; ++i) {
        slicesFrom[i] = std::make_pair(from_padding[i][0], from_padding[i][0] + from_sizes[i]);
        slicesTo[i] = std::make_pair(to_padding[i][0], to_padding[i][0] + to_sizes[i]);

        std::cout << "slicesFrom[" << i << "] = " << slicesFrom[i].first << ", " << slicesFrom[i].second << std::endl;
        std::cout << "slicesTo[" << i << "] = " << slicesTo[i].first << ", " << slicesTo[i].second << std::endl;

        from_full_sizes[i] = from_padding[i][0] + from_sizes[i] + from_padding[i][1];
        to_full_sizes[i] = to_padding[i][0] + to_sizes[i] + to_padding[i][1];
      }

      auto fromView = block.getNDView(from_full_sizes);
      auto toView = block.getNDView(to_full_sizes);

      auto fromSubView =
          std::apply([&](const auto &...args) { return Kokkos::subview(fromView, args...); }, slicesFrom);
      auto toSubView = std::apply([&](const auto &...args) { return Kokkos::subview(toView, args...); }, slicesTo);

      // sanity check: the sizes of the subviews should match
      for (size_t i = 0; i < NDim - 1; ++i) {
        if (fromSubView.extent(i) != toSubView.extent(i)) {
          throw GhostBusterBoundsException("GhostBuster: Subview sizes in dimension ", i,
                                           " do not match after padding: ", fromSubView.extent(i),
                                           " != ", toSubView.extent(i));
        }
      }
      // except for the last dimension, which can be padded for the FFT

      std::array<ptrdiff_t, NDim - 1> largeIdx{};
      if (mDirection < 0) {
        // if we are moving in the negative direction, we
        // - have larger indices in the to view
        // - thus have to start moving from the end of from (we expand the memory)
        // - we can move blocks of the smallest dimension (i.e. last dimension) first, and then
        //   go to the larger dimensions.
        for (size_t i = 0; i < NDim - 1; ++i) {
          largeIdx[i] = fromSubView.extent(i) - 1; // start from the last index in each dimension
        }
      } else {
        // if we are moving in the positive direction, we
        // - have smaller indices in the to view
        // - thus have to start moving from the start of from (we compress the memory)
        // - we move blocks of the smallest dimension (i.e. last dimension) first, and then
        //   go to the larger dimensions.
      }
      IterateRecurse(largeIdx, fromSubView, toSubView);
    }

  public:
    template <typename FV, typename TV>
    void IterateRecurse(std::array<ptrdiff_t, NDim - 1> curIdx, FV fromView, TV toView)
    {
      std::cout << "IterateRecurse: curIdx = " << curIdx << std::endl;
      // First, perform the copy
      size_t copy_size = std::min(toView.extent(NDim - 1), fromView.extent(NDim - 1));
      Kokkos::parallel_for(
          Kokkos::RangePolicy<typename decltype(fromView)::execution_space>(0, copy_size),
          KOKKOS_LAMBDA(const size_t i) {
            std::apply([&](const auto &...args) { toView(args..., i) = fromView(args..., i); }, curIdx);
          });

      // Then, figure out the next large index to copy
      std::array<ptrdiff_t, NDim - 1> nextIdx = curIdx;

      // Check if we have a next index to recurse down
      bool hasNext = (mDirection < 0) ? lowerDimN(NDim - 2, nextIdx, fromView) : raiseDimN(0, nextIdx, fromView);
      if (hasNext) {
        // If we have a next index, recurse down
        IterateRecurse(nextIdx, fromView, toView);
      } else {
        // If we are done, we can return
        return;
      }
    }

  private:
    bool lowerDimN(ptrdiff_t DimN, auto &nextIdx, auto &fromView)
    {
      if (DimN < 0) {
        // We are done, we have iterated through all dimensions
        return false;
      }
      if (nextIdx[DimN] > 0) {
        --nextIdx[DimN];
        return true;
      } else {
        nextIdx[DimN] = (ptrdiff_t)fromView.extent(DimN) - 1; // reset to the last index
        return lowerDimN(DimN - 1, nextIdx, fromView);        // carry to the next dimension
      }
    };

    bool raiseDimN(ptrdiff_t DimN, auto &nextIdx, auto &fromView)
    {
      if (DimN < 0) {
        // We are done, we have iterated through all dimensions
        return false;
      }
      if (nextIdx[DimN] < (ptrdiff_t)fromView.extent(DimN) - 1) {
        ++nextIdx[DimN];
        return true;
      } else {
        nextIdx[DimN] = 0;                             // reset to the first index
        return raiseDimN(DimN - 1, nextIdx, fromView); // carry to the next dimension
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
      if (thisDim < NDim - 1) {
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

#ifdef TEMPLATTEST
#include "TempLat/lattice/ghostcells/ghostbuster_test.h"
#endif

#endif
