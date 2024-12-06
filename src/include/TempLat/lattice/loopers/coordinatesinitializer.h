#ifndef TEMPLAT_LATTICE_ITERATORS_COORDINATESINITIALIZER_H
#define TEMPLAT_LATTICE_ITERATORS_COORDINATESINITIALIZER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019. Jorge Baeza-Ballesteros, Year: 2023 (big changes to eliminate coordinates initialization)

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/jumpsholder.h"

namespace TempLat {

  /** \brief A class which
   * \todo Create a set of coordinates according to the layout, in arbitrary (integer...) dimensions
   * TODO: Would win in readability with a bit of refactoring. The original idea was to use that directly for iteration. Now we just
   * use it to initialise the coordinates which are saved to memory. Hence, no threading whatsoever appears here.
   *
   * Unit test: make test-coordinatesinitializer
   **/

  class CoordinatesInitializer {
  public:
      /* Put public methods here. These should change very little over time. */
      CoordinatesInitializer(const JumpsHolder& jumpsHolder, const LayoutStruct& layoutStruct):
      mJumpsHolder(jumpsHolder),
      mLayoutStruct(layoutStruct),
      mNDimensions(mJumpsHolder.getSizesInMemory().size()) {

      }
      void initCoords(const ptrdiff_t& stride, std::vector<ptrdiff_t>& mOffsets, ptrdiff_t& mIStart, ptrdiff_t& mIEnd) {
          bool allCarryOn = true;

          for ( int i = 0, iEnd = 1; i < iEnd; ++i ) {
              //See note below. This is legacy code which should be refactored, the threading is not done here.
              setupRunInitPerThread(stride, allCarryOn, {{ i, iEnd }}, mOffsets, mIStart, mIEnd);
          }
      }

  private:
      /* Put all member variables and private methods here. These may change arbitrarily. */
      const JumpsHolder mJumpsHolder;
      const LayoutStruct mLayoutStruct;
      const ptrdiff_t mNDimensions;
      int countNum; //AF: dummy count because I am dumb.


      /** \brief TODO: This function is a legacy function which should be refactored out. Nothing to do with threading.  */
      void setupRunInitPerThread(const ptrdiff_t& stride, bool &allCarryOn, const std::array<ptrdiff_t, 2u>& workShare,std::vector<ptrdiff_t>& mOffsets, ptrdiff_t& mIStart, ptrdiff_t& mIEnd) {
          /* we are a friend class of IterationCoordinates: allowed to construct these things. */
          initCoordinates(workShare,mOffsets,mIStart,mIEnd);
          countNum = 0;
          performIterationInit(mJumpsHolder.toOrigin(),  mOffsets, 0,  stride, allCarryOn);

      }
    void initCoordinates(const std::array<ptrdiff_t, 2u>& workShare,std::vector<ptrdiff_t>& mOffsets, ptrdiff_t& mIStart, ptrdiff_t& mIEnd)
    {
            ptrdiff_t thisMemorySize = 1;
            for(const auto& tmp: mJumpsHolder.getSizesInMemory()) thisMemorySize*=tmp;
            mIStart = (thisMemorySize * workShare[0]) / workShare[1];
            mIEnd = (thisMemorySize * (workShare[0] + 1)) / workShare[1];
            mOffsets.clear();

            auto jumps = mJumpsHolder.getJumpsInMemoryOrder();
            auto sizes = mJumpsHolder.getSizesInMemory();

            for ( ptrdiff_t i = mIStart; i < mIEnd; ++i) {
                mOffsets.push_back(0);
            }
    }
      /** \brief Original recursive iteration, used to cache the coordinates to memory at initialisation

    */
      /** \brief Iterate one dimension, at each step call recursively the same function to iterate the next dimension. At the last dimension, call the innerBody lambda.

          Takes a reference as input, to avoid endless copy constructors at each recursion
          (happens for each step in each dimension). Not a const ref&,
          because we never know what the user gives as input.
      */
      inline bool performIterationInit(ptrdiff_t offset, std::vector<ptrdiff_t>& mOffsets, ptrdiff_t thisDim,const ptrdiff_t& stride, bool &allCarryOn , const std::array<ptrdiff_t, 2u>& workShare = {{ 0, 1 }}) {
          bool carryOn = true;

          const ptrdiff_t thisMemorySize = mJumpsHolder.getSizesInMemory()[thisDim];
          const ptrdiff_t thisMemoryJump = mJumpsHolder.getJumpsInMemoryOrder()[thisDim];

          const ptrdiff_t iStart = (thisMemorySize * workShare[0]) / workShare[1];
          const ptrdiff_t iEnd = (thisMemorySize * (workShare[0] + 1)) / workShare[1];

          if ( thisDim < mNDimensions - 1) {
              /* outer loop, pass on to next dimension */
              for ( ptrdiff_t i = iStart; i < iEnd && carryOn; ++i) {

                  carryOn = carryOn && performIterationInit(offset + i * thisMemoryJump,mOffsets,thisDim + 1, stride, allCarryOn);
                  /* only in dimension 0, otherwise too slow */
                  //bool expected = true;
                  //allCarryOn.compare_exchange_weak(expected, carryOn);
                  //
                  //carryOn = allCarryOn;

                  /* that was too slow. This is fast: */
                  if ( ! carryOn ) allCarryOn = false;
                  carryOn = allCarryOn;
              }
          } else {
              /* funny, get some speed increase by not checking for carry on here: effect -> finish entire rod before breaking the recursive loops. */
              for ( ptrdiff_t i = iStart; i < iEnd && carryOn; i += stride) {

                  mOffsets[countNum] = offset + i * thisMemoryJump;

                  countNum++;
                  if ( ! carryOn ) allCarryOn = false;
                  carryOn = allCarryOn;

              }
          }
          return carryOn;
      }

  public:
#ifdef TEMPLATTEST
      static inline void Test(TDDAssertion& tdd);
#endif
  };


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/loopers/coordinatesinitializer_test.h"
#endif


#endif
