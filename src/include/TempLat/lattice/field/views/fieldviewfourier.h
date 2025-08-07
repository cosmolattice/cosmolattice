#ifndef TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWFOURIER_H
#define TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWFOURIER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/field/abstractfield.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/preget.h"
#include "TempLat/lattice/algebra/helpers/postget.h"

namespace TempLat
{
  MakeException(FieldViewFourierWrongSpaceConfirmation);

  /** \brief A view on the field which, when interacted with, assures every time again that things are in
   *   *fourier* space, and possibly the ghost cells are updated when needed.
   *
   *   No public constructor: only the friend class Field can instantiate this view on its own memory.
   *
   * Unit test: make test-fieldviewconfig
   **/

  /** \brief A simple class which provides a get method for basic types.
   * Field class
   *
   **/
  template <size_t NDim, typename T> class FourierView : public AbstractField<NDim, T>
  {
  public:
    using AbstractField<NDim, T>::mManager;
    using AbstractField<NDim, T>::mToolBox;

    template <typename R> void operator=(R &&g) { this->assign(std::forward<R>(g)); }

    template <typename R> void assign(R &&g)
    {
      onBeforeAssignment(g);
#ifndef NOKOKKOS
      PreGet::apply(g);
      if constexpr (NDim > 1) {
        auto functor = KOKKOS_CLASS_LAMBDA(const device::array<size_t, NDim> &idx)
        {
          device::apply([&](auto &&...args) { mView(args...) = GetEval::getEval(g, args...); }, idx);
        };
        Kokkos::parallel_for("ConfigViewAssign",                                                         //
                             Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(start_iteration, stop_iteration), //
                             KokkosNDLambdaWrapper<NDim, decltype(functor)>(functor));
      } else if constexpr (NDim == 1) {
        Kokkos::parallel_for(
            "ConfigViewAssign", //
            Kokkos::RangePolicy(start_iteration[0], stop_iteration[0]),
            KOKKOS_CLASS_LAMBDA(const size_t idx) { mView(idx) = GetEval::getEval(g, idx); });
      } else {
        static_assert(NDim > 0);
      }
      PostGet::apply(g);
#else
      throw Naaaaaa;
#endif
    }

    inline auto getLocalNDHostView() const
    {
      // As we have no ghosts in Fourier space, we can return the full view.
      return mManager->template getNDHostView<complex<T>>(memorySizes);
    }
    inline auto getFullNDHostView() const { return mManager->template getNDHostView<complex<T>>(memorySizes); }
    inline auto directView() const { return mManager->template getRawHostView<complex<T>>(); }

    template <typename R> void onBeforeAssignment(R &&g)
    {
      /* likewise, make sure we are in configuration space (here the FFT may be fired!). */
      mManager->confirmFourierSpace();

      ConfirmSpace::apply(g, mToolBox->mLayouts.getFourierSpaceLayout(), SpaceStateType::Fourier);
      GhostsHunter::apply(g);
      mManager->flagHostMirrorOutdated();
    }

    complex<T> get(ptrdiff_t i) const { return mManager->as_complex(i); }

    complex<T> &getSet(ptrdiff_t i) { return mManager->as_complex(i); }

    virtual const JumpsHolder<NDim> &getJumps() const { return mToolBox->mLayouts.getFourierSpaceJumps(); }

    inline void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType)
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

    std::string toString() const { return mManager->getName() + "(k)"; }

    const auto &getLayout() { return mToolBox->mLayouts.getFourierSpaceLayout(); }

    /** \brief Getting a single entry from an array. Variadic because the number of dimensions is variable.
     *  Use for tests only, never for actual integrations and iterations.
     *  The arguments are in LOCAL SPACE!! Not global. Use true array indices, but not accounting for ghosts.
     *  At the same time, the arguments are in original dimension order, not transposed.
     *  Transposition will be applied internally where applicable.
     */
    template <typename... Args> auto &operator()(bool &test, Args... args)
    {
      //            const ptrdiff_t sz = sizeof...(args);
      std::vector<ptrdiff_t> asVec{{args...}};
      return operator()(test, asVec);
    }

    auto &operator()(bool &test, std::vector<ptrdiff_t> position)
    {
      ptrdiff_t offset = this->getOffsetFromCoords(test, position);
      /* our hack to give something that is not in the memory, without throwing an exception. */
      static complex<T> dummy = 0;
      dummy = complex<T>(std::numeric_limits<T>::infinity(), 0);
      return offset > -1 ? mManager->as_complex(offset) : dummy;
    }

    // MPI aware setting of value. Use exceptionnaly (remove zero mode for example)

    template <typename... Args> void setZeroMode(const complex<T> &toSet)
    {
      // This is dimension-aware.
      std::vector<ptrdiff_t> asVec(mToolBox->mNDimensions, 0);
      set(toSet, asVec);
    }

    template <typename... Args> void set(const complex<T> &toSet, Args... args)
    {
      std::vector<ptrdiff_t> asVec{{args...}};
      set(toSet, asVec);
    }

    void set(const complex<T> &toSet, std::vector<ptrdiff_t> vec)
    {
      bool tmp;
      auto &res = (*this)(tmp, vec);
      if (tmp) res = toSet;
    }

    template <size_t _NDim, typename S> friend class Field;

  private:
    FourierView(const AbstractField<NDim, T> &f) : AbstractField<NDim, T>(f)
    {
      auto layout = mToolBox->mLayouts.getFourierSpaceLayout();
      auto localSizes = layout.getLocalSizes();
      // auto globalSizes = layout.getGlobalSizes();

      for (size_t d = 0; d < NDim; ++d) {
        start_iteration[d] = 0;
        stop_iteration[d] = start_iteration[d] + localSizes[d];
      }

      for (size_t d = 0; d < NDim; ++d)
        memorySizes[d] = layout.getLocalSizes()[d];
      mView = mManager->template getNDView<complex<T>>(memorySizes);
      mRawView = mManager->template getRawView<complex<T>>();
    }

    Kokkos::Array<int64_t, NDim> start_iteration;
    Kokkos::Array<int64_t, NDim> stop_iteration;

    KokkosNDViewUnmanaged<NDim, complex<T>> mView;
    KokkosNDViewUnmanaged<1, complex<T>> mRawView;
    KokkosNDViewUnmanaged<NDim, complex<T>, Kokkos::DefaultHostExecutionSpace> mHostView;

    std::array<ptrdiff_t, NDim> memorySizes;
    std::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> localSlicing;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
