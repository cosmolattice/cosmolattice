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
#include <Kokkos_Core.hpp>
#include <Kokkos_Macros.hpp>
#include <sys/types.h>

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
      updateKokkosView();
    }

    void updateKokkosView()
    {
#ifndef NOKOKKOS
      mView = mManager->getView();
#endif
    }

    template <typename G> class NDAssignment
    {
    public:
      NDAssignment(Kokkos::View<T *, Kokkos::DefaultExecutionSpace> _mView, const G &_g) : mView(_mView), g(_g) {}

      template <typename... IDX> KOKKOS_FUNCTION void operator()(IDX... idx) const
      {
        mView(idx...) = GetEval::getEval(g, idx...);
        // This is a no-op, but it is needed to make sure that the lambda is
        // instantiated correctly in the Kokkos parallel_for.
        // The actual assignment happens in the assign() method.
      }

    private:
      Kokkos::View<T *, Kokkos::DefaultExecutionSpace> mView;
      const G &g;
    };

    template <typename R> void assign(R &&g)
    {
      auto layout = mToolBox->mLayouts.getConfigSpaceLayout();
      auto localSizes = layout.getLocalSizes();
      auto globalSizes = layout.getGlobalSizes();
      auto localStarts = layout.getLocalStarts();

      for (uint i = 0; i < localSizes.size(); ++i)
        std::cout << "Local size[" << i << "] = " << localSizes[i] << std::endl;
      for (uint i = 0; i < globalSizes.size(); ++i)
        std::cout << "Global size[" << i << "] = " << globalSizes[i] << std::endl;
      for (uint i = 0; i < localStarts.size(); ++i)
        std::cout << "Local start[" << i << "] = " << localStarts[i] << std::endl;

      auto configSpaceJumps = mToolBox->mLayouts.getConfigSpaceJumps();
      auto padding = configSpaceJumps.getPadding();
      for (uint i = 0; i < padding.size(); ++i) {
        for (uint j = 0; j < padding[i].size(); ++j) {
          std::cout << "Padding[" << i << "][" << j << "] = " << padding[i][j] << " | ";
        }
        std::cout << std::endl;
      }

      onBeforeAssignment(g);
      updateKokkosView();

#ifndef NOKOKKOS
      Kokkos::Array<int64_t, NDim> start;
      Kokkos::Array<int64_t, NDim> stop;
      for (size_t d = 0; d < NDim; ++d) {
        start[d] = padding[d][0] + localStarts[d];
        stop[d] = start[d] + localSizes[d];
      }

      Kokkos::parallel_for("ConfigViewAssign",                                     //
                           Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(start, stop), //
                           NDAssignment<R>(mView, g)                               //
      );
#else
      auto it = mToolBox->itX();
      int i = 0;
      for (it.begin(); it.end(); ++(it)) {
        i = it();
        DoEval::eval(g, i);
        mManager->operator[](i) = GetEval::getEval(g, i);
      }
#endif

      mManager->setGhostsAreStale();
    }

    template <typename R> void operator=(R &&g) { this->assign(std::forward<R>(g)); }

    template <typename R> void operator+=(R &&g) { this->operator=(*this + g); }

    void operator=(const ConfigView<NDim, T> &other)
    { // overwrite the default = operator.
      this->assign(other);
    }

    KOKKOS_FORCEINLINE_FUNCTION
    T get(ptrdiff_t i) const
    {
#ifndef NOKOKKOS
      return mView(i);
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

    virtual Looper<NDim> &getIt() { return (Looper<NDim> &)mToolBox->itX(); }

    void updateGhosts() { this->mManager->updateGhosts(); }

    T &get(const Looper<NDim> &itX) { return mManager->operator[](itX()); }

    T get(const Looper<NDim> &itX) const { return mManager->operator[](itX()); }

    std::string toString() const { return mManager->getName() + "(x)"; }

    /** \brief Disable the blocking of going from configuration space to fourier space.
     *  Used by PowerSpectrumBuilder, which uses newly allocate memory which is filled in configuration space,
     *  and then FFT'ed to fourier space.
     */
    void setDisableFFTBlocking() { mDisableFFTBlocking = true; }

    template <typename... Args> auto &operator()(bool &test, Args... args)
    {
      //            const ptrdiff_t sz = sizeof...(args);
      std::vector<ptrdiff_t> asVec{{args...}};
      return operator()(test, asVec);
    }
    /** \brief Getting a single entry from an array. Variadic because the number of dimensions is variable.
     *  Use for tests only, never for actual integrations and iterations.
     *  The arguments are in LOCAL SPACE!! Not global. Use true array indices, but not accounting for ghosts.
     *  At the same time, the arguments are in original dimension order, not transposed.
     *  Transposition will be applied internally where applicable.
     */
    auto &operator()(bool &test, std::vector<ptrdiff_t> position)
    {
      ptrdiff_t offset = this->getOffsetFromCoords(test, position);
      /* our hack to give something that is not in the memory, without throwing an exception. */
      static T dummy = 0;
      dummy = std::numeric_limits<T>::infinity();
      return offset > -1 ? mManager->operator[](offset) : dummy;
    }

    template <typename R> void onBeforeAssignment(R &&g)
    {
      /* likewise, make sure we are in configuration space (here the FFT may be fired!). */
      mManager->confirmConfigSpace();

      ConfirmSpace::apply(g, mToolBox->mLayouts.getConfigSpaceLayout(),
                          SpaceStateInterface<NDim>::SpaceType::Configuration);

      GhostsHunter::apply(g);
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
    bool mDisableFFTBlocking;

#ifndef NOKOKKOS
    Kokkos::View<T *, Kokkos::DefaultExecutionSpace> mView;
#endif

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
