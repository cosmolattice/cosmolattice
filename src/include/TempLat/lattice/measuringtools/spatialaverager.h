#ifndef TEMPLAT_LATTICE_MEASUREMENTS_SPATIALAVERAGER_H
#define TEMPLAT_LATTICE_MEASUREMENTS_SPATIALAVERAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/util/tdd/tdd.h"

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/measuringtools/averagerhelper.h"

namespace TempLat
{
  /** \brief A class which computes the average value of a getter over the first n-1 coordinates, returns a vector.
   *
   * Unit test: make test-spatialaverager
   **/
  template <typename T> class SpatialAverager
  {
  public:
    using vType = typename GetGetReturnType<T>::type;

    // TODO (Franz)
    static constexpr size_t NDim = T::NDim;

    /* Put public methods here. These should change very little over time. */
    SpatialAverager(const T &pT, SpaceStateType spaceType) : mT(pT), mSpaceType(spaceType)
    {
      if (mSpaceType != SpaceStateType::Configuration)
        throw AveragerWrongSpace("Spatial averager works only in configuration space.");

      mToolBox = mT.getToolBox();
      if (mToolBox == nullptr) throw std::runtime_error("Averager: ToolBox is null, cannot initialize.");

      const auto layout = mToolBox->mLayouts.getConfigSpaceLayout();
      const auto localSizes = layout.getLocalSizes();
      nGhosts = layout.getNGhosts();

      for (size_t d = 0; d < NDim - 1; ++d) {
        start_iteration[d] = nGhosts;
        stop_iteration[d] = start_iteration[d] + localSizes[d];
      }

      localResult =
          Kokkos::View<vType *, Kokkos::DefaultExecutionSpace>("localResult", stop_iteration[0] - start_iteration[0]);
    }

    std::vector<vType> compute()
    {
      // We use the fact that the last dimension is never split in the parallelization

      if (mSpaceType != SpaceStateType::Configuration)
        throw AveragerWrongSpace("Spatial averager works only in configuration space.");

      std::vector<vType> selfResult = computeConfigurationSpace();

      std::vector<vType> reducedRes;
      for (auto x : selfResult)
        reducedRes.emplace_back(mT.getToolBox()->mGroup.getBaseComm().computeAllSum(x));

      auto tmpDims = mT.getToolBox()->mNGridPointsVec;
      vType spatialVolume = 1;

      for (size_t i = 0; i < tmpDims.size() - 1; ++i)
        spatialVolume *= tmpDims[i];
      for (auto &x : reducedRes)
        x /= spatialVolume;

      return reducedRes;
    }

    std::vector<vType> computeConfigurationSpace()
    {
      std::vector<vType> result(mT.getToolBox()->mNGridPointsVec.back(), 0);
      // unmanaged view of the local result
      KokkosNDViewUnmanaged<1, vType> localResultHostView(result.data(), stop_iteration[0] - start_iteration[0]);

      if constexpr (NDim > 2) {
        for (uint cur_lidx = nGhosts; cur_lidx < result.size() + nGhosts; ++cur_lidx) {
          auto functor = KOKKOS_CLASS_LAMBDA(const device::IdxArray<NDim - 1> &idx, vType &update)
          {
            device::apply(
                [&](auto &&...args) {
                  DoEval::eval(mT, args..., cur_lidx);
                  update += mT.get(args..., cur_lidx);
                },
                idx);
          };
          Kokkos::parallel_reduce("Averager",                                                                     //
                                  Kokkos::MDRangePolicy<Kokkos::Rank<NDim - 1>>(start_iteration, stop_iteration), //
                                  KokkosNDLambdaWrapperReduction<NDim - 1, decltype(functor)>(functor),
                                  Kokkos::subview(localResult, cur_lidx - nGhosts));
        }
      } else if constexpr (NDim == 2) {
        Kokkos::parallel_for(
            TeamPolicy(result.size(), Kokkos::AUTO()), KOKKOS_CLASS_LAMBDA(auto team) {
              const auto cur_lidx = nGhosts + team.league_rank();
              auto functor = [&](const device::Idx &idx, vType &update) {
                DoEval::eval(mT, idx, cur_lidx);
                update += mT.get(idx, cur_lidx);
              };
              Kokkos::parallel_reduce("Averager", //
                                      Kokkos::TeamThreadRange(team, start_iteration[0], stop_iteration[0]), functor,
                                      Kokkos::subview(localResult, cur_lidx - nGhosts));
            });
      } else if constexpr (NDim == 1) {
        // just copy the values.
        Kokkos::parallel_for(
            "SpatialAverager",
            Kokkos::RangePolicy<Kokkos::DefaultExecutionSpace>(start_iteration[0], stop_iteration[0]),
            KOKKOS_CLASS_LAMBDA(const device::Idx &idx) { localResult(idx - start_iteration[0]) = mT.get(idx); });
      }
      Kokkos::deep_copy(localResultHostView, localResult);

      return result;
    }

    std::string toString() const { return "<" + GetString::get(mT) + ">_spat"; }

    /** For measurement objects. */
    inline std::shared_ptr<MemoryToolBox<NDim>> getToolBox() const { return GetToolBox::get(mT); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    T mT;
    SpaceStateType mSpaceType;

    std::shared_ptr<MemoryToolBox<NDim>> mToolBox;

    Kokkos::Array<device::Idx, NDim - 1> start_iteration{};
    Kokkos::Array<device::Idx, NDim - 1> stop_iteration{};

    Kokkos::View<vType *, Kokkos::DefaultExecutionSpace> localResult;

    size_t nGhosts;
  };

  template <typename T>
    requires HasGetMethod<T>
  auto spatialAverage(T instance, SpaceStateType spaceType = SpaceStateType::Configuration)
  {
    return SpatialAverager<T>(instance, spaceType).compute();
  }

  auto spatialAverage(ZeroType a) { return 0; }

  struct SpatialAveragerTester {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif
