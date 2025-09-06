#ifndef TEMPLAT_LATTICE_MEASUREMENTS_MAXIMUM_H
#define TEMPLAT_LATTICE_MEASUREMENTS_MAXIMUM_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/measuringtools/averagerhelper.h"

#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** \brief A class which computes the maximum value of a getter.
   *
   *
   * Unit test: make test-maximum
   **/
  MakeException(MaximumWrongSpace);

  template <typename T> class Maximum
  {
  public:
    typedef typename GetGetReturnType<T>::type vType;

    static constexpr size_t NDim = GetNDim::get<T>();

    /* Put public methods here. These should change very little over time. */
    Maximum(const T &pT, SpaceStateType spaceType)
        : mT(pT), mSpaceType(spaceType), mToolBox(mT.getToolBox()), mLayout(mToolBox->mLayouts.getConfigSpaceLayout())
    {
      if (mSpaceType != SpaceStateType::Configuration) throw(MaximumWrongSpace("Maximum works only in real space."));
    }

    vType compute()
    {
      if (mSpaceType != SpaceStateType::Configuration) throw(MaximumWrongSpace("Maximum works only in real space."));
      onBeforeAverageConfiguration(mT);

      // --------------------------------------------------------
      // Reduce the result on the local lattice
      // --------------------------------------------------------

      vType localResult{};
      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx, vType &update)
      {
        device::apply(
            [&](auto &&...args) {
              DoEval::eval(mT, args...);
              update = Kokkos::max(mT.get(args...), update);
            },
            idx);
      };
      Kokkos::parallel_reduce("Averager",                                                              //
                              device::getLocalKokkosPolicy(mToolBox->mLayouts.getConfigSpaceLayout()), //
                              KokkosNDLambdaWrapperReduction<NDim, decltype(functor), vType>(functor),
                              Kokkos::Max<vType>(localResult));

      // --------------------------------------------------------
      // Reduce the result across all processes
      // --------------------------------------------------------

      const vType reducedRes = mT.getToolBox()->mGroup.getBaseComm().computeAllMax(localResult);
      return reducedRes;
    }

    template <typename R> static void onBeforeAverageConfiguration(R &&pT)
    {
      /* likewise, make sure we are in configuration space (here the FFT may be fired!). */
      ConfirmSpace::apply(pT, pT.getToolBox()->mLayouts.getConfigSpaceLayout(), SpaceStateType::Configuration);
      GhostsHunter::apply(pT);
    }
    std::string toString() const { return "max(" + GetString::get(mT) + ")"; }

    /** For measurement objects. */
    inline std::shared_ptr<MemoryToolBox<NDim>> getToolBox() const { return GetToolBox::get(mT); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    T mT;
    SpaceStateType mSpaceType;

    std::shared_ptr<MemoryToolBox<NDim>> mToolBox;

    LayoutStruct<NDim> mLayout;

    Kokkos::Array<device::Idx, NDim> start_iteration{};
    Kokkos::Array<device::Idx, NDim> stop_iteration{};
  };

  template <typename T>
    requires(!IsTempLatGettable<0, T>)
  auto max(T instance, SpaceStateType spaceType = GetGetReturnType<T>::isComplex ? SpaceStateType::Fourier
                                                                                 : SpaceStateType::Configuration)
  {
    return Maximum<T>(instance, spaceType).compute();
  }

  auto max(ZeroType a) { return 0; }

  struct MaximumTester {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
