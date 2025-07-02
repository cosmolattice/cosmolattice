#ifndef TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWCONFIG_H
#define TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWCONFIG_H

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

#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/parallel/kokkos/lambdawrapper.h"
#include <Kokkos_Core.hpp>
#include <impl/Kokkos_Profiling.hpp>

namespace TempLat
{
  MakeException(FieldViewConfigWrongSpaceConfirmation);

  /** \brief A view on the field which, when interacted with, assures every time again that things are in
   *   configuration space, and possibly the ghost cells are updated when needed.
   *   The final Field class defaults to config space, which means it inherits from this class.
   *
   * Unit test: make test-fieldviewconfig
   **/
  template <size_t NDim, typename T> class ConfigView : public AbstractField<NDim, T>
  {
  public:
    using AbstractField<NDim, T>::mManager;
    using AbstractField<NDim, T>::mToolBox;

    /* Put public methods here. These should change very little over time. */
    ConfigView(std::string name, std::shared_ptr<MemoryToolBox<NDim>> toolBox, LatticeParameters<T> pLatPar)
        : AbstractField<NDim, T>(name, toolBox, pLatPar), mDisableFFTBlocking(false)
    {
      mManager->setGhostsAreStale();
      mManager->confirmConfigSpace(); // allocation happens here

      auto layout = mToolBox->mLayouts.getConfigSpaceLayout();
      auto localSizes = layout.getLocalSizes();
      // auto globalSizes = layout.getGlobalSizes();
      auto localStarts = layout.getLocalStarts();

      auto configSpaceJumps = mToolBox->mLayouts.getConfigSpaceJumps();

      auto padding = configSpaceJumps.getPadding();

      for (size_t d = 0; d < NDim; ++d) {
        start_iteration[d] = padding[d][0] + localStarts[d];
        stop_iteration[d] = start_iteration[d] + localSizes[d];
      }

      memorySizes = layout.getLocalSizes();
      for (size_t d = 0; d < NDim; ++d) {
        memorySizes[d] += padding[d][0] + padding[d][1]; // add padding to the local sizes
        localSlicing[d] = std::make_pair(padding[d][0], padding[d][0] + localSizes[d]);
      }

      mView = mManager->getNDView(memorySizes);
      mRawView = mManager->getRawView();
    }

    template <typename R> void assign(R &&g)
    {
#ifndef NOKOKKOS
      onBeforeAssignment(g);
      if constexpr (NDim > 1) {
        auto functor = KOKKOS_CLASS_LAMBDA(const std::array<size_t, NDim> &idx)
        {
          std::apply([&](auto &&...args) { mView(args...) = GetEval::getEval(g, args...); }, idx);
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
#else
      throw Naaaaaa;
#endif

      mManager->setGhostsAreStale();
    }

    auto getLocalNDHostView() const { return mManager->getNDHostSubView(memorySizes, localSlicing); }
    auto getFullNDHostView() const { return mManager->getNDHostView(memorySizes); }

    template <typename R> void operator=(R &&g) { this->assign(std::forward<R>(g)); }

    template <typename R> void operator+=(R &&g) { this->operator=(*this + g); }

    void operator=(const ConfigView<NDim, T> &other)
    { // overwrite the default = operator.
      this->assign(other);
    }

    template <typename... IDX>
      requires(NDim == sizeof...(IDX))
    KOKKOS_FORCEINLINE_FUNCTION T get(IDX &&...idx) const
    {
#ifndef NOKOKKOS
      return mView(idx...);
#else
      return mManager->operator[](i);
#endif
    }

    T &getSet(ptrdiff_t i) { return mManager->operator[](i); }

    virtual const JumpsHolder<NDim> &getJumps() const { return mToolBox->mLayouts.getConfigSpaceJumps(); }

    inline void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateInterface<NDim>::SpaceType &spaceType)
    {
      switch (spaceType) {
      case SpaceStateInterface<NDim>::SpaceType::Fourier:
        if (!mDisableFFTBlocking)
          throw FieldViewConfigWrongSpaceConfirmation(
              "FieldViewConfig explicitly only can be used in configuration space. Do not transform to Fourier space "
              "in place. Or, if you know what you are doing and you are not doing multiple in-place FFT's on your "
              "integration data, you can call Field<T>::setDisableFFTBlocking() to disable this block, and enable "
              "going from configuration to Fourier space.");
        break;
      case SpaceStateInterface<NDim>::SpaceType::Configuration:
      default:
        AbstractField<NDim, T>::confirmSpace(newLayout, spaceType);
        break;
      }
    }

    const auto &getLayout() { return mToolBox->mLayouts.getConfigSpaceLayout(); }

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

      ConfirmSpace::apply(g, mToolBox->mLayouts.getConfigSpaceLayout(),
                          SpaceStateInterface<NDim>::SpaceType::Configuration);

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

  private:
    Kokkos::Array<int64_t, NDim> start_iteration;
    Kokkos::Array<int64_t, NDim> stop_iteration;

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

#ifdef TEMPLATTEST
#include "TempLat/lattice/field/views/fieldviewconfig_test.h"
#endif

#endif
