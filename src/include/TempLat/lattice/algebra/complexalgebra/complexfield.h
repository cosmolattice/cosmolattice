#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELD_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/device.h"
#include "TempLat/lattice/field/assignablefieldcollection.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexfieldget.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/complexalgebra/complexwrapper.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldfourierview.h"
#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"
#include <memory>

namespace TempLat
{
  /** @brief A class which implements complex-valued fields.
   *
   * Unit test: ctest -R test-complexfield
   **/
  template <size_t _NDim, typename T> class ComplexField
  {
  public:
    // Put public methods here. These should change very little over time.

    static constexpr size_t NDim = _NDim;

    ComplexField(Field<NDim, T> f1, Field<NDim, T> f2)
        : mR(f1), mI(f2), mName("complex(" + f1.getName() + ", " + f2.getName() + ")"),
          mToolBox(mR.getToolBox() == nullptr ? mI.getToolBox() : mR.getToolBox()),
          mLayout(mToolBox->mLayouts.getConfigSpaceLayout())
    {
    }

    ComplexField(std::string name, device::memory::host_ptr<MemoryToolBox<NDim>> toolBox,
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
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      device::array<T, 2> result;
      result[0] = mR.eval(idx...);
      result[1] = mI.eval(idx...);
      return result;
    }

    ComplexFieldFourierView<NDim, T> inFourierSpace() { return {mR.inFourierSpace(), mI.inFourierSpace()}; }

    template <typename R> void operator=(R &&g)
    {
      const auto &gR = ComplexFieldGetter::get(g, 0_c);
      const auto &gI = ComplexFieldGetter::get(g, 1_c);

      mR.onBeforeAssignment(gR);
      mI.onBeforeAssignment(gI);

      PreGet::apply(g);

      const auto viewR = mR.getView();
      const auto viewI = mI.getView();

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx)
      {
        device::apply(
            [&](auto &&...args) {
              auto result = DoEval::eval(g, args...);
              viewR(args...) = result[0];
              viewI(args...) = result[1];
            },
            idx);
      };
      device::iteration::foreach ("ComplexConfigViewAssign", mLayout, functor);

      PostGet::apply(g);

      mR.setGhostsAreStale();
      mI.setGhostsAreStale();
    }

    template <typename R> void operator+=(R &&r) { (*this) = (*this) + r; }

    std::string toString() const { return *mName; }

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

    device::memory::host_string mName;

    device::memory::host_ptr<MemoryToolBox<NDim>> mToolBox;

    LayoutStruct<NDim> mLayout;
  };

  template <size_t NDim, typename T> auto CField(Field<NDim, T> f1, Field<NDim, T> f2)
  {
    return ComplexField<NDim, T>(f1, f2);
  }
} // namespace TempLat

#endif
