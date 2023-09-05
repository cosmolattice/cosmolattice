#ifndef TEMPLAT_LATTICE_ITERATORS_ITERATOR_H
#define TEMPLAT_LATTICE_ITERATORS_ITERATOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/loopers/coordinates.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"
#include <functional>

namespace TempLat {
        /** \brief A class to perform iteration.
         *
         *
         * Unit test: make test-looper
         **/

    class Looper {
    public:
        /* Put public methods here. These should change very little over time. */

        Looper(std::shared_ptr<Coordinates> pCoord, ptrdiff_t pStart, ptrdiff_t pEnd):
        mCoord(pCoord),
        mStart(pStart),
        mEnd(pEnd)
        {

        }
        Looper(std::shared_ptr<Coordinates> pCoord):
               Looper(pCoord, pCoord->getStart(), pCoord->getEnd())
        {
        }

        void operator++(){
          ++mICurrent;
        }

        inline bool end() const {

          return mICurrent<mEnd;
        }
        inline void begin() const{
          mICurrent = mStart;
        }

       bool iterate(const std::function<void(ptrdiff_t)>& func) const
       {
           for(begin(); end(); ++mICurrent)
           {
               func((*this)());
           }
           return true;
       }

       template<typename T>
       typename std::enable_if<IsTempLatGettable<0,T>::value || IsSTDGettable<0,T>::value,bool>::type  //This function allows to iterate over fieldcollection for example.
        iterate(T tup) const
        {
            for(begin(); end(); ++mICurrent)
            {
                for_each(tup,[this](auto& f){f((*this)());});
            }
            return true;
        }

        template <typename F>
        auto reduce(F func) const
        {
            using rType = decltype(func.get(0));
            rType ret = 0;

            for(begin(); end(); ++mICurrent)
            {
                ret += func.get((*this)());
            }
            return ret;
        }



        size_t next()
        {
          return mICurrent++;
        }

        size_t getEnd()
        {
          return mEnd;
        }

        ptrdiff_t operator()() const
        {
//           return mCoord->mOffsets[mICurrent];
          return (*mCoord)(mICurrent);
        }
        ptrdiff_t operator()(ptrdiff_t i) const
        {
//           return mCoord->mOffsets[mICurrent+i];
          return (*mCoord)(mICurrent);
        }
        ptrdiff_t operator()()
        {
//           return mCoord->mOffsets[mICurrent];
          return (*mCoord)(mICurrent);
        }
        std::vector<ptrdiff_t> getVec() const
        {
//           return mCoord->mVectorCoords[mCoord->mOffsets[mICurrent]];
          return mCoord->getVec(mICurrent);
        }
        std::vector<ptrdiff_t> getVec(const ptrdiff_t& i) const
        {
//           return mCoord->mVectorCoords[i];
          return mCoord->getVec(i);
        }

        bool isAtOrigin()
        {
            auto vec = getVec();
            bool res = true;
            for(auto x : vec) res = (res && (x == 0));
            return res;
        }

    private:
        std::shared_ptr<Coordinates> mCoord;
        mutable ptrdiff_t mICurrent;
        const ptrdiff_t mStart;
        const ptrdiff_t mEnd;

        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/loopers/looper_test.h"
#endif


#endif
