#ifndef TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWCONFIG_H
#define TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWCONFIG_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler  Year: 2025

#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/field/abstractfield.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"
#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/helpers/preget.h"
#include "TempLat/lattice/algebra/helpers/postget.h"

#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/parallel/kokkos/kokkoslambdawrapper.h"

namespace TempLat
{
  MakeException(FieldViewConfigWrongSpaceConfirmation);

  /** \brief A view on the field which, when interacted with, assures every time again that things are in
   *   configuration space, and possibly the ghost cells are updated when needed.
   *   The final Field class defaults to config space, which means it inherits from this class.
   *
   * Unit test: make test-fieldviewconfig
   **/
  template <size_t _NDim, typename T> class ConfigView : public AbstractField<_NDim, T>
  {
  public:
    static constexpr size_t NDim = _NDim;

    using AbstractField<NDim, T>::mManager;
    using AbstractField<NDim, T>::mToolBox;

    /* Put public methods here. These should change very little over time. */
    ConfigView(std::string name, std::shared_ptr<MemoryToolBox<NDim>> toolBox, LatticeParameters<T> pLatPar)
        : AbstractField<NDim, T>(name, toolBox, pLatPar), mDisableFFTBlocking(false)
    {
      if (toolBox != nullptr)
        mLayout = mToolBox->mLayouts.getConfigSpaceLayout();
      else
        return;

      mManager->setGhostsAreStale();
      mManager->confirmConfigSpace(); // allocation happens here

      const auto localSizes = mLayout.getLocalSizes();
      const size_t nGhosts = mLayout.getNGhosts();

      for (size_t d = 0; d < NDim; ++d) {
        memorySizes[d] = mLayout.getLocalSizes()[d];
        memorySizes[d] += nGhosts + nGhosts; // add padding to the local sizes
        localSlicing[d] = std::make_pair(nGhosts, nGhosts + localSizes[d]);
      }

      mView = mManager->getNDView(memorySizes);
      mRawView = mManager->getRawView();
    }

    KOKKOS_FORCEINLINE_FUNCTION
    auto getView() const { return mView; }

    template <typename R> void assign(R &&g)
    {
      onBeforeAssignment(g);

      PreGet::apply(g);

      auto functor = KOKKOS_CLASS_LAMBDA(const device::array<size_t, NDim> &idx)
      {
        device::apply([&](auto &&...args) { mView(args...) = GetEval::getEval(g, args...); }, idx);
      };
      Kokkos::parallel_for("ConfigViewAssign",            //
                           getLocalKokkosPolicy(mLayout), //
                           KokkosNDLambdaWrapper<NDim, decltype(functor)>(functor));

      PostGet::apply(g);

      mManager->setGhostsAreStale();
    }

    inline auto getLocalNDHostView() const { return mManager->getNDHostSubView(memorySizes, localSlicing); }
    inline auto getFullNDHostView() const { return mManager->getNDHostView(memorySizes); }
    inline auto directView() const { return mManager->getRawHostView(); }

    template <typename R> void operator=(R &&g) { this->assign(std::forward<R>(g)); }

    template <typename R> void operator+=(R &&g) { this->operator=(*this + g); }

    void operator=(const ConfigView<NDim, T> &other)
    { // overwrite the default = operator.
      this->assign(other);
    }

    template <typename... IDX>
      requires requires {
        requires(NDim == sizeof...(IDX));
        requires(std::is_integral_v<std::decay_t<IDX>> && ...);
      }
    KOKKOS_FORCEINLINE_FUNCTION T get(IDX &&...idx) const
    {
      return mView(idx...);
    }

    template <typename... IDX>
      requires requires {
        requires(NDim == sizeof...(IDX));
        requires(std::is_integral_v<std::decay_t<IDX>> && ...);
      }
    KOKKOS_FORCEINLINE_FUNCTION T &getSet(IDX &&...idx) const
    {
      return mView(idx...);
    }

    const JumpsHolder<NDim> &getJumps() const { return mToolBox->mLayouts.getConfigSpaceJumps(); }

    inline void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType)
    {
      switch (spaceType) {
      case SpaceStateType::Fourier:
        if (!mDisableFFTBlocking)
          throw FieldViewConfigWrongSpaceConfirmation(
              "FieldViewConfig explicitly only can be used in configuration space. Do not transform to Fourier space "
              "in place. Or, if you know what you are doing and you are not doing multiple in-place FFT's on your "
              "integration data, you can call Field<T>::setDisableFFTBlocking() to disable this block, and enable "
              "going from configuration to Fourier space.");
        break;
      case SpaceStateType::Configuration:
      default:
        AbstractField<NDim, T>::confirmSpace(newLayout, spaceType);
        break;
      }
    }

    const auto &getLayout() { return mLayout; }

    void updateGhosts() { this->mManager->updateGhosts(); }

    std::string toString() const { return mManager->getName() + "(x)"; }

    /** \brief Disable the blocking of going from configuration space to fourier space.
     *  Used by PowerSpectrumBuilder, which uses newly allocate memory which is filled in configuration space,
     *  and then FFT'ed to fourier space.
     */
    void setDisableFFTBlocking() { mDisableFFTBlocking = true; }

    /*
        template <typename... Args> auto &operator()(bool &test, Args... args)
        {
          //            const ptrdiff_t sz = sizeof...(args);
          std::vector<ptrdiff_t> asVec{{args...}};
          return operator()(test, asVec);
        }
         */
    /** \brief Getting a single entry from an array. Variadic because the number of dimensions is variable.
     *  Use for tests only, never for actual integrations and iterations.
     *  The arguments are in LOCAL SPACE!! Not global. Use true array indices, but not accounting for ghosts.
     *  At the same time, the arguments are in original dimension order, not transposed.
     *  Transposition will be applied internally where applicable.
     */
    /*auto &operator()(bool &test, std::vector<ptrdiff_t> position)
    {
      ptrdiff_t offset = this->getOffsetFromCoords(test, position);
      // our hack to give something that is not in the memory, without throwing an exception.
      static T dummy = 0;
      dummy = std::numeric_limits<T>::infinity();
      return offset > -1 ? mManager->operator[](offset) : dummy;
    }
*/
    template <typename R> void onBeforeAssignment(R &&g)
    {
      /* likewise, make sure we are in configuration space (here the FFT may be fired!). */
      mManager->confirmConfigSpace();

      ConfirmSpace::apply(g, mLayout, SpaceStateType::Configuration);

      GhostsHunter::apply(g);
      mManager->flagHostMirrorOutdated();
    }

    // MPI aware seetting of value. Use exceptionnaly (remove zero mode for example)
    template <typename... Args> void set(const T &toSet, Args... args)
    {
      std::vector<ptrdiff_t> asVec{{args...}};
      set(toSet, asVec);
    }

    void set(const T &toSet, std::vector<ptrdiff_t> vec)
    {
      bool tmp;
      auto &res = (*this)(tmp, vec);
      if (tmp) res = toSet;
    }

    std::string to_string() const { return mManager->getName() + "(x)"; }

  private:
    LayoutStruct<NDim> mLayout;

    KokkosNDViewUnmanaged<NDim, T> mView;
    KokkosNDViewUnmanaged<1, T> mRawView;
    KokkosNDViewUnmanaged<NDim, T, Kokkos::DefaultHostExecutionSpace> mHostView;

    std::array<ptrdiff_t, NDim> memorySizes;
    std::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> localSlicing;

    bool mDisableFFTBlocking;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
