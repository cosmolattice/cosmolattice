#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2FIELD_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2FIELD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/assignablefieldcollection.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2get.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/operators/squareroot.h"

namespace TempLat
{
  /** \brief A class which implements a SU2 field (group).
   *
   *
   * Unit test: make test-su2field
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

    SU2FieldBase(std::string name, std::shared_ptr<MemoryToolBox<NDim>> toolBox,
                 LatticeParameters<T> pLatPar = LatticeParameters<T>())
        : fs{{
              Field<NDim, T>(name + "_1", toolBox, pLatPar), //
              Field<NDim, T>(name + "_2", toolBox, pLatPar), //
              Field<NDim, T>(name + "_3", toolBox, pLatPar)  //
          }},
          mName(name), mLayout(toolBox->mLayouts.getConfigSpaceLayout())
    {
    }

#ifdef __CUDA_ARCH__
    DEVICE_FUNCTION
    SU2FieldBase(const SU2FieldBase &other) : fs{{other.fs[0], other.fs[1], other.fs[2]}}, mLayout(other.mLayout) {}
#endif

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

    template <typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<0> t, const IDX &...idx) const
    {
      return sqrt(
          T(1) - pow<2>(fs[0].get(idx...)) - pow<2>(fs[1].get(idx...)) -
          pow<2>(fs[2].get(
              idx...))); // Apriori not optimal, as we compute several time c0, but does not seem to make a difference.
    }

    template <int M, typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<M> t, const IDX &...idx) const
    {
      return fs[M - 1].get(idx...);
    }

    template <typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION device::array<T, 4> SU2Get(const IDX &...idx) const
    {
      return {{SU2Get(0_c, idx...), SU2Get(1_c, idx...), SU2Get(2_c, idx...), SU2Get(3_c, idx...)}};
    }

    template <typename R> void operator=(R &&r)
    {
      ForLoop(i, 1, size - 1, fs[i - 1].onBeforeAssignment(std::remove_reference<R>::type::Getter::get(r, i)););
      ForLoop(j, 0, 2, PreGet::apply(fs[j]));

      const auto view1 = fs[0].getView();
      const auto view2 = fs[1].getView();
      const auto view3 = fs[2].getView();

      auto functor = DEVICE_CLASS_LAMBDA(const device::array<size_t, NDim> &idx)
      {
        // The problem here is that NVCC copies the given captures to constant memory. Clang moves them to registers,
        // which is what we need, as we need to use the cache.
#if defined(__NVCC__)
        std::decay_t<R> __r = r;
#else
        const auto &__r = r;
#endif
        device::apply(
            [&](auto &&...args) {
              DoEval::eval(r, args...);
              view1(args...) = __r.SU2Get(1_c, args...);
              view2(args...) = __r.SU2Get(2_c, args...);
              view3(args...) = __r.SU2Get(3_c, args...);
            },
            idx);
      };
      device::iteration::foreach ("SU2ConfigViewAssign", mLayout, functor);

      ForLoop(j, 0, 2, PostGet::apply(fs[j]));
      ForLoop(j, 0, 2, fs[j].setGhostsAreStale());
    }

    std::string toString() const { return mName; }

    DEVICE_FORCEINLINE_FUNCTION
    auto getDx() const { return GetDx::getDx(fs[0]); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getKIR() const { return GetKIR::getKIR(fs[0]); }

    using Getter = SU2Getter;

    static constexpr size_t SHIFTIND = 0;
    static constexpr size_t size = 4;
    static constexpr size_t numberToSkipAsTuple = 1;

  protected:
    device::array<Field<NDim, T>, 3> fs;
    const std::string mName;
    LayoutStruct<NDim> mLayout;

#ifdef TEMPLATTEST
  public:
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <size_t NDim, typename T> using SU2Field = SU2FieldBase<NDim, T>;
  // TODO: What is the point of this aliasing?
} // namespace TempLat

#endif
