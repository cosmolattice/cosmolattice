#ifndef TEMPLAT_LATTICE_ITERATORS_COORDINATES_H
#define TEMPLAT_LATTICE_ITERATORS_COORDINATES_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

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
          mIEnd2(1),
          jumps(mJumps.getJumpsInMemoryOrder()),
          vect(jumps.size())
          {
              CoordinatesInitializer mCI(jumpsHolder,layoutStruct);
              mCI.initCoords(1,mOffsets,mIStart,mIEnd);
              auto sizes = mJumps.getSizesInMemory();
              for(size_t i = 0; i < sizes.size(); i++) mIEnd2 *= sizes[i];
//                 exit(1);

          }


          ptrdiff_t operator()(ptrdiff_t i) const
          {
            return mOffsets[i];
          }

          ptrdiff_t& operator()(ptrdiff_t i)
          {
            return mOffsets[i];
          }
        std::vector<ptrdiff_t> getVec(ptrdiff_t i)
        {
            auto offset = mOffsets[i];
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

//           inline ptrdiff_t getOffset(ptrdiff_t i)
//           {
//             ptrdiff_t res = mJumps.toOrigin();
//             ptrdiff_t auxfact = mIEnd2, newi = i;
//             auto sizes = mJumps.getSizesInMemory();
//             auto jumps = mJumps.getJumpsInMemoryOrder();
//
//             for(size_t j = 0; j < sizes.size(); j++) {
//                 auxfact /= sizes[j];
//                 res += (newi / auxfact) * jumps[j];
//                 newi = newi % auxfact;
//             }
//             return res;
//           }

          LayoutStruct mLayouts;
          JumpsHolder mJumps;

          std::vector<ptrdiff_t> mOffsets;
          ptrdiff_t mIStart;
          ptrdiff_t mIEnd;
          ptrdiff_t mIEnd2;
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
