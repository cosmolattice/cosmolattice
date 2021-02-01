#ifndef TEMPLAT_LATTICE_NPARTITEITERATORS_NPARTITEITERATOR_H
#define TEMPLAT_LATTICE_NPARTITEITERATORS_NPARTITEITERATOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/loopers/npartiteloopers/npartitecoordinates.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"
#include <functional>

namespace TempLat {
        /** \brief A class which iterates over a sublattice.
         *
         *
         * Unit test: make test-looper
         **/

    class NPartiteLooper {
    public:
        /* Put public methods here. These should change very little over time. */

        NPartiteLooper(std::shared_ptr<NPartiteCoordinates> pCoord, ptrdiff_t pStart, std::vector<ptrdiff_t> pEnd):
        mCoord(pCoord),
        mStart(pStart),
        mEnd(pEnd)
        {

        }
        NPartiteLooper(std::shared_ptr<NPartiteCoordinates> pCoord):
               NPartiteLooper(pCoord, pCoord->getStart(), pCoord->getEnd())
        {

        }
        NPartiteLooper() :
        mStart(0)
        {

        }

        void init(std::shared_ptr<NPartiteCoordinates> pCoord)
        {
            mCoord = pCoord;
            mStart = pCoord->getStart();
            mEnd = pCoord->getEnd();
        }

        void operator++(){
          ++mICurrent;
        }

        inline bool end() const {

          return mICurrent<mEnd[mPCurrent];
        }
        inline void begin() const{
          mICurrent = mStart;
        }

        void parityBegin(){
            mPCurrent = 0;
        }
        bool parityEnd() const{
            return mPCurrent < mCoord->getParity();
        }

        void parityMove()
        {
            ++mPCurrent;
        }



        size_t next()
        {
          return mICurrent++;
        }

        size_t getEnd()
        {
          return mEnd[mPCurrent];
        }

        ptrdiff_t operator()() const
        {
          return mCoord->mOffsets[mPCurrent][mICurrent];
          //return (*mCoord)(mICurrent);
        }
        ptrdiff_t operator()(ptrdiff_t i) const
        {
          return mCoord->mOffsets[mPCurrent][mICurrent+i];
          //return (*mCoord)(mICurrent);
        }
        ptrdiff_t& operator()()
        {
          return mCoord->mOffsets[mPCurrent][mICurrent];
          //return (*mCoord)(mICurrent);
        }
        std::vector<ptrdiff_t>& getVec() const
        {
          return mCoord->mVectorCoords[mPCurrent][mCoord->mOffsets[mPCurrent][mICurrent]];
          //return mCoord->getVec((*this)());
        }
        std::vector<ptrdiff_t>& getVec(const ptrdiff_t& i) const
        {
          return mCoord->mVectorCoords[mPCurrent][i];
          //return mCoord->getVec(i);
        }

        ptrdiff_t getCurrentParity() const {
            return mPCurrent;
        }

    private:
        std::shared_ptr<NPartiteCoordinates> mCoord;
        mutable ptrdiff_t mICurrent;
        mutable ptrdiff_t mStart;
        mutable ptrdiff_t mPCurrent;
        mutable std::vector<ptrdiff_t> mEnd;

        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/loopers/npartiteloopers/npartitelooper_test.h"
#endif


#endif
