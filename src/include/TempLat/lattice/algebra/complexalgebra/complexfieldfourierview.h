#ifndef TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_COMPLEXFIELDFOURIERVIEW_H
#define TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_COMPLEXFIELDFOURIERVIEW_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/lattice/algebra/complexalgebra/helpers/complexfieldget.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/views/fieldviewfourier.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/variadicindex.h"
#include <memory>

namespace TempLat
{

  /** \brief A class which holds complex field in fourier space.
   *
   *
   * Unit test: make test-complexfieldfourierview
   **/
  template <size_t NDim, typename T> class ComplexFieldFourierView
  {
  public:
    /* Put public methods here. These should change very little over time. */
    ComplexFieldFourierView(FourierView<NDim, T> r, FourierView<NDim, T> i) : mR(r), mI(i)
    {
      mToolBox = mR.getToolBox() == nullptr ? mI.getToolBox() : mR.getToolBox();

      auto layout = mToolBox->mLayouts.getFourierSpaceLayout();
      auto localSizes = layout.getLocalSizes();

      for (size_t d = 0; d < NDim; ++d) {
        start_iteration[d] = 0;
        stop_iteration[d] = start_iteration[d] + localSizes[d];
      }
    }

    std::string toString() const { return "(" + mR.toString() + ", " + mI.toString() + ")"; }

    KOKKOS_FORCEINLINE_FUNCTION
    auto &ComplexFieldGet(Tag<0> t) { return mR; }
    KOKKOS_FORCEINLINE_FUNCTION
    const auto &ComplexFieldGet(Tag<0> t) const { return mR; }

    KOKKOS_FORCEINLINE_FUNCTION
    auto &ComplexFieldGet(Tag<1> t) { return mI; }
    KOKKOS_FORCEINLINE_FUNCTION
    const auto &ComplexFieldGet(Tag<1> t) const { return mI; }

    template <typename... IDX>
      requires VariadicNDIndex<NDim, IDX...>
    KOKKOS_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<0> t, const IDX &...idx)
    {
      return mR.get(idx...);
    }

    template <typename... IDX>
      requires VariadicNDIndex<NDim, IDX...>
    KOKKOS_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<1> t, const IDX &...idx)
    {
      return mI.get(idx...);
    }

    template <typename R> void operator=(R &&g)
    {
      const auto &gR = g.ComplexFieldGet(0_c);
      const auto &gI = g.ComplexFieldGet(1_c);

      mR.onBeforeAssignment(gR);
      mI.onBeforeAssignment(gI);

      PreGet::apply(g);

      const auto viewR = mR.getView();
      const auto viewI = mI.getView();

      if constexpr (NDim > 1) {
        auto functor = KOKKOS_CLASS_LAMBDA(const device::array<size_t, NDim> &idx)
        {
          device::apply(
              [&](auto &&...args) {
                viewR(args...) = GetEval::getEval(gR, args...);
                viewI(args...) = GetEval::getEval(gI, args...);
              },
              idx);
        };
        Kokkos::parallel_for("ComplexFourierViewAssign",                                                 //
                             Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(start_iteration, stop_iteration), //
                             KokkosNDLambdaWrapper<NDim, decltype(functor)>(functor));
      } else if constexpr (NDim == 1) {
        Kokkos::parallel_for(
            "ComplexFourierViewAssign", //
            Kokkos::RangePolicy(start_iteration[0], stop_iteration[0]), KOKKOS_CLASS_LAMBDA(const size_t idx) {
              viewR(idx) = GetEval::getEval(gR, idx);
              viewI(idx) = GetEval::getEval(gI, idx);
            });
      } else {
        static_assert(NDim > 0);
      }

      PostGet::apply(g);
    }

    KOKKOS_FORCEINLINE_FUNCTION
    auto getDx() const { return mR.getDx(); }

    KOKKOS_FORCEINLINE_FUNCTION
    auto getKIR() const { return mR.getKIR(); }

    using Getter = ComplexFieldGetter;
    static constexpr size_t SHIFTIND = 0;
    static constexpr size_t size = 2;

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    FourierView<NDim, T> mR;
    FourierView<NDim, T> mI;

    std::shared_ptr<MemoryToolBox<NDim>> mToolBox;

    Kokkos::Array<int64_t, NDim> start_iteration;
    Kokkos::Array<int64_t, NDim> stop_iteration;
  };

  class ComplexFieldFourierViewTester
  {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
