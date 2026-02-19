#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETAVERAGER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETAVERAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
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
#include "TempLat/lattice/algebra/su2algebra/helpers/su2doubletgetgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

#include "TempLat/parallel/device_iteration.h"

namespace TempLat
{
  /** @brief An averager specialised for SU2Doublet. Allows to take into account cached operations consistently.
   *
   *
   * Unit test: ctest -R test-su2doubletaverager
   **/
  template <typename T> class SU2DoubletAverager
  {
  public:
    using vType = typename SU2DoubletGetGetReturnType<T>::type;
    static constexpr bool isComplexValued = IsComplexType<vType>;
    static constexpr size_t size = tuple_size<T>::value;

    static constexpr size_t NDim = TempLat::GetNDim::get<T>();

    using arrVType = std::array<vType, size>;

    // Put public methods here. These should change very little over time.
    SU2DoubletAverager(const T &pT, SpaceStateType spaceType) : mT(pT), mSpaceType(spaceType)
    {
      mToolBox = mT.SU2DoubletGet(0_c).getToolBox();
      if (mToolBox == nullptr) throw std::runtime_error("SU2Averager: ToolBox is null, cannot initialize.");
    }

    arrVType compute()
    {
      arrVType selfResult = mSpaceType == SpaceStateType::Fourier ? computeFourierSpace() : computeConfigurationSpace();
      auto toolBox = mT.SU2DoubletGet(0_c).getToolBox();

      arrVType reducedRes, ret; //= mT.getToolBox()->mGroup.getBaseComm().computeAllSum(selfResult);

      for_in_range<0, size>([&](auto i) {
        reducedRes[i] = toolBox->mGroup.getBaseComm().computeAllSum(selfResult[i]);
        ret[i] = AveragerHelper<vType, isComplexValued>::normalize(toolBox, mSpaceType, reducedRes[i]);
      });

      return ret;
    }

    class su2doubletresult : public device::array<vType, size>
    {
    public:
      DEVICE_FUNCTION
      auto &operator+=(const su2doubletresult &other)
      {
        for (size_t i = 0; i < size; ++i) {
          (*this)[i] += other[i];
        }
        return *this;
      }
    };

    arrVType computeConfigurationSpace()
    {
      su2doubletresult localResult{};

      ForLoop(i, 0, size - 1,
              (AveragerHelper<vType, isComplexValued>::onBeforeAverageConfiguration(mT.SU2DoubletGet(i), mSpaceType)));

      const auto mLayout = mToolBox->mLayouts.getConfigSpaceLayout();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx, su2doubletresult &update)
      {
        device::apply(
            [&](auto &&...args) {
              auto result = DoEval::eval(mT, args...);
              constexpr_for<0, size>([&](auto j) { update[j] += result[j]; });
            },
            idx);
      };
      device::iteration::reduce("SU2DoubletAverager", mLayout, functor, localResult);

      arrVType _localResult;
      for_in_range<0, size>([&](auto i) { _localResult[i] = localResult[i]; });
      return _localResult;
    }

    arrVType computeFourierSpace()
    {
      su2doubletresult localResult{};

      ForLoop(i, 0, size - 1,
              (AveragerHelper<vType, isComplexValued>::onBeforeAverageFourier(mT.SU2DoubletGet(i), mSpaceType)));

      const LayoutStruct<NDim> mLayout = mToolBox->mLayouts.getFourierSpaceLayout();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx, su2doubletresult &update)
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

      device::iteration::reduce("SU2DoubletAverager", mLayout, functor, localResult);

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
    requires HasSU2DoubletGet<T>
  auto su2doubletaverage(T instance,
                         SpaceStateType spaceType = IsComplexType<typename SU2DoubletGetGetReturnType<T>::type>
                                                        ? SpaceStateType::Fourier
                                                        : SpaceStateType::Configuration)
  {
    return make_list_from_array(SU2DoubletAverager<T>(instance, spaceType).compute());
  }
} // namespace TempLat

#endif
