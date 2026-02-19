#ifndef TEMPLAT_LATTICE_MEASUREMENTS_WALLAVERAGER_H
#define TEMPLAT_LATTICE_MEASUREMENTS_WALLAVERAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2025

#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/algebra/helpers/haseval.h"
#include "TempLat/lattice/measuringtools/averagerhelper.h"

#include "TempLat/parallel/device_memory.h"
#include "TempLat/parallel/device_iteration.h"

namespace TempLat
{

  /** @brief A class which computes the average value of a getter over the first n-1 coordinates, returns a vector.
   *
   *
   * Unit test: ctest -R test-averager
   **/
  template <typename T> class WallAverager
  {
  public:
    using vType = typename GetGetReturnType<T>::type;
    static constexpr bool isComplexValued = GetGetReturnType<T>::isComplex;
    static constexpr size_t NDim = GetNDim::get<T>();

    WallAverager(const T &pT, SpaceStateType spaceType)
      requires requires {
        { pT.getToolBox() } -> std::same_as<device::memory::host_ptr<MemoryToolBox<NDim>>>;
      }
        : mT(pT), mSpaceType(spaceType)
    {
      if (mSpaceType != SpaceStateType::Configuration)
        throw AveragerWrongSpace("Wall averager works only in configuration space.");

      mToolBox = mT.getToolBox();
      if (mToolBox == nullptr) throw std::runtime_error("WallAverager: ToolBox is null.");

      const auto layout = mToolBox->mLayouts.getConfigSpaceLayout();
      const auto localSizes = layout.getLocalSizes();
      mLocalStarts = layout.getLocalStarts();
      nGhosts = layout.getNGhosts();

      // starts = absolute start positions; stops = counts (sizes)
      // getLocalKokkosPolicy computes: kokkos_stop[d] = starts[d] + stops[d]
      for (size_t d = 0; d < NDim; ++d) {
        mStartIteration[d] = nGhosts;
        mStopIteration[d] = localSizes[d];
        mLocalSizes[d] = localSizes[d];
      }

      // Allocate device result buffer (sized for max local dimension)
      size_t maxLocalSize = 0;
      for (size_t d = 0; d < NDim; ++d)
        maxLocalSize = device::max(maxLocalSize, static_cast<size_t>(localSizes[d]));
      mMaxLocalSize = maxLocalSize;
      mDeviceResult = device::memory::NDView<1, vType>("wallResult", maxLocalSize);

      // Host workspace: NDim vectors, each of global size N[t]
      for (size_t t = 0; t < NDim; ++t)
        mWorkspace[t].assign(mToolBox->mNGridPointsVec[t], vType{});
    }

    void compute()
    {
      AveragerHelper<vType, isComplexValued>::onBeforeAverageConfiguration(mT, mSpaceType);
      computeConfigurationSpace();

      // MPI Allreduce per dimension (vector overload, in-place)
      for (size_t t = 0; t < NDim; ++t)
        mToolBox->mGroup.getBaseComm().Allreduce(&mWorkspace[t], MPI_SUM);

      // Normalize by codimension
      for (size_t t = 0; t < NDim; ++t) {
        vType codim = 1;
        for (size_t j = 0; j < NDim; ++j)
          if (j != t) codim *= mToolBox->mNGridPointsVec[j];
        for (auto &val : mWorkspace[t])
          val /= codim;
      }
    }

    void computeConfigurationSpace()
    {
      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx, vType &update)
      {
        device::apply([&](auto &&...args) { update += DoEval::eval(mT, args...); }, idx);
      };

      for (size_t t = 0; t < NDim; ++t) {
        // Zero the host workspace for this dimension
        std::fill(mWorkspace[t].begin(), mWorkspace[t].end(), vType{});

        for (size_t local_j = 0; local_j < static_cast<size_t>(mLocalSizes[t]); ++local_j) {
          // Restrict dimension t to a single slice
          // starts = absolute positions, stops = counts
          auto cur_start = mStartIteration;
          auto cur_stop = mStopIteration;
          cur_start[t] = nGhosts + local_j;
          cur_stop[t] = 1; // count of 1 along dimension t

          // Reduce this hyperplane into a single element of the device buffer
          device::iteration::reduce("WallAverager", cur_start, cur_stop, functor,
                                    device::memory::subview(mDeviceResult, local_j));
        }

        // Copy device results to host via appropriately-sized subview
        size_t curLocalSize = static_cast<size_t>(mLocalSizes[t]);
        std::vector<vType> localResults(curLocalSize, vType{});
        if (curLocalSize == mMaxLocalSize) {
          device::memory::copyDeviceToHost(mDeviceResult, localResults.data());
        } else {
          auto sliceView = device::memory::subview(mDeviceResult, std::pair<size_t, size_t>(0, curLocalSize));
          device::memory::copyDeviceToHost(sliceView, localResults.data());
        }

        // Map local indices to global workspace positions
        for (size_t local_j = 0; local_j < curLocalSize; ++local_j) {
          size_t global_j = mLocalStarts[t] + local_j;
          mWorkspace[t][global_j] = localResults[local_j];
        }
      }
    }

    std::string toString() const { return "<" + GetString::get(mT) + ">_walls"; }

    /** For measurement objects. */
    inline auto getToolBox() const { return GetToolBox::get(mT); }

    auto getWall(size_t dim) const { return mWorkspace[dim]; }

  private:
    T mT;
    SpaceStateType mSpaceType;

    device::memory::host_ptr<MemoryToolBox<NDim>> mToolBox;

    device::array<device::Idx, NDim> mStartIteration{};
    device::array<device::Idx, NDim> mStopIteration{};
    device::IdxArray<NDim> mLocalSizes{};
    device::IdxArray<NDim> mLocalStarts{};

    device::memory::NDView<1, vType> mDeviceResult;
    size_t mMaxLocalSize;

    std::array<std::vector<vType>, NDim> mWorkspace;

    size_t nGhosts;
  };

  template <typename T>
    requires HasEvalMethod<T>
  auto wallAverager(T instance, SpaceStateType spaceType = SpaceStateType::Configuration)
  {
    return WallAverager<T>(instance, spaceType);
  }

  auto wallAverager(ZeroType a) { return 0; }

} // namespace TempLat

#endif
