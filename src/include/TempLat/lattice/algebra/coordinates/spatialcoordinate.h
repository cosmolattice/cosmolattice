#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_SPATIALCOORDINATE_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_SPATIALCOORDINATE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/coordinates/coordinatevector.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"

namespace TempLat {


    /** \brief A class which implements spatial coordinates.
     *
     *
     * Unit test: make test-spatialcoordinate
     **/
    MakeException(SpatialCoordinateConfigWrongSpaceConfirmation);

     class SpatialCoordinate : public CoordinateVector {
     public:
         /* Put public methods here. These should change very little over time. */
         SpatialCoordinate( std::shared_ptr<MemoryToolBox> toolBox):
         mToolBox(toolBox) {

         }


         ptrdiff_t getVectorSize()
         {
           return mToolBox->mNDimensions;
         }

         auto vectorGet(ptrdiff_t i, ptrdiff_t j)
         {
           return mToolBox->getCoordConfiguration(i)[j];
         }
         auto operator[](const ptrdiff_t& i)
         {
           return getVectorComponent(*this, i);
         }
         JumpsHolder getJumps()
         {
           return mToolBox->mLayouts.getConfigSpaceJumps();
         }
         void confirmSpace(ptrdiff_t i, const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType)
         {
           switch (spaceType) {
               case SpaceStateInterface::SpaceType::Fourier:
                   throw SpatialCoordinateConfigWrongSpaceConfirmation("SpatialCoordinate explicitly only can be used in configuration space. Abort.");
                   break;
               case SpaceStateInterface::SpaceType::Configuration:
               default:
                   break;
           }
         }


     private:
         /* Put all member variables and private methods here. These may change arbitrarily. */
         std::shared_ptr<MemoryToolBox> mToolBox;

     public:
#ifdef TEMPLATTEST
         static inline void Test(TDDAssertion& tdd);
#endif
     };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate_test.h"
#endif


#endif
