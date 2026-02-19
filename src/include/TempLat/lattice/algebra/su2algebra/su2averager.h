#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2AVERAGER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2AVERAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/rangeiteration/make_tuple_tag.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/assignabletuple.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/measuringtools/averagerhelper.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/operators/power.h"

#include "TempLat/parallel/device_iteration.h"

namespace TempLat
{
  /** @brief An averager for su2. Allows to take into account cached operations consistently.
   *
   * Unit test: ctest -R test-su2averager
   **/
  template <typename T> class SU2Averager
  {
  public:
    // Put public methods here. These should change very little over time.
    using vType = typename SU2GetGetReturnType<T>::type;
    static constexpr bool isComplexValued = IsComplexType<vType>;
    static constexpr size_t size = tuple_size<T>::value;

    static constexpr size_t NDim = TempLat::GetNDim::get<T>();

    using arrVType = std::array<vType, size>;

    SU2Averager(const T &pT, SpaceStateType spaceType) : mT(pT), mSpaceType(spaceType)
    {
      mToolBox = mT.SU2Get(0_c).getToolBox();
      if (mToolBox == nullptr) throw std::runtime_error("SU2Averager: ToolBox is null, cannot initialize.");
    }

    arrVType compute()
    {
      arrVType selfResult = mSpaceType == SpaceStateType::Fourier ? computeFourierSpace() : computeConfigurationSpace();
      auto toolBox = mT.SU2Get(0_c).getToolBox();

      arrVType reducedRes, ret; //= mT.getToolBox()->mGroup.getBaseComm().computeAllSum(selfResult);

      for_in_range<0, size>([&](auto i) {
        reducedRes[i] = toolBox->mGroup.getBaseComm().computeAllSum(selfResult[i]);
        ret[i] = AveragerHelper<vType, isComplexValued>::normalize(toolBox, mSpaceType, reducedRes[i]);
      });

      return ret;
    }

    class su2result : public device::array<vType, size>
    {
    public:
      DEVICE_FUNCTION
      auto &operator+=(const su2result &other)
      {
        for (size_t i = 0; i < size; ++i) {
          (*this)[i] += other[i];
        }
        return *this;
      }
    };

    arrVType computeConfigurationSpace()
    {
      su2result localResult{};

      ForLoop(i, 0, size - 1,
              (AveragerHelper<vType, isComplexValued>::onBeforeAverageConfiguration(mT.SU2Get(i), mSpaceType)));

      const auto mLayout = mToolBox->mLayouts.getConfigSpaceLayout();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx, su2result &update)
      {
        device::apply(
            [&](auto &&...args) {
              auto result = DoEval::eval(mT, args...);
              constexpr_for<0, size>([&](auto j) { update[j] += result[j]; });
            },
            idx);
      };
      device::iteration::reduce("SU2Averager", mLayout, functor, localResult);

      arrVType _localResult;
      for_in_range<0, size>([&](auto i) { _localResult[i] = localResult[i]; });
      return _localResult;
    }

    arrVType computeFourierSpace()
    {
      AveragerHelper<vType, isComplexValued>::onBeforeAverageConfiguration(mT.SU2Get(0_c), mSpaceType);
      AveragerHelper<vType, isComplexValued>::onBeforeAverageConfiguration(mT.SU2Get(1_c), mSpaceType);

      su2result localResult{};

      const LayoutStruct<NDim> mLayout = mToolBox->mLayouts.getFourierSpaceLayout();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx, su2result &update)
      {
        device::apply(
            [&](auto &&...args) {
              device::IdxArray<NDim> global_coord;
              mLayout.putSpatialLocationFromMemoryIndexInto(global_coord, args...);
              if (mLayout.getHermitianPartners().qualify(global_coord) == HermitianRedundancy::negativePartner)
                return; // skip negative partners

              auto result = DoEval::eval(mT, args...);
              constexpr_for<0, size>([&](auto j) { update[j] += result[j]; });
            },
            idx);
      };
      device::iteration::reduce("ComplexFieldAverager", mLayout, functor, localResult);

      arrVType _localResult;
      for_in_range<0, size>([&](auto i) { _localResult[i] = localResult[i]; });
      return _localResult;
    }

    std::string toString() const { return "<" + GetString::get(mT) + ">"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    T mT;
    SpaceStateType mSpaceType;

    device::memory::host_ptr<MemoryToolBox<NDim>> mToolBox;
  };

  template <typename T>
    requires HasSU2Get<T>
  auto su2average(T instance, SpaceStateType spaceType = IsComplexType<typename SU2GetGetReturnType<T>::type>
                                                             ? SpaceStateType::Fourier
                                                             : SpaceStateType::Configuration)
  {
    return make_list_from_array(SU2Averager<T>(instance, spaceType).compute());
  }
} // namespace TempLat

#endif
