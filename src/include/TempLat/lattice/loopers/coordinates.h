#ifndef TEMPLAT_LATTICE_ITERATORS_COORDINATES_H
#define TEMPLAT_LATTICE_ITERATORS_COORDINATES_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019. Jorge Baeza-Ballesteros, Year: 2023

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/loopers/coordinatesinitializer.h"

namespace TempLat {

        /** \brief A class which
         * holds offsets and vector coordinates. Used to cache the coordinate access and have loopers in arbitrary dimensions.
         *
         *
         * Unit test: make test-coordinates
         **/

      class Coordinates {
      public:
          /* Put public methods here. These should change very little over time. */
          Coordinates(const JumpsHolder& jumpsHolder, const LayoutStruct& layoutStruct):
          mLayouts(layoutStruct),
          mJumps(jumpsHolder),
          mIStart(0),
          jumps(mJumps.getJumpsInMemoryOrder()),
          vect(jumps.size())
          {
              CoordinatesInitializer mCI(jumpsHolder,layoutStruct);
              mCI.initCoords(1,mOffsets,mIStart,mIEnd);
              auto sizes = mJumps.getSizesInMemory();
              mOffsets.shrink_to_fit();

          }


          ptrdiff_t operator()(ptrdiff_t i) const
          {
            return mOffsets[i];
          }

          ptrdiff_t& operator()(ptrdiff_t i)
          {
            return mOffsets[i];
          }
        std::vector<ptrdiff_t> getVec(ptrdiff_t i)//(JBB, Sep 2023)
        {
            auto offset = mOffsets[i] - mJumps.toOrigin();;
            for(size_t j = 0; j < jumps.size(); j++) {
                mLayouts.putSpatialLocationFromMemoryIndexInto(offset / jumps[j], j, vect);
                offset = offset % jumps[j];
        }

          return vect;
        }
        ptrdiff_t getStart() const
        {
          return mIStart;
        }
        ptrdiff_t getEnd() const
        {
          return mIEnd;
        }
      private:
          /* Put all member variables and private methods here. These may change arbitrarily. */


          LayoutStruct mLayouts;
          JumpsHolder mJumps;

          std::vector<ptrdiff_t> mOffsets;
          ptrdiff_t mIStart;
          ptrdiff_t mIEnd;
          std::vector<ptrdiff_t> jumps, vect;


          friend class Looper;

      public:
#ifdef TEMPLATTEST
          static inline void Test(TDDAssertion& tdd);
#endif
      };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/loopers/coordinates_test.h"
#endif


#endif
