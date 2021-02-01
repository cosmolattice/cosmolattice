#ifndef TEMPLAT_LATTICE_NPARTITEITERATORS_NPARTITECOORDINATES_H
#define TEMPLAT_LATTICE_NPARTITEITERATORS_NPARTITECOORDINATES_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/loopers/npartiteloopers/npartitecoordinatesinitializer.h"

namespace TempLat {

        /** \brief A class which
         * holds offsets and vector coordinates. Used to cache the coordinate access and have loopers in arbitrary dimensions.
         *
         *
         * Unit test: make test-coordinates
         **/

      class NPartiteCoordinates {
      public:
          /* Put public methods here. These should change very little over time. */
          NPartiteCoordinates(ptrdiff_t parity, const JumpsHolder& jumpsHolder, const LayoutStruct& layoutStruct) :
          mParity(parity)
          {

              NPartiteCoordinatesInitializer mCI(parity, jumpsHolder,layoutStruct);
              mCI.initCoords(1,mOffsets,mVectorCoords,mIStart,mIEnd);
          }

          ptrdiff_t operator()(size_t lat, ptrdiff_t i) const
          {
            return mOffsets[lat][i];
          }

          ptrdiff_t& operator()(size_t lat, ptrdiff_t i)
          {
            return mOffsets[lat][i];
          }
        std::vector<ptrdiff_t> getVec(size_t lat, ptrdiff_t i) const
        {
          return mVectorCoords[lat][i];
        }
        ptrdiff_t getStart() const
        {
          return mIStart;
        }
        std::vector<ptrdiff_t> getEnd() const
        {
          return mIEnd;
        }

        ptrdiff_t getParity() const
        {
              return mParity;
        }

      private:
          /* Put all member variables and private methods here. These may change arbitrarily. */
          std::vector<std::vector<ptrdiff_t>> mOffsets;
          std::vector<std::vector<std::vector<ptrdiff_t> > > mVectorCoords;
          ptrdiff_t mIStart;
          std::vector<ptrdiff_t> mIEnd;

          const ptrdiff_t mParity;

          friend class NPartiteLooper;

      public:
#ifdef TEMPLATTEST
          static inline void Test(TDDAssertion& tdd);
#endif
      };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/loopers/npartiteloopers/npartitecoordinates_test.h"
#endif


#endif
