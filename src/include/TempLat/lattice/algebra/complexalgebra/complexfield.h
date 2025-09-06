#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELD_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/device.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/assignablefieldcollection.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexfieldget.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/complexalgebra/complexwrapper.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldfourierview.h"
#include "TempLat/lattice/algebra/helpers/variadicindex.h"
#include <memory>

namespace TempLat
{
  /** \brief A class which implements complex-valued fields.
   *
   * Unit test: make test-complexfield
   **/
  template <size_t _NDim, typename T> class ComplexField
  {
  public:
    /* Put public methods here. These should change very little over time. */

    static constexpr size_t NDim = _NDim;

    ComplexField(Field<NDim, T> f1, Field<NDim, T> f2)
        : mR(f1), mI(f2), mName("complex(" + f1.getName() + ", " + f2.getName() + ")"),
          mToolBox(mR.getToolBox() == nullptr ? mI.getToolBox() : mR.getToolBox()),
          mLayout(mToolBox->mLayouts.getConfigSpaceLayout())
    {
    }

    ComplexField(std::string name, std::shared_ptr<MemoryToolBox<NDim>> toolBox,
                 LatticeParameters<T> pLatPar = LatticeParameters<T>())
        : mR("Re_" + name, toolBox, pLatPar), mI("Im_" + name, toolBox, pLatPar), mName(name), mToolBox(toolBox),
          mLayout(mToolBox->mLayouts.getConfigSpaceLayout())
    {
    }

    DEVICE_FORCEINLINE_FUNCTION
    auto &ComplexFieldGet(Tag<0> t) { return mR; }
    DEVICE_FORCEINLINE_FUNCTION
    const auto &ComplexFieldGet(Tag<0> t) const { return mR; }
    DEVICE_FORCEINLINE_FUNCTION
    auto &operator()(Tag<0> t) { return mR; }
    DEVICE_FORCEINLINE_FUNCTION
    const auto &operator()(Tag<0> t) const { return mR; }

    DEVICE_FORCEINLINE_FUNCTION
    auto &ComplexFieldGet(Tag<1> t) { return mI; }
    DEVICE_FORCEINLINE_FUNCTION
    const auto &ComplexFieldGet(Tag<1> t) const { return mI; }
    DEVICE_FORCEINLINE_FUNCTION
    auto &operator()(Tag<1> t) { return mI; }
    DEVICE_FORCEINLINE_FUNCTION
    const auto &operator()(Tag<1> t) const { return mI; }

    template <int N> auto &operator()(Tag<N> t) { return ComplexFieldGet(t); }
    template <int N> const auto &operator()(Tag<N> t) const { return ComplexFieldGet(t); }

    template <typename... IDX>
      requires VariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<0> t, const IDX &...idx) const
    {
      return mR.get(idx...);
    }

    template <typename... IDX>
      requires VariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<1> t, const IDX &...idx) const
    {
      return mI.get(idx...);
    }

    template <typename... IDX>
      requires VariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(const IDX &...idx) const
    {
      return Kokkos::Array<T, 2>{mR.get(idx...), mI.get(idx...)};
    }

    ComplexFieldFourierView<NDim, T> inFourierSpace() { return {mR.inFourierSpace(), mI.inFourierSpace()}; }

    template <typename R> void operator=(R &&g)
    {
      const auto &gR = ComplexFieldGetter::get(g, 0_c);
      const auto &gI = ComplexFieldGetter::get(g, 1_c);

      mR.onBeforeAssignment(gR);
      mI.onBeforeAssignment(gI);

      PreGet::apply(gR);
      PreGet::apply(gI);

      const auto viewR = mR.getView();
      const auto viewI = mI.getView();

      auto functor = DEVICE_CLASS_LAMBDA(const device::array<size_t, NDim> &idx)
      {
        device::apply(
            [&](auto &&...args) {
              viewR(args...) = GetEval::getEval(gR, args...);
              viewI(args...) = GetEval::getEval(gI, args...);
            },
            idx);
      };
      Kokkos::parallel_for("ComplexConfigViewAssign", //
                           device::getLocalKokkosPolicy(mLayout),
                           KokkosNDLambdaWrapper<NDim, decltype(functor)>(functor));

      PostGet::apply(gR);
      PostGet::apply(gI);

      mR.setGhostsAreStale();
      mI.setGhostsAreStale();
    }

    template <typename R> void operator+=(R &&r) { (*this) = (*this) + r; }

    std::string toString() const { return "(" + mR.toString() + ", " + mI.toString() + ")"; }

    DEVICE_FORCEINLINE_FUNCTION
    auto getDx() const { return mR.getDx(); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getKIR() const { return mR.getKIR(); }

    void updateGhosts()
    {
      mR.updateGhosts();
      mI.updateGhosts();
    }

    void setGhostsAreStale()
    {
      mR.setGhostsAreStale();
      mI.setGhostsAreStale();
    }

    using Getter = ComplexFieldGetter;
    static constexpr size_t SHIFTIND = 0;
    static constexpr size_t size = 2;

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    Field<NDim, T> mR;
    Field<NDim, T> mI;

    const std::string mName;

    Kokkos::Array<int64_t, NDim> start_iteration;
    Kokkos::Array<int64_t, NDim> stop_iteration;

    std::shared_ptr<MemoryToolBox<NDim>> mToolBox;

    LayoutStruct<NDim> mLayout;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <size_t NDim, typename T> auto CField(Field<NDim, T> f1, Field<NDim, T> f2)
  {
    return ComplexField<NDim, T>(f1, f2);
  }

} // namespace TempLat

#endif
