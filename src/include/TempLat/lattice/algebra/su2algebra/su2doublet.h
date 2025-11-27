#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLET_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLET_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/assignablefieldcollection.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2doubletget.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"

#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** \brief A class which implements su2doublets.
   *
   *
   * Unit test: make test-su2doublet
   **/
  template <size_t _NDim, typename T> class SU2DoubletBase
  {
  public:
    // Put public methods here. These should change very little over time.
    static constexpr size_t NDim = _NDim;

    SU2DoubletBase(Field<NDim, T> f1, Field<NDim, T> f2, Field<NDim, T> f3, Field<NDim, T> f4)
        : fs{{f1, f2, f3, f4}}, mName("NoName"), mLayout(f1.getToolBox()->mLayouts.getConfigSpaceLayout())
    {
    }
    SU2DoubletBase(std::string name, device::memory::host_ptr<MemoryToolBox<NDim>> toolBox,
                   LatticeParameters<T> pLatPar = LatticeParameters<T>())
        : mName(name), fs{{
                           Field<NDim, T>(name + "_0", toolBox, pLatPar), //
                           Field<NDim, T>(name + "_1", toolBox, pLatPar), //
                           Field<NDim, T>(name + "_2", toolBox, pLatPar), //
                           Field<NDim, T>(name + "_3", toolBox, pLatPar)  //
                       }},
          mLayout(toolBox->mLayouts.getConfigSpaceLayout())
    {
    }

    template <int N> DEVICE_FORCEINLINE_FUNCTION const Field<NDim, T> &SU2DoubletGet(Tag<N> t) const { return fs[t]; }

    template <int N, typename... IDX>
      requires requires(Field<NDim, T> f, IDX... idx) { f.get(idx...); }
    DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<N> t, const IDX &...idx) const
    {
      return fs[t].get(idx...);
    }

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto &operator()(Tag<M> t) { return fs[t]; }

    template <typename R> void operator=(R &&r)
    {
      ForLoop(i, 0, size - 1, fs[i].onBeforeAssignment(std::decay_t<R>::Getter::get(r, i)););
      ForLoop(j, 0, size - 1, PreGet::apply(fs[j]));

      const auto views = device::make_tuple(fs[0].getView(), fs[1].getView(), fs[2].getView(), fs[3].getView());

      auto functor = DEVICE_CLASS_LAMBDA(const device::array<size_t, NDim> &idx)
      {
#if defined(__NVCC__)
        std::decay_t<R> __r = r;
#else
        const auto &__r = r;
#endif
        device::apply(
            [&](auto &&...args) {
              DoEval::eval(__r, args...);
              constexpr_for<0, size, 1>([&](auto _i) {
                constexpr size_t i = decltype(_i)::value;
                device::get<i>(views)(args...) = __r.SU2DoubletGet(_i, args...);
              });
            },
            idx);
      };
      device::iteration::foreach ("SU2DoubleConfigViewAssign", mLayout, functor);

      ForLoop(j, 0, size - 1, PostGet::apply(fs[j]));
      ForLoop(j, 0, size - 1, fs[j].setGhostsAreStale());
    }

    template <typename R> void operator+=(R &&r) { (*this) = (*this) + r; }

    std::string toString() const { return mName; }

    device::memory::host_ptr<MemoryToolBox<NDim>> getToolBox() const { return GetToolBox::get(fs[0]); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getDx() const { return GetDx::getDx(fs[0]); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getKIR() const { return GetKIR::getKIR(fs[0]); }

    using Getter = SU2DoubletGetter;
    static constexpr size_t size = 4;

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    const std::string mName;

    device::array<Field<NDim, T>, 4> fs;

    LayoutStruct<NDim> mLayout;
  };

  template <size_t NDim, typename T> using SU2Doublet = SU2DoubletBase<NDim, T>;

  struct SU2DoubletTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
