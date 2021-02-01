#ifndef TEMPLAT_LATTICE_ITERATORS_COORDINATESINITIALIZER_H
#define TEMPLAT_LATTICE_ITERATORS_COORDINATESINITIALIZER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

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
      void initCoords(const ptrdiff_t& stride, std::vector<ptrdiff_t>& mOffsets, std::vector<std::vector<ptrdiff_t> >& mVectorCoords, ptrdiff_t& mIStart, ptrdiff_t& mIEnd) {
          bool allCarryOn = true;

          for ( int i = 0, iEnd = 1; i < iEnd; ++i ) {
              //See note below. This is legacy code which should be refactored, the threading is not done here.
              setupRunInitPerThread(stride, allCarryOn, {{ i, iEnd }},mOffsets,mVectorCoords,mIStart,mIEnd);
          }
      }

  private:
      /* Put all member variables and private methods here. These may change arbitrarily. */
      const JumpsHolder mJumpsHolder;
      const LayoutStruct mLayoutStruct;
      const ptrdiff_t mNDimensions;
      int countNum; //AF: dummy count because I am dumb.


      /** \brief TODO: This function is a legacy function which should be refactored out. Nothing to do with threading.  */
      void setupRunInitPerThread(const ptrdiff_t& stride, bool &allCarryOn, const std::array<ptrdiff_t, 2u>& workShare,std::vector<ptrdiff_t>& mOffsets, std::vector<std::vector<ptrdiff_t> >& mVectorCoords, ptrdiff_t& mIStart, ptrdiff_t& mIEnd) {
          /* we are a friend class of IterationCoordinates: allowed to construct these things. */
          initCoordinates(workShare,mOffsets,mVectorCoords,mIStart,mIEnd);
          countNum = 0;
          std::vector<ptrdiff_t> vecCoord(mNDimensions);
          performIterationInit(mJumpsHolder.toOrigin(),  mOffsets,mVectorCoords,vecCoord,0,  stride, allCarryOn);

      }
      void initCoordinates(const std::array<ptrdiff_t, 2u>& workShare,std::vector<ptrdiff_t>& mOffsets, std::vector<std::vector<ptrdiff_t> >& mVectorCoords, ptrdiff_t& mIStart, ptrdiff_t& mIEnd)
    	{
                ptrdiff_t thisMemorySize = 1;
    	          for(const auto& tmp: mJumpsHolder.getSizesInMemory()) thisMemorySize*=tmp;
                mIStart = (thisMemorySize * workShare[0]) / workShare[1];
                mIEnd = (thisMemorySize * (workShare[0] + 1)) / workShare[1];
                mOffsets.clear();

                auto jumps = mJumpsHolder.getJumpsInMemoryOrder();
                auto sizes = mJumpsHolder.getSizesInMemory();
                ptrdiff_t maxOffsets = mJumpsHolder.toOrigin();

            for(size_t i = 0; i< sizes.size(); ++i){
                    maxOffsets+=(sizes[i]-1) * jumps[i];
                }

            //mVectorCoords=std::vector<std::vector<ptrdiff_t> >(2*mIEnd,std::vector<ptrdiff_t>(mNDimensions)); //TODO: Too large, think about what is the size of the largest offset
            mVectorCoords=std::vector<std::vector<ptrdiff_t> >(maxOffsets + 1,std::vector<ptrdiff_t>(mNDimensions));


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
      inline bool performIterationInit(ptrdiff_t offset, std::vector<ptrdiff_t>& mOffsets, std::vector<std::vector<ptrdiff_t> >& mVectorCoords,  std::vector<ptrdiff_t>& vecCoord,ptrdiff_t thisDim,const ptrdiff_t& stride, bool &allCarryOn , const std::array<ptrdiff_t, 2u>& workShare = {{ 0, 1 }}) {
          bool carryOn = true;

          const ptrdiff_t thisMemorySize = mJumpsHolder.getSizesInMemory()[thisDim];
          const ptrdiff_t thisMemoryJump = mJumpsHolder.getJumpsInMemoryOrder()[thisDim];



          const ptrdiff_t iStart = (thisMemorySize * workShare[0]) / workShare[1];
          const ptrdiff_t iEnd = (thisMemorySize * (workShare[0] + 1)) / workShare[1];

          if ( thisDim < mNDimensions - 1) {
              /* outer loop, pass on to next dimension */
              for ( ptrdiff_t i = iStart; i < iEnd && carryOn; ++i) {

                  mLayoutStruct.putSpatialLocationFromMemoryIndexInto(i, thisDim, vecCoord);

                  carryOn = carryOn && performIterationInit(offset + i * thisMemoryJump,mOffsets,mVectorCoords,vecCoord,thisDim + 1, stride, allCarryOn);
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
    //say << i << ", " << thisMemorySize << ", " << carryOn << "\n";
                  mLayoutStruct.putSpatialLocationFromMemoryIndexInto(i, thisDim, vecCoord);

                  mOffsets[countNum] = offset + i * thisMemoryJump;

//                  say << mOffsets[countNum];
  //                say <<  mVectorCoords.size();

                  mVectorCoords[mOffsets[countNum]] = vecCoord; //Not a typo. Allow to access ijkl... coordinate by passing the offset to array.

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
