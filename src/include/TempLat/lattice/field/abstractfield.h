#ifndef TEMPLAT_LATTICE_FIELD_ABSTRACTFIELD_H
#define TEMPLAT_LATTICE_FIELD_ABSTRACTFIELD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/memory/memorymanager.h"
#include "TempLat/lattice/memory/jumpsholder.h"
#include "TempLat/lattice/memory/memorylayoutstate.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/latticeparameters.h"

#include "TempLat/lattice/algebra/helpers/getderiv.h"

namespace TempLat {

  MakeException(FieldValueGetterException);

  enum CANONICALTYPE {AMPLITUDE, MOMENTUM};


  template<typename T>  //The three extra template parameters are just a conveninient way to create a different type for every field.
  class AbstractField {
    /** \brief A simple class which provides a get method for basic types.
     * Field class
     *
     *
     *
     **/
  public:
      /* Put public methods here. These should change very little over time. */

      AbstractField(std::string name, std::shared_ptr<MemoryToolBox> toolBox, LatticeParameters<T> pLatPar):
      mToolBox(toolBox),
      mManager(std::make_shared<MemoryManager<T>>(mToolBox, name)),
      latPar(pLatPar)
      {
          //mManager->confirmConfigSpace(); //allocation happens here
          //mManager->allocate();
      }


      /** \brief Symbolic derivative: check this out.
       *  Assumes that different field are properly labelled,
       *  User responsible for that. Done automatically when one uses
       * fieldcollection.
       * If same type, then return 1, otherwise return 0.
       */



      virtual const JumpsHolder& getJumps() const = 0;



      //virtual void onBeforeAssignment() const = 0;
      virtual inline void confirmSpace(const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType)
      {
        switch (spaceType) {
            case SpaceStateInterface::SpaceType::Fourier:
                mManager->confirmFourierSpace();
                break;
            case SpaceStateInterface::SpaceType::Configuration:
            default:
                mManager->confirmConfigSpace();
                break;
        }
      }

      std::shared_ptr<MemoryToolBox> getToolBox()
      {
        return mToolBox;
      }

      ptrdiff_t confirmGhostsUpToDate()
      {
        return this->mManager->confirmGhostsUpToDate();
      }


      virtual Looper& getIt() = 0 ;

      //Mostly for testing purpose

      /** \brief Check the current state. */
      bool isConfigSpace() const {
          return mManager->isConfigSpace();
      }
      /** \brief Check the current state. */
      bool isFourierSpace() const {
          return mManager->isFourierSpace();
      }

      void setGhostsAreStale()
      {
        mManager->setGhostsAreStale();
      }
      bool areGhostsStale() const {
          return mManager->areGhostsStale();
      }

      std::shared_ptr<MemoryManager<T>>getMemoryManager()
      {
        return mManager;
      }

      inline auto getDx() const
      {
        return latPar.getDx();
      }

      inline auto getKIR() const
      {
        return latPar.getKIR();
      }

  protected:
      /* Put all member variables and private methods here. These may change arbitrarily. */
    std::shared_ptr<MemoryToolBox> mToolBox;
    std::shared_ptr<MemoryManager<T>> mManager;

    LatticeParameters<T> latPar; // Information about the lattice (dx, kir...)
                              // Conceptually not amazing but really useful.

    ptrdiff_t getOffsetFromCoords(bool& test, std::vector<ptrdiff_t> position)
    {
          auto mJumps = getJumps();

          if ( mJumps.size() != position.size() ) throw FieldValueGetterException("Wrong size / number of arguments to Field<T>::operator(). Dimensionality of field:", mJumps.size(), ", dimensionality of your arguments:", position.size());

          ptrdiff_t offset = mJumps.getTotalOffsetFromSpatialCoordinates(position);

          /* our hack to give something that is not in the memory, without throwing an exception. */
          if(offset > -1) test = true;
          else test = false;
          return offset;
    }

  };

} /* TempLat */



#endif
