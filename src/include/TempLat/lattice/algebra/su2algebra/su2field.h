#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2FIELD_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2FIELD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/field/assignablefieldcollection.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2get.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

#include "TempLat/parallel/device.h"

#include "TempLat/lattice/algebra/su2algebra/su2binaryoperator.h"
#include "TempLat/lattice/algebra/su2algebra/su2commutator.h"

namespace TempLat
{
  /** @brief A class which implements a SU2 field (group).
   *
   *
   * Unit test: ctest -R test-su2field
   **/
  template <size_t _NDim, typename T> class SU2FieldBase
  {
  public:
    // Put public methods here. These should change very little over time.
    static constexpr size_t NDim = _NDim;

    SU2FieldBase(Field<NDim, T> f0, Field<NDim, T> f1, Field<NDim, T> f2, Field<NDim, T> f3)
        : fs{{f0, f1, f2, f3}}, mName("NoName"), mLayout(fs[0].getToolBox()->mLayouts.getConfigSpaceLayout())
    {
    }

    SU2FieldBase(std::string name, device::memory::host_ptr<MemoryToolBox<NDim>> toolBox,
                 LatticeParameters<T> pLatPar = LatticeParameters<T>())
        : fs{{
              Field<NDim, T>(name + "_0", toolBox, pLatPar), //
              Field<NDim, T>(name + "_1", toolBox, pLatPar), //
              Field<NDim, T>(name + "_2", toolBox, pLatPar), //
              Field<NDim, T>(name + "_3", toolBox, pLatPar)  //
          }},
          mName(name), mLayout(toolBox->mLayouts.getConfigSpaceLayout())
    {
      fs[0] = T(1);
      fs[0].updateGhosts();
    }

    template <int N> DEVICE_FORCEINLINE_FUNCTION const auto &SU2Get(Tag<N> t) const { return fs[N]; }

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto &operator()(Tag<M> t) { return fs[M]; }

    template <int M> DEVICE_FORCEINLINE_FUNCTION const auto &operator()(Tag<M> t) const { return fs[M]; }

    template <typename R> void operator=(R &&r)
    {
      fs[0].onBeforeAssignment(r.SU2Get(0_c));
      fs[1].onBeforeAssignment(r.SU2Get(1_c));
      fs[2].onBeforeAssignment(r.SU2Get(2_c));
      fs[3].onBeforeAssignment(r.SU2Get(3_c));

      PreGet::apply(r);

      const auto view0 = fs[0].getView();
      const auto view1 = fs[1].getView();
      const auto view2 = fs[2].getView();
      const auto view3 = fs[3].getView();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx)
      {
        device::apply(
            [&](const auto &...args) {
              auto result = DoEval::eval(r, args...);
              view0(args...) = result[0];
              view1(args...) = result[1];
              view2(args...) = result[2];
              view3(args...) = result[3];
            },
            idx);
      };
      device::iteration::foreach ("SU2ConfigViewAssign", mLayout, functor);

      PostGet::apply(r);

      fs[0].setGhostsAreStale();
      fs[1].setGhostsAreStale();
      fs[2].setGhostsAreStale();
      fs[3].setGhostsAreStale();
    }

    // Recompute c0 from c1,c2,c3 to enforce SU(2) unitarity constraint.
    void unitarize()
    {
      const auto view1 = fs[1].getView();
      const auto view2 = fs[2].getView();
      const auto view3 = fs[3].getView();
      const auto view0 = fs[0].getView();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx)
      {
        device::apply(
            [&](const auto &...args) {
              T c1 = view1(args...);
              T c2 = view2(args...);
              T c3 = view3(args...);
              view0(args...) = sqrt(T(1) - c1 * c1 - c2 * c2 - c3 * c3);
            },
            idx);
      };
      device::iteration::foreach ("SU2Unitarize", mLayout, functor);

      fs[0].setGhostsAreStale();
      fs[0].updateGhosts();
    }

    std::string toString() const { return *mName; }

    DEVICE_FORCEINLINE_FUNCTION
    auto getDx() const { return GetDx::getDx(fs[0]); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getKIR() const { return GetKIR::getKIR(fs[0]); }

    inline auto getToolBox() { return GetToolBox::get(fs[0]); }

    inline void updateGhosts()
    {
      fs[0].updateGhosts();
      fs[1].updateGhosts();
      fs[2].updateGhosts();
      fs[3].updateGhosts();
    }

    template <typename... IDX>
      requires requires(Field<NDim, T> f, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(f, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      device::array<T, 4> result;
      result[0] = fs[0].eval(idx...);
      result[1] = fs[1].eval(idx...);
      result[2] = fs[2].eval(idx...);
      result[3] = fs[3].eval(idx...);
      return result;
    }

    using Getter = SU2Getter;

    static constexpr size_t SHIFTIND = 0;
    static constexpr size_t size = 4;
    static constexpr size_t numberToSkipAsTuple = 0;

  protected:
    device::array<Field<NDim, T>, 4> fs;
    const device::memory::host_string mName;
    LayoutStruct<NDim> mLayout;
  };

  template <size_t NDim, typename T> using SU2Field = SU2FieldBase<NDim, T>;
} // namespace TempLat

#endif
