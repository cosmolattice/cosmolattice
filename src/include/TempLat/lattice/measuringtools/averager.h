#ifndef TEMPLAT_LATTICE_MEASUREMENTS_AVERAGER_H
#define TEMPLAT_LATTICE_MEASUREMENTS_AVERAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/measuringtools/averagerhelper.h"

#include "TempLat/parallel/device_iteration.h"

namespace TempLat
{

  /** @brief A class which computes the average value of a getter.
   *
   *
   * Unit test: ctest -R test-averager
   **/
  template <typename T> class Averager
  {
  public:
    using vType = typename GetGetReturnType<T>::type;
    static constexpr bool isComplexValued = GetGetReturnType<T>::isComplex;

    static constexpr size_t NDim = GetNDim::get<T>();

    // Put public methods here. These should change very little over time.
    Averager(const T &pT, SpaceStateType spaceType)
      requires requires {
        { pT.getToolBox() } -> std::same_as<device::memory::host_ptr<MemoryToolBox<NDim>>>;
      }
        : mT(pT), mSpaceType(spaceType)
    {
      mToolBox = mT.getToolBox();
      if (mToolBox == nullptr) throw std::runtime_error("Averager: ToolBox is null, cannot initialize.");
    }

    vType compute()
    {
      if (mSpaceType == SpaceStateType::Fourier) {
        AveragerHelper<vType, isComplexValued>::onBeforeAverageFourier(mT, mSpaceType);
      } else if (mSpaceType == SpaceStateType::Configuration) {
        AveragerHelper<vType, isComplexValued>::onBeforeAverageConfiguration(mT, mSpaceType);
      } else
        throw std::runtime_error("Averager: Unknown space type.");

      // --------------------------------------------------------
      // Reduce the result on the local lattice
      // --------------------------------------------------------

      vType localResult{};
      if (mSpaceType == SpaceStateType::Configuration)
        localResult = computeConfigurationSpace();
      else if (mSpaceType == SpaceStateType::Fourier)
        localResult = computeFourierSpace();
      else
        throw std::runtime_error("Averager: Unknown space type.");

      // --------------------------------------------------------
      // Reduce the result across all processes
      // --------------------------------------------------------

      const vType reducedRes = mT.getToolBox()->mGroup.getBaseComm().computeAllSum(localResult);
      return AveragerHelper<vType, isComplexValued>::normalize(mT.getToolBox(), mSpaceType, reducedRes);
    }

    vType computeConfigurationSpace()
    {
      vType localResult{};

      const LayoutStruct<NDim> mLayout = mToolBox->mLayouts.getConfigSpaceLayout();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx, vType &update)
      {
        device::apply([&](auto &&...args) { update += DoEval::eval(mT, args...); }, idx);
      };
      device::iteration::reduce("Averager", mLayout, functor, localResult);

      return localResult;
    }

    vType computeFourierSpace()
    {
      vType localResult{};

      const LayoutStruct<NDim> mLayout = mToolBox->mLayouts.getFourierSpaceLayout();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx, vType &update)
      {
        device::apply(
            [&](auto &&...args) {
              device::IdxArray<NDim> global_coord;
              mLayout.putSpatialLocationFromMemoryIndexInto(global_coord, args...);
              if (mLayout.getHermitianPartners().qualify(global_coord) == HermitianRedundancy::negativePartner)
                return; // skip negative partners

              update += DoEval::eval(mT, args...);
            },
            idx);
      };
      device::iteration::reduce("Averager", mLayout, functor, localResult);

      return localResult;
    }

    std::string toString() const { return "<" + GetString::get(mT) + ">"; }

    /** For measurement objects. */
    auto getToolBox() const { return GetToolBox::get(mT); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    T mT;
    SpaceStateType mSpaceType;

    device::memory::host_ptr<MemoryToolBox<NDim>> mToolBox;
  };

  template <typename T>
    requires(!IsTempLatGettable<0, T> && !std::is_arithmetic_v<T>)
  auto average(T instance, SpaceStateType spaceType = GetGetReturnType<T>::isComplex ? SpaceStateType::Fourier
                                                                                     : SpaceStateType::Configuration)
  {
    return Averager<T>(instance, spaceType).compute();
  }

  template <typename T>
    requires std::is_arithmetic_v<T>
  auto average(T a)
  {
    return a;
  }

  auto average(ZeroType a) { return 0; }

  template <typename T>
  auto getAverager(T instance, SpaceStateType spaceType = GetGetReturnType<T>::isComplex
                                                              ? SpaceStateType::Fourier
                                                              : SpaceStateType::Configuration)
  {
    return Averager<T>(instance, spaceType);
  }

} // namespace TempLat

#endif
