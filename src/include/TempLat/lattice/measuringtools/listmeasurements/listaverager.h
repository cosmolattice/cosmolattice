#ifndef TEMPLAT_LATTICE_MEASUREMENTS_LISTMEASUREMENTS_LISTAVERAGER_H
#define TEMPLAT_LATTICE_MEASUREMENTS_LISTMEASUREMENTS_LISTAVERAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/rangeiteration/make_tuple_tag.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/measuringtools/averagerhelper.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

#include "TempLat/lattice/algebra/helpers/getndim.h"

namespace TempLat
{
  /** @brief A class which implements the average for list objects.
   *
   *
   * Unit test: ctest -R test-listaverager
   **/
  template <typename T> class ListAverager
  {
  public:
    using memberType = decltype(GetComponent::get(std::declval<T &>(), Tag<0>()));
    using vType = GetGetReturnType<memberType>::type;
    static constexpr bool isComplexValued = GetGetReturnType<memberType>::isComplex;
    static constexpr size_t size = tuple_size<T>::value;
    static constexpr size_t NDim = GetNDim::get<memberType>();
    using arrVType = std::array<vType, size>;

    // Put public methods here. These should change very little over time.
    ListAverager(const T &pT, SpaceStateType spaceType) : mT(pT), mSpaceType(spaceType) {}

    /* operator vType()
     {
         return compute();
     }*/

    arrVType compute()
    {
      arrVType selfResult = mSpaceType == SpaceStateType::Fourier ? computeFourierSpace() : computeConfigurationSpace();
      auto toolBox = GetComponent::get(mT, Tag<0>()).getToolBox();

      arrVType reducedRes, ret; //= mT.getToolBox()->mGroup.getBaseComm().computeAllSum(selfResult);

      for_in_range<0, size>([&](auto i) {
        reducedRes[i] = toolBox->mGroup.getBaseComm().computeAllSum(selfResult[i]);
        ret[i] = AveragerHelper<vType, isComplexValued>::normalize(toolBox, mSpaceType, reducedRes[i]);
      });

      return ret;
    }

    arrVType computeConfigurationSpace()
    {
      // TODO: THIS IS STILL CL1.0
      ptrdiff_t i = 0;
      auto &it = GetComponent::get(mT, Tag<0>()).getToolBox()->itX();
      arrVType mWorkspace{};
      for_each(mT,
               [&](auto x) { AveragerHelper<vType, isComplexValued>::onBeforeAverageConfiguration(x, mSpaceType); });

      auto list = make_tuple_tag<size>([&](auto j) { return GetComponent::get(mT, j); });

      for (it.begin(); it.end(); ++it) {
        i = it();
        for_in_range<0, size>([&](auto j) { mWorkspace[j] += DoEval::eval(GetComponent::get(list, j), i); });
      }
      return mWorkspace;
    }

    arrVType computeFourierSpace()
    {
      auto toolBox = GetComponent::get(mT, Tag<0>()).getToolBox();
      auto &it = toolBox->itP();
      arrVType mWorkspace{};
      for_each(mT, [&](auto x) { AveragerHelper<vType, isComplexValued>::onBeforeAverageFourier(x, mSpaceType); });

      auto list = make_tuple_tag<size>([&](auto j) { return GetComponent::get(mT, j); });

      for (it.begin(); it.end(); ++it) {
        if (toolBox->mLayouts.getFourierSpaceLayout().getHermitianPartners()->qualify(it.getVec()) !=
            HermitianRedundancy::negativePartner) {
          for_in_range<0, size>([&](auto j) { mWorkspace[j] += DoEval::eval(GetComponent::get(list, j), it()); });
        }
      }
      return mWorkspace;
    }

    std::string toString() const { return "<" + GetString::get(mT) + ">"; }

    /** For measurement objects. */
    inline auto getToolBox() const { return GetToolBox::get(mT); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    T mT;
    SpaceStateType mSpaceType;

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  template <typename T>
  auto listaverage(
      T instance,
      SpaceStateType spaceType = GetGetReturnType<decltype(GetComponent::get(std::declval<T &>(), Tag<0>()))>::isComplex
                                     ? SpaceStateType::Fourier
                                     : SpaceStateType::Configuration)
  {
    return ListAverager<T>(instance, spaceType).compute();
  }

  template <typename T>
    requires IsTempLatGettable<0, T>
  auto average(T instance, SpaceStateType spaceType =
                               GetGetReturnType<decltype(GetComponent::get(std::declval<T &>(), Tag<0>()))>::isComplex
                                   ? SpaceStateType::Fourier
                                   : SpaceStateType::Configuration)
  {
    return make_list_from_array(ListAverager<T>(instance, spaceType).compute());
  }

#ifdef TEMPLATTEST
template<typename T>
  struct ListAveragerTester
  {
  public:
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
