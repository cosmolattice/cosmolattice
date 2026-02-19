#ifndef TEMPLAT_LATTICE_FIELD_ABSTRACTFIELD_H
#define TEMPLAT_LATTICE_FIELD_ABSTRACTFIELD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/latticeparameters.h"
#include "TempLat/lattice/memory/memorylayoutstate.h"
#include "TempLat/lattice/memory/memorymanager.h"
#include "TempLat/lattice/memory/memorytoolbox.h"

#include "TempLat/lattice/algebra/helpers/getderiv.h"

namespace TempLat
{

  MakeException(FieldValueGetterException);

  enum CANONICALTYPE { AMPLITUDE, MOMENTUM };

  /** @brief A base class for a one-component field, providing common functionality and holding relevant pointers to
   * memory tools.
   *
   **/
  template <size_t _NDim, typename T> class AbstractField
  {
  public:
    // Put public methods here. These should change very little over time.

    static constexpr size_t NDim = _NDim;

    AbstractField(std::string name, device::memory::host_ptr<MemoryToolBox<NDim>> toolBox, LatticeParameters<T> pLatPar)
        : mToolBox(toolBox), mManager(mToolBox, name), latPar(pLatPar)
    {
    }

    inline void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const
    {
      switch (spaceType) {
      case SpaceStateType::Fourier:
        mManager->confirmFourierSpace();
        break;
      case SpaceStateType::Configuration:
      default:
        mManager->confirmConfigSpace();
        break;
      }
    }

    device::memory::host_ptr<MemoryToolBox<NDim>> getToolBox() const { return mToolBox; }

    ptrdiff_t confirmGhostsUpToDate() const { return this->mManager->confirmGhostsUpToDate(); }

    // Mostly for testing purpose

    /** @brief Check the current state.
     *
     * @return true if in configuration space
     */
    bool isConfigSpace() const { return mManager->isConfigSpace(); }

    /** @brief Check the current state.
     *
     * @return true if in fourier space
     */
    bool isFourierSpace() const { return mManager->isFourierSpace(); }

    void setGhostsAreStale() const { mManager->setGhostsAreStale(); }
    bool areGhostsStale() const { return mManager->areGhostsStale(); }

    device::memory::host_ptr<MemoryManager<NDim, T>> getMemoryManager() const { return mManager; }

    DEVICE_FORCEINLINE_FUNCTION
    auto getDx() const { return latPar.getDx(); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getKIR() const { return latPar.getKIR(); }

  protected:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    device::memory::host_ptr<MemoryToolBox<NDim>> mToolBox;
    device::memory::host_ptr<MemoryManager<NDim, T>> mManager;

    const LatticeParameters<T> latPar; // Information about the lattice (dx, kir...)
                                       // Conceptually not amazing but really useful.
  };
} // namespace TempLat

#endif
