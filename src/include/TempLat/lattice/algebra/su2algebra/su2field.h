#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2FIELD_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2FIELD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/assignablefieldcollection.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2get.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/operators/squareroot.h"

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

    SU2FieldBase(Field<NDim, T> f1, Field<NDim, T> f2, Field<NDim, T> f3)
        : fs{{f1, f2, f3}}, mName("NoName"), mLayout(fs[0].getToolBox()->mLayouts.getConfigSpaceLayout())
    {
    }

    SU2FieldBase(std::string name, device::memory::host_ptr<MemoryToolBox<NDim>> toolBox,
                 LatticeParameters<T> pLatPar = LatticeParameters<T>())
        : fs{{
              Field<NDim, T>(name + "_1", toolBox, pLatPar), //
              Field<NDim, T>(name + "_2", toolBox, pLatPar), //
              Field<NDim, T>(name + "_3", toolBox, pLatPar)  //
          }},
          mName(name), mLayout(toolBox->mLayouts.getConfigSpaceLayout())
    {
    }

    DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<0> t) const
    {
      return sqrt(T(1) - pow<2>(fs[0]) - pow<2>(fs[1]) - pow<2>(fs[2]));
    }
    template <int N> DEVICE_FORCEINLINE_FUNCTION const auto &SU2Get(Tag<N> t) const { return fs[N - 1]; }

    DEVICE_FORCEINLINE_FUNCTION
    auto operator()(Tag<0> t) const { return sqrt(T(1) - pow<2>(fs[0]) - pow<2>(fs[1]) - pow<2>(fs[2])); }

    template <int M>
      requires(M > 0)
    DEVICE_FORCEINLINE_FUNCTION auto &operator()(Tag<M> t)
    {
      return fs[M - 1];
    }

    template <int M>
      requires(M > 0)
    DEVICE_FORCEINLINE_FUNCTION const auto &operator()(Tag<M> t) const
    {
      return fs[M - 1];
    }

    template <typename R> void operator=(R &&r)
    {
      fs[0].onBeforeAssignment(r.SU2Get(1_c));
      fs[1].onBeforeAssignment(r.SU2Get(2_c));
      fs[2].onBeforeAssignment(r.SU2Get(3_c));

      PreGet::apply(r);

      const auto view1 = fs[0].getView();
      const auto view2 = fs[1].getView();
      const auto view3 = fs[2].getView();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx)
      {
        device::apply(
            [&](const auto &...args) {
              auto result = DoEval::eval(r, args...);
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
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      device::array<T, 4> result;
      result[1] = fs[0].get(idx...);
      result[2] = fs[1].get(idx...);
      result[3] = fs[2].get(idx...);
      result[0] = sqrt(T(1) - pow<2>(result[1]) - pow<2>(result[2]) - pow<2>(result[3]));
      return result;
    }

    using Getter = SU2Getter;

    static constexpr size_t SHIFTIND = 0;
    static constexpr size_t size = 4;
    static constexpr size_t numberToSkipAsTuple = 1;

  protected:
    device::array<Field<NDim, T>, 3> fs;
    const device::memory::host_string mName;
    LayoutStruct<NDim> mLayout;
  };

  template <size_t NDim, typename T> using SU2Field = SU2FieldBase<NDim, T>;
  // TODO: What is the point of this aliasing?

#ifdef TEMPLATTEST
template<size_t _NDim, typename T>
  struct SU2FieldBaseTester
  {
  public:
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
