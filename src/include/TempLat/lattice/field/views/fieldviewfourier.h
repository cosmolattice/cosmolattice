#ifndef TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWFOURIER_H
#define TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWFOURIER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2026

#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/field/abstractfield.h"
#include "TempLat/lattice/algebra/helpers/preget.h"
#include "TempLat/lattice/algebra/helpers/postget.h"
#include "TempLat/util/rangeiteration/tagliteral.h"

#include "TempLat/parallel/device.h"
#include "TempLat/parallel/device_memory.h"
#include "TempLat/parallel/device_iteration.h"

namespace TempLat
{
  MakeException(FieldViewFourierWrongSpaceConfirmation);

  /** @brief A view on the field which, when interacted with, assures every time again that things are in
   *   *fourier* space, and possibly the ghost cells are updated when needed.
   *
   *   No public constructor: only the friend class Field can instantiate this view on its own memory.
   *
   * Unit test: ctest -R test-fieldviewconfig
   **/
  template <size_t _NDim, typename T> class FourierView : public AbstractField<_NDim, T>
  {
  public:
    static constexpr size_t NDim = _NDim;

    using AbstractField<NDim, T>::mManager;
    using AbstractField<NDim, T>::mToolBox;

    template <typename R> void operator=(R &&g) { this->assign(std::forward<R>(g)); }

    template <typename R> void assign(R &&g)
    {
      const auto layout = mToolBox->mLayouts.getFourierSpaceLayout();
      onBeforeAssignment(g);

      PreGet::apply(g);

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx)
      {
        device::apply([&](auto &&...args) { mView(args...) = DoEval::eval(g, args...); }, idx);
      };
      device::iteration::foreach ("FourierViewAssign", layout, functor);

      PostGet::apply(g);
    }

    template <typename R>
      requires requires(R r) {
        r.ComplexFieldGet(0_c);
        r.ComplexFieldGet(1_c);
      }
    void assign(R &&g)
    {
      const auto layout = mToolBox->mLayouts.getFourierSpaceLayout();
      onBeforeAssignment(g);

      PreGet::apply(g);

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx)
      {
        device::apply(
            [&](auto &&...args) {
              const auto result = DoEval::eval(g, args...);
              mView(args...) = complex<T>(result[0], result[1]);
            },
            idx);
      };
      device::iteration::foreach ("FourierViewAssign", layout, functor);

      PostGet::apply(g);
    }

    inline auto getLocalNDHostView() const
    {
      // As we have no ghosts in Fourier space, we can return the full view.
      return mManager->template getNDHostView<complex<T>>(memorySizes);
    }
    inline auto getFullNDHostView() const { return mManager->template getNDHostView<complex<T>>(memorySizes); }
    inline auto getRawHostView() const { return mManager->template getRawHostView<complex<T>>(); }

    template <typename R> void onBeforeAssignment(R &&g)
    {
      /* likewise, make sure we are in configuration space (here the FFT may be fired!). */
      mManager->confirmFourierSpace();
      ConfirmSpace::apply(g, mToolBox->mLayouts.getFourierSpaceLayout(), SpaceStateType::Fourier);
      GhostsHunter::apply(g);
      mManager->flagHostMirrorOutdated();
    }

    template <typename... IDX>
      requires requires {
        requires(NDim == sizeof...(IDX));
        requires(std::is_integral_v<std::decay_t<IDX>> && ...);
      }
    DEVICE_FORCEINLINE_FUNCTION complex<T> &getSet(const IDX &...idx) const
    {
      return mView(idx...);
    }

    template <typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      return mView(idx...);
    }

    inline void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const
    {
      switch (spaceType) {
      case SpaceStateType::Configuration:
        throw FieldViewFourierWrongSpaceConfirmation("FieldViewFourier explicitly only can be used in Fourier space. "
                                                     "Do not transform to configuration space in place.");
        break;
      case SpaceStateType::Fourier:
      default:
        AbstractField<NDim, T>::confirmSpace(newLayout, spaceType);
        break;
      }
    }

    auto getView() const { return mView; }

    std::string toString() const { return mManager->getName() + "(k)"; }

    const auto &getLayout() const { return mToolBox->mLayouts.getFourierSpaceLayout(); }

    /** @brief Getting a single entry from an array. Variadic because the number of dimensions is variable.
     *  Use for tests only, never for actual integrations and iterations.
     *  The arguments are in LOCAL SPACE!! Not global. Use true array indices, but not accounting for ghosts.
     *  At the same time, the arguments are in original dimension order, not transposed.
     *  Transposition will be applied internally where applicable.
     */
    /*
   template <typename... Args> auto &operator()(bool &test, Args... args)
   {
     //            const ptrdiff_t sz = sizeof...(args);
     std::vector<ptrdiff_t> asVec{{args...}};
     return operator()(test, asVec);
   }*/

    /*
    auto &operator()(bool &test, std::vector<ptrdiff_t> position)
    {
      ptrdiff_t offset = this->getOffsetFromCoords(test, position);
      // our hack to give something that is not in the memory, without throwing an exception.
      constexpr complex<T> dummy(std::numeric_limits<T>::infinity(), 0);
      return offset > -1 ? mManager->as_complex(offset) : dummy;
    }
    */

    // MPI aware setting of value. Use exceptionnaly (remove zero mode for example)

    template <typename... Args> void setZeroMode(const complex<T> &toSet)
    {
      // This is dimension-aware.
      device::IdxArray<NDim> global_coord{{}};
      device::IdxArray<NDim> mem_pos{{}};

      const auto &layout = mToolBox->mLayouts.getFourierSpaceLayout();
      const bool owned = device::apply(
          [&](const auto &...idx) { return layout.putMemoryIndexFromSpatialLocationInto(mem_pos, idx...); },
          global_coord);

      // do this only if this process owns the zero mode!
      if (owned) device::memory::setAtOnePoint(*this, mem_pos, toSet);
    }

    std::string to_string() const { return mManager->getName() + "(k)"; }

    template <size_t __NDim, typename S> friend class Field;

  private:
    FourierView(const AbstractField<NDim, T> &f) : AbstractField<NDim, T>(f)
    {
      if (mToolBox == nullptr) return;
      auto layout = mToolBox->mLayouts.getFourierSpaceLayout();
      memorySizes = layout.getSizesInMemory();
      mView = mManager->template getNDView<complex<T>>(memorySizes);
      mRawView = mManager->template getRawView<complex<T>>();
    }

    device::memory::NDViewUnmanaged<NDim, complex<T>> mView;
    device::memory::NDViewUnmanaged<1, complex<T>> mRawView;
    device::memory::NDViewUnmanagedHost<NDim, complex<T>> mHostView;

    device::IdxArray<NDim> memorySizes;
    device::array<device::pair<ptrdiff_t, ptrdiff_t>, NDim> localSlicing;
  };
} // namespace TempLat

#endif
