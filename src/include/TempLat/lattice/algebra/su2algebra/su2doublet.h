#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLET_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLET_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"
#include "TempLat/lattice/field/assignablefieldcollection.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2doubletget.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"

#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A class which implements su2doublets.
   *
   *
   * Unit test: ctest -R test-su2doublet
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

    template <typename... IDX>
      requires requires(Field<NDim, T> f, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(f, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      device::array<T, 4> result;
      result[0] = DoEval::eval(fs[0], idx...);
      result[1] = DoEval::eval(fs[1], idx...);
      result[2] = DoEval::eval(fs[2], idx...);
      result[3] = DoEval::eval(fs[3], idx...);
      return result;
    }

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto &operator()(Tag<M> t) { return fs[t]; }

    template <typename R> void operator=(R &&r)
    {
      fs[0].onBeforeAssignment(std::decay_t<R>::Getter::get(r, 0_c));
      fs[1].onBeforeAssignment(std::decay_t<R>::Getter::get(r, 1_c));
      fs[2].onBeforeAssignment(std::decay_t<R>::Getter::get(r, 2_c));
      fs[3].onBeforeAssignment(std::decay_t<R>::Getter::get(r, 3_c));

      PreGet::apply(r);

      const auto views = device::make_tuple(fs[0].getView(), fs[1].getView(), fs[2].getView(), fs[3].getView());

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx)
      {
        device::apply(
            [&](auto &&...args) {
              auto result = DoEval::eval(r, args...);
              constexpr_for<0, size>([&](auto _i) {
                constexpr size_t i = decltype(_i)::value;
                device::get<i>(views)(args...) = result[i];
              });
            },
            idx);
      };
      device::iteration::foreach ("SU2DoubleConfigViewAssign", mLayout, functor);

      PostGet::apply(r);

      fs[0].setGhostsAreStale();
      fs[1].setGhostsAreStale();
      fs[2].setGhostsAreStale();
      fs[3].setGhostsAreStale();
    }

    template <typename R> void operator+=(R &&r) { (*this) = (*this) + r; }

    std::string toString() const { return *mName; }

    device::memory::host_ptr<MemoryToolBox<NDim>> getToolBox() const { return GetToolBox::get(fs[0]); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getDx() const { return GetDx::getDx(fs[0]); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getKIR() const { return GetKIR::getKIR(fs[0]); }

    using Getter = SU2DoubletGetter;
    static constexpr size_t size = 4;

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    const device::memory::host_string mName;

    device::array<Field<NDim, T>, 4> fs;

    LayoutStruct<NDim> mLayout;
  };

  template <size_t NDim, typename T> using SU2Doublet = SU2DoubletBase<NDim, T>;
} // namespace TempLat

#endif
