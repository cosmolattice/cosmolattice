#ifndef TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWCONFIG_H
#define TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWCONFIG_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler  Year: 2025

#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"
#include "TempLat/lattice/field/abstractfield.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"

#include "TempLat/lattice/algebra/helpers/preget.h"
#include "TempLat/lattice/algebra/helpers/postget.h"

#include "TempLat/parallel/device.h"
#include "TempLat/parallel/device_memory.h"
#include "TempLat/parallel/device_iteration.h"

namespace TempLat
{
  MakeException(FieldViewConfigWrongSpaceConfirmation);
  MakeException(FieldViewConfigMissingToolBox);

  /** @brief A view on the field which, when interacted with, assures every time again that things are in
   *   configuration space, and possibly the ghost cells are updated when needed.
   *   The final Field class defaults to config space, which means it inherits from this class.
   *
   * Unit test: ctest -R test-fieldviewconfig
   **/
  template <size_t _NDim, typename T> class ConfigView : public AbstractField<_NDim, T>
  {
  public:
    // Put public methods here. These should change very little over time.
    static constexpr size_t NDim = _NDim;

    using AbstractField<NDim, T>::mManager;
    using AbstractField<NDim, T>::mToolBox;

    ConfigView(std::string name, device::memory::host_ptr<MemoryToolBox<NDim>> toolBox, LatticeParameters<T> pLatPar)
        : AbstractField<NDim, T>(name, toolBox, pLatPar), mDisableFFTBlocking(false)
    {
      if (toolBox != nullptr)
        mLayout = mToolBox->mLayouts.getConfigSpaceLayout();
      else
        throw FieldViewConfigMissingToolBox("A FieldViewConfig must be constructed with a valid MemoryToolBox.");

      mManager->setGhostsAreStale();
      mManager->confirmConfigSpace(); // allocation happens here

      const auto localSizes = mLayout.getLocalSizes();
      const size_t nGhosts = mLayout.getNGhosts();

      memorySizes = mLayout.getSizesInMemory();
      for (size_t d = 0; d < NDim; ++d) {
        memorySizes[d] += nGhosts + nGhosts; // add padding to the local sizes
        localSlicing[d] = std::make_pair(nGhosts, nGhosts + localSizes[d]);
      }

      mView = mManager->getNDView(memorySizes);
      mRawView = mManager->getRawView();
    }

    DEVICE_FORCEINLINE_FUNCTION
    auto getView() const { return mView; }

    template <typename R> void assign(R &&g)
    {
      onBeforeAssignment(g);

      PreGet::apply(g);

      auto functor = DEVICE_CLASS_LAMBDA(const device::IdxArray<NDim> &idx)
      {
        device::apply([&](auto &&...args) { mView(args...) = DoEval::eval(g, args...); }, idx);
      };
      device::iteration::foreach ("ConfigViewAssign", mLayout, functor);

      PostGet::apply(g);

      mManager->setGhostsAreStale();
    }

    inline auto getLocalNDHostView() const { return mManager->getNDHostSubView(memorySizes, localSlicing); }
    inline auto getFullNDHostView() const { return mManager->getNDHostView(memorySizes); }
    inline auto getRawHostView() const { return mManager->getRawHostView(); }

    template <typename R> void operator=(R &&g) { this->assign(std::forward<R>(g)); }

    template <typename R> void operator+=(R &&g) { this->operator=(*this + g); }

    void operator=(const ConfigView<NDim, T> &other) { this->assign(other); }

    template <typename... IDX>
      requires requires {
        requires(NDim == sizeof...(IDX));
        requires(std::is_integral_v<std::decay_t<IDX>> && ...);
      }
    DEVICE_FORCEINLINE_FUNCTION T &getSet(IDX &&...idx) const
    {
      return mView(idx...);
    }

    inline void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const
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

    const auto &getLayout() const { return mLayout; }

    void updateGhosts() const { this->mManager->updateGhosts(); }

    std::string toString() const { return mManager->getName() + "(x)"; }

    /** @brief Disable the blocking of going from configuration space to fourier space.
     *  Used by PowerSpectrumBuilder, which uses newly allocate memory which is filled in configuration space,
     *  and then FFT'ed to fourier space.
     */
    void setDisableFFTBlocking() { mDisableFFTBlocking = true; }

    template <typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      return mView(idx...);
    }

    template <typename R> void onBeforeAssignment(R &&g)
    {
      /* likewise, make sure we are in configuration space (here the FFT may be fired!). */
      mManager->confirmConfigSpace();

      ConfirmSpace::apply(g, mLayout, SpaceStateType::Configuration);

      GhostsHunter::apply(g);
      mManager->flagHostMirrorOutdated();
    }

    std::string to_string() const { return mManager->getName() + "(x)"; }

  private:
    LayoutStruct<NDim> mLayout;

    device::memory::NDViewUnmanaged<NDim, T> mView;
    device::memory::NDViewUnmanaged<1, T> mRawView;
    device::memory::NDViewUnmanagedHost<NDim, T> mHostView;

    device::IdxArray<NDim> memorySizes;
    device::array<std::pair<ptrdiff_t, ptrdiff_t>, NDim> localSlicing;

    bool mDisableFFTBlocking;
  };
} // namespace TempLat

#endif
