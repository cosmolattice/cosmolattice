#ifndef TEMPLAT_LATTICE_NPARTITEITERATORS_NPARTITECOORDINATESINITIALIZER_H
#define TEMPLAT_LATTICE_NPARTITEITERATORS_NPARTITECOORDINATESINITIALIZER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/jumpsholder.h"

namespace TempLat {

  /** \brief A class which create lattices according to their parity.
   * \todo Create a set of coordinates according to the layout, in arbitrary (integer...) dimensions
   * TODO: Would win in readability with a bit of refactoring. The original idea was to use that directly for iteration. Now we just
   * use it to initialise the coordinates which are saved to memory. Hence, no threading whatsoever appears here.
   *
   * Unit test: make test-coordinatesinitializer
   **/

  class NPartiteCoordinatesInitializer {
  public:
      /* Put public methods here. These should change very little over time. */
      NPartiteCoordinatesInitializer(ptrdiff_t parity, const JumpsHolder& jumpsHolder, const LayoutStruct& layoutStruct):
      mJumpsHolder(jumpsHolder),
      mLayoutStruct(layoutStruct),
      mNDimensions(mJumpsHolder.getSizesInMemory().size()),
      mParity(parity){

      }

      void initCoords(const ptrdiff_t& stride, std::vector<std::vector<ptrdiff_t>>& mOffsets, std::vector<std::vector<std::vector<ptrdiff_t> > >& mVectorCoords, ptrdiff_t& mIStart, std::vector<ptrdiff_t>& mIEnd) {
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
      std::vector<ptrdiff_t > countNum; //AF: dummy count because I am dumb.
      const ptrdiff_t mParity;


      /** \brief TODO: This function is a legacy function which should be refactored out. Nothing to do with threading.  */
      void setupRunInitPerThread(const ptrdiff_t& stride, bool &allCarryOn, const std::array<ptrdiff_t, 2u>& workShare,std::vector<std::vector<ptrdiff_t> >& mOffsets, std::vector<std::vector<std::vector<ptrdiff_t> > >& mVectorCoords, ptrdiff_t& mIStart, std::vector<ptrdiff_t>& mIEnd) {
          /* we are a friend class of IterationCoordinates: allowed to construct these things. */

          initCoordinates(workShare,mOffsets,mVectorCoords,mIStart,mIEnd);

          countNum=std::vector<ptrdiff_t >(mParity,0);
          std::vector<ptrdiff_t> vecCoord(mNDimensions);

          performIterationInit(mJumpsHolder.toOrigin(),  mOffsets,mVectorCoords,vecCoord,0,  stride, allCarryOn);


      }
      void initCoordinates(const std::array<ptrdiff_t, 2u>& workShare,std::vector<std::vector<ptrdiff_t>>& mOffsets, std::vector<std::vector<std::vector<ptrdiff_t>> >& mVectorCoords, ptrdiff_t& mIStart, std::vector<ptrdiff_t>& mIEnd)
    	{
                ptrdiff_t thisMemorySize = 1;
    	          for(const auto& tmp: mJumpsHolder.getSizesInMemory()) thisMemorySize*=tmp;
                mIStart = (thisMemorySize * workShare[0]) / workShare[1];

                ptrdiff_t mIEndTot = (thisMemorySize * (workShare[0] + 1)) / workShare[1];
                ptrdiff_t mIEndPerLat = mIEndTot / mParity;
                size_t remainder = mIEndTot - mIEndPerLat * mParity;
                mIEnd = std::vector<ptrdiff_t >(mParity, mIEndPerLat);
                for(size_t i = 0; i<remainder; ++i) mIEnd[i] += 1;

                mOffsets.clear();
                auto jumps = mJumpsHolder.getJumpsInMemoryOrder();
            auto sizes = mJumpsHolder.getSizesInMemory();
            ptrdiff_t maxOffsets = mJumpsHolder.toOrigin();

            for(size_t i = 0; i< sizes.size(); ++i){
                maxOffsets+=(sizes[i]-1) * jumps[i];
            }

                for(int j = 0; j< mParity; ++j) {
                    mOffsets.push_back({});
                    mVectorCoords.push_back(std::vector<std::vector<ptrdiff_t >>(maxOffsets + 1,std::vector<ptrdiff_t>(mNDimensions)));
                    for (ptrdiff_t i = mIStart; i < mIEnd[j]; ++i) {
                        mOffsets.back().push_back(0);
                    }
                }
    	}
      /** \brief Original recursive iteration, used to cache the coordinates to memory at initialisation

    */
      /** \brief Iterate one dimension, at each step call recursively the same function to iterate the next dimension. At the last dimension, call the innerBody lambda.

          Takes a reference as input, to avoid endless copy constructors at each recursion
          (happens for each step in each dimension). Not a const ref&,
          because we never know what the user gives as input.
      */
      inline bool performIterationInit(ptrdiff_t offset, std::vector<std::vector<ptrdiff_t> >& mOffsets, std::vector<std::vector<std::vector<ptrdiff_t> > >& mVectorCoords,  std::vector<ptrdiff_t>& vecCoord,ptrdiff_t thisDim,const ptrdiff_t& stride, bool &allCarryOn , const std::array<ptrdiff_t, 2u>& workShare = {{ 0, 1 }}) {
          bool carryOn = true;

          const ptrdiff_t thisMemorySize = mJumpsHolder.getSizesInMemory()[thisDim];

          const ptrdiff_t thisMemoryJump = mJumpsHolder.getJumpsInMemoryOrder()[thisDim];

          const ptrdiff_t iStart = (thisMemorySize * workShare[0]) / workShare[1];
          const ptrdiff_t iEnd = (thisMemorySize * (workShare[0] + 1)) / workShare[1];



          ptrdiff_t parityToCheck = 0;

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
                  parityToCheck = 0;
                  for(size_t index = 0; index < vecCoord.size() ; ++index) parityToCheck+=vecCoord[index];
                  parityToCheck = ( (parityToCheck % mParity) + mParity) % mParity;

                  mOffsets[parityToCheck][countNum[parityToCheck]] = offset + i * thisMemoryJump;
                  mVectorCoords[parityToCheck][mOffsets[parityToCheck][countNum[parityToCheck]]] = vecCoord; //Not a typo. Allow to access ijkl... coordinate by passing the offset to array.
                  countNum[parityToCheck]++;
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
#include "TempLat/lattice/loopers/npartiteloopers/npartitecoordinatesinitializer_test.h"
#endif


#endif
