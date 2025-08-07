#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDAVERAGER_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDAVERAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/rangeiteration/make_tuple_tag.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/measuringtools/averagerhelper.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexgetgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

namespace TempLat
{
  /** \brief A class which averages complex fields.
   *
   *
   * Unit test: make test-complexfieldaverager
   **/
  template <typename T> class ComplexFieldAverager
  {
  public:
    using vType = typename ComplexGetGetReturnType<T>::type;
    static constexpr bool isComplexValued = IsComplexType<vType>;
    static constexpr size_t size = tuple_size<T>::value;

    static constexpr size_t NDim = T::NDim;

    using arrVType = std::array<vType, size>;

    /* Put public methods here. These should change very little over time. */
    ComplexFieldAverager(const T &pT, SpaceStateType spaceType) : mT(pT), mSpaceType(spaceType)
    {
      mToolBox = mT.ComplexFieldGet(Tag<0>()).getToolBox();
      if (mToolBox == nullptr) throw std::runtime_error("ComplexFieldAverager: ToolBox is null, cannot initialize.");

      if (mSpaceType == SpaceStateType::Configuration) {
        const auto layout = mToolBox->mLayouts.getConfigSpaceLayout();
        const auto localSizes = layout.getLocalSizes();
        const size_t nGhosts = layout.getNGhosts();

        for (size_t d = 0; d < NDim; ++d) {
          start_iteration[d] = nGhosts;
          stop_iteration[d] = start_iteration[d] + localSizes[d];
        }
      } else if (mSpaceType == SpaceStateType::Fourier) {
        const auto layout = mToolBox->mLayouts.getFourierSpaceLayout();
        const auto localSizes = layout.getLocalSizes();
        for (size_t d = 0; d < NDim; ++d) {
          start_iteration[d] = 0;
          stop_iteration[d] = start_iteration[d] + localSizes[d];
        }
      } else {
        throw std::runtime_error("ComplexFieldAverager: Unknown space type.");
      }
    }

    arrVType compute()
    {
      if (mSpaceType == SpaceStateType::Fourier) {
        AveragerHelper<vType, isComplexValued>::onBeforeAverageConfiguration(mT.ComplexFieldGet(Tag<0>()), mSpaceType);
        AveragerHelper<vType, isComplexValued>::onBeforeAverageConfiguration(mT.ComplexFieldGet(Tag<1>()), mSpaceType);
      } else if (mSpaceType == SpaceStateType::Configuration) {
        AveragerHelper<vType, isComplexValued>::onBeforeAverageFourier(mT.ComplexFieldGet(Tag<0>()), mSpaceType);
        AveragerHelper<vType, isComplexValued>::onBeforeAverageFourier(mT.ComplexFieldGet(Tag<1>()), mSpaceType);
      } else
        throw std::runtime_error("ComplexFieldAverager: Unknown space type.");

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
      complex<vType> localResult{};

      if constexpr (NDim > 1) {
        auto functor = KOKKOS_CLASS_LAMBDA(const device::IdxArray<NDim> &idx, complex<vType> &update)
        {
          device::apply(
              [&](auto &&...args) {
                DoEval::eval(mT, args...);
                update.real() += mT.ComplexFieldGet(Tag<0>(), args...);
                update.imag() += mT.ComplexFieldGet(Tag<1>(), args...);
              },
              idx);
        };
        Kokkos::parallel_reduce("ComplexFieldAverager",                                                     //
                                Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(start_iteration, stop_iteration), //
                                KokkosNDLambdaWrapperReduction<NDim, decltype(functor), complex<vType>>(functor),
                                localResult);
      } else if constexpr (NDim == 1) {
        auto functor = KOKKOS_CLASS_LAMBDA(const device::Idx &idx, complex<vType> &update)
        {
          DoEval::eval(mT, idx);
          update.real() += mT.ComplexFieldGet(Tag<0>(), idx);
          update.imag() += mT.ComplexFieldGet(Tag<1>(), idx);
        };
        Kokkos::parallel_reduce("ComplexFieldAverager", //
                                Kokkos::RangePolicy(start_iteration[0], stop_iteration[0]), functor, localResult);
      } else {
        static_assert(NDim > 0);
      }

      arrVType a{};
      a[0] = localResult.real();
      a[1] = localResult.imag();
      return a;
    }

    arrVType computeFourierSpace()
    {
      complex<vType> localResult{};

      const LayoutStruct<NDim> mLayout = mToolBox->mLayouts.getFourierSpaceLayout();

      if constexpr (NDim > 1) {
        auto functor = KOKKOS_CLASS_LAMBDA(const device::IdxArray<NDim> &idx, complex<vType> &update)
        {
          device::apply(
              [&](auto &&...args) {
                Kokkos::Array<ptrdiff_t, NDim> global_coord;
                mLayout.putSpatialLocationFromMemoryIndexInto(global_coord, args...);
                if (mLayout.getHermitianPartners().qualify(global_coord) == HermitianRedundancy::negativePartner)
                  return; // skip negative partners

                DoEval::eval(mT, args...);
                update.real() += mT.ComplexFieldGet(Tag<0>(), args...);
                update.imag() += mT.ComplexFieldGet(Tag<1>(), args...);
              },
              idx);
        };
        Kokkos::parallel_reduce("ComplexFieldAverager",                                                     //
                                Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(start_iteration, stop_iteration), //
                                KokkosNDLambdaWrapperReduction<NDim, decltype(functor), complex<vType>>(functor),
                                localResult);
      } else if constexpr (NDim == 1) {
        auto functor = KOKKOS_CLASS_LAMBDA(const device::Idx &idx, complex<vType> &update)
        {
          DoEval::eval(mT, idx);
          update.real() += mT.ComplexFieldGet(Tag<0>(), idx);
          update.imag() += mT.ComplexFieldGet(Tag<1>(), idx);
        };
        Kokkos::parallel_reduce("ComplexFieldAverager", //
                                Kokkos::RangePolicy(start_iteration[0], stop_iteration[0]), functor, localResult);
      } else {
        static_assert(NDim > 0);
      }

      arrVType a{};
      a[0] = localResult.real();
      a[1] = localResult.imag();
      return a;
    }
    /*
        arrVType computeFourierSpace()
        {
          auto toolBox = mT.ComplexFieldGet(0_c).getToolBox();
          auto it = mT.ComplexFieldGet(0_c).getToolBox()->itP();
          arrVType mWorkspace{};
          ForLoop(i, 0, size - 1,
                  (AveragerHelper<vType, isComplexValued>::onBeforeAverageFourier(mT.ComplexFieldGet(i), mSpaceType)));

          ptrdiff_t i = 0;

          for (it.begin(); it.end(); ++it) {
            if (toolBox->mLayouts.getFourierSpaceLayout().getHermitianPartners()->qualify(it.getVec()) !=
                HermitianRedundancy::negativePartner) {
              i = it();

              DoEval::eval(mT, i);
              ForLoop(j, 0, 1, mWorkspace[j] += mT.ComplexFieldGet(j, i));
            }
          }
          return mWorkspace;
        }
    */

    std::string toString() const { return "<" + GetString::get(mT) + ">"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    T mT;
    SpaceStateType mSpaceType;

    std::shared_ptr<MemoryToolBox<NDim>> mToolBox;

    device::IdxArray<NDim> start_iteration{};
    device::IdxArray<NDim> stop_iteration{};
  };

  class ComplexFieldAveragerTester
  {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
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
