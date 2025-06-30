#ifndef TEMPLAT_LATTICE_FIELD_ABSTRACTFIELD_H
#define TEMPLAT_LATTICE_FIELD_ABSTRACTFIELD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/latticeparameters.h"
#include "TempLat/lattice/memory/jumpsholder.h"
#include "TempLat/lattice/memory/memorylayoutstate.h"
#include "TempLat/lattice/memory/memorymanager.h"
#include "TempLat/lattice/memory/memorytoolbox.h"

#include "TempLat/lattice/algebra/helpers/getderiv.h"

namespace TempLat
{

  MakeException(FieldValueGetterException);

  enum CANONICALTYPE { AMPLITUDE, MOMENTUM };

  /** \brief A simple class which provides a get method for basic types.
   * Field class
   *
   *
   **/
  template <size_t NDim, typename T> class AbstractField
  {
  public:
    /* Put public methods here. These should change very little over time. */

    AbstractField(std::string name, std::shared_ptr<MemoryToolBox<NDim>> toolBox, LatticeParameters<T> pLatPar)
        : mToolBox(toolBox), mManager(std::make_shared<MemoryManager<NDim, T>>(mToolBox, name)), latPar(pLatPar)
    {
      // mManager->confirmConfigSpace(); //allocation happens here
      // mManager->allocate();
    }

    /** \brief Symbolic derivative: check this out.
     *  Assumes that different field are properly labelled,
     *  User responsible for that. Done automatically when one uses
     * fieldcollection.
     * If same type, then return 1, otherwise return 0.
     */

    virtual const JumpsHolder<NDim> &getJumps() const = 0;

    // virtual void onBeforeAssignment() const = 0;
    virtual inline void confirmSpace(const LayoutStruct<NDim> &newLayout,
                                     const SpaceStateInterface<NDim>::SpaceType &spaceType)
    {
      switch (spaceType) {
      case SpaceStateInterface<NDim>::SpaceType::Fourier:
        mManager->confirmFourierSpace();
        break;
      case SpaceStateInterface<NDim>::SpaceType::Configuration:
      default:
        mManager->confirmConfigSpace();
        break;
      }
    }

    std::shared_ptr<MemoryToolBox<NDim>> getToolBox() { return mToolBox; }

    ptrdiff_t confirmGhostsUpToDate() { return this->mManager->confirmGhostsUpToDate(); }

    virtual Looper<NDim> &getIt() = 0;

    // Mostly for testing purpose

    /** \brief Check the current state. */
    bool isConfigSpace() const { return mManager->isConfigSpace(); }
    /** \brief Check the current state. */
    bool isFourierSpace() const { return mManager->isFourierSpace(); }

    void setGhostsAreStale() { mManager->setGhostsAreStale(); }
    bool areGhostsStale() const { return mManager->areGhostsStale(); }

    std::shared_ptr<MemoryManager<NDim, T>> getMemoryManager() { return mManager; }

    KOKKOS_FORCEINLINE_FUNCTION
    auto getDx() const { return latPar.getDx(); }

    KOKKOS_FORCEINLINE_FUNCTION
    auto getKIR() const { return latPar.getKIR(); }

  protected:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::shared_ptr<MemoryToolBox<NDim>> mToolBox;
    std::shared_ptr<MemoryManager<NDim, T>> mManager;

    LatticeParameters<T> latPar; // Information about the lattice (dx, kir...)
                                 // Conceptually not amazing but really useful.

    ptrdiff_t getOffsetFromCoords(bool &test, std::vector<ptrdiff_t> position)
    {
      auto mJumps = getJumps();

      if (mJumps.size() != position.size())
        throw FieldValueGetterException(
            "Wrong size / number of arguments to Field<T>::operator(). Dimensionality of field:", mJumps.size(),
            ", dimensionality of your arguments:", position.size());

      ptrdiff_t offset = mJumps.getTotalOffsetFromSpatialCoordinates(position);

      /* our hack to give something that is not in the memory, without throwing an exception. */
      if (offset > -1)
        test = true;
      else
        test = false;
      return offset;
    }
  };

} // namespace TempLat

#endif
