#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDAVERAGER_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDAVERAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/rangeiteration/make_tuple_tag.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/measuringtools/averagerhelper.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexgetgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

#include "TempLat/parallel/device_iteration.h"

namespace TempLat
{
  /** @brief A class which averages complex fields.
   *
   *
   * Unit test: ctest -R test-complexfieldaverager
   **/
  template <typename T> class ComplexFieldAverager
  {
  public:
    using vType = typename ComplexGetGetReturnType<T>::type;
    static constexpr bool isComplexValued = IsComplexType<vType>;
    static constexpr size_t size = tuple_size<T>::value;

    static constexpr size_t NDim = GetNDim::get<T>();

    using arrVType = std::array<vType, size>;

    // Put public methods here. These should change very little over time.
    ComplexFieldAverager(const T &pT, SpaceStateType spaceType) : mT(pT), mSpaceType(spaceType)
    {
      mToolBox = mT.ComplexFieldGet(Tag<0>()).getToolBox();
      if (mToolBox == nullptr) throw std::runtime_error("ComplexFieldAverager: ToolBox is null, cannot initialize.");
    }

    arrVType compute()
    {
      // --------------------------------------------------------
      // Reduce the result on the local lattice
      // --------------------------------------------------------

      arrVType localResult{};
      if (mSpaceType == SpaceStateType::Configuration)
        localResult = computeConfigurationSpace();
      else if (mSpaceType == SpaceStateType::Fourier)
        localResult = computeFourierSpace();
      else
        throw std::runtime_error("ComplexFieldAverager: Unknown space type.");

      // --------------------------------------------------------
      // Reduce the result across all processes
      // --------------------------------------------------------

      arrVType selfResult, reducedRes, ret;
      selfResult[0] = localResult.real();
      selfResult[1] = localResult.imag();

      for_in_range<0, size>([&](auto i) {
        reducedRes[i] = mToolBox->mGroup.getBaseComm().computeAllSum(selfResult[i]);
        ret[i] = AveragerHelper<vType, isComplexValued>::normalize(mToolBox, mSpaceType, reducedRes[i]);
      });

      return ret;
    }

    arrVType computeConfigurationSpace()
    {
      AveragerHelper<vType, isComplexValued>::onBeforeAverageFourier(mT.ComplexFieldGet(Tag<0>()), mSpaceType);
      AveragerHelper<vType, isComplexValued>::onBeforeAverageFourier(mT.ComplexFieldGet(Tag<1>()), mSpaceType);

      complex<vType> localResult{};

      const auto mLayout = mToolBox->mLayouts.getConfigSpaceLayout();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx, complex<vType> &update)
      {
        device::apply(
            [&](auto &&...args) {
              auto result = DoEval::eval(mT, args...);
              update.real() += result[0];
              update.imag() += result[1];
            },
            idx);
      };
      device::iteration::reduce("ComplexFieldAverager", mLayout, functor, localResult);

      arrVType a{};
      a[0] = localResult.real();
      a[1] = localResult.imag();
      return a;
    }

    arrVType computeFourierSpace()
    {
      AveragerHelper<vType, isComplexValued>::onBeforeAverageConfiguration(mT.ComplexFieldGet(Tag<0>()), mSpaceType);
      AveragerHelper<vType, isComplexValued>::onBeforeAverageConfiguration(mT.ComplexFieldGet(Tag<1>()), mSpaceType);

      complex<vType> localResult{};

      const LayoutStruct<NDim> mLayout = mToolBox->mLayouts.getFourierSpaceLayout();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx, complex<vType> &update)
      {
        device::apply(
            [&](auto &&...args) {
              device::IdxArray<NDim> global_coord;
              mLayout.putSpatialLocationFromMemoryIndexInto(global_coord, args...);
              if (mLayout.getHermitianPartners().qualify(global_coord) == HermitianRedundancy::negativePartner)
                return; // skip negative partners

              auto result = DoEval::eval(mT, args...);
              update.real() += result[0];
              update.imag() += result[1];
            },
            idx);
      };
      device::iteration::reduce("ComplexFieldAverager", mLayout, functor, localResult);

      arrVType a{};
      a[0] = localResult.real();
      a[1] = localResult.imag();
      return a;
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
    requires HasComplexFieldGet<T>
  auto complexfieldaverage(T instance,
                           SpaceStateType spaceType = IsComplexType<typename ComplexGetGetReturnType<T>::type>
                                                          ? SpaceStateType::Fourier
                                                          : SpaceStateType::Configuration)
  {
    return make_list_from_array(ComplexFieldAverager<T>(instance, spaceType).compute());
  }
} // namespace TempLat

#endif
