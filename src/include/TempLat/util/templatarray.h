#ifndef TEMPLAT_UTIL_TempLatARRAY_H
#define TEMPLAT_UTIL_TempLatARRAY_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/util/rangeiteration/for_in_range.h"

namespace TempLat {


    /** \brief A class which makes array compatible which getComp algebra.
     *
     * 
     * Unit test: make test-templatarray
     **/

    template<typename T, int N, int shift = 0>
    class TempLatArray {
    public:
        /* Put public methods here. These should change very little over time. */
        TempLatArray(){}

        TempLatArray(const std::array<T,N>& vec) :
                mVec(vec)
        {}
        TempLatArray(std::array<T,N>&& vec) :
                mVec(vec)
        {}

        void operator=(const std::vector<T>& vec)
        {
            for(size_t i = 0; i< N; ++i) mVec[i] = vec[i];
        }

        template <typename S>
        void operator=(S&& vec)
        {
            ForLoop(i, 0, N-1, mVec[i] = GetValue::get(vec.getComp(i),0));
        }

        std::array<T,N>& asArr()
        {
            return mVec;
        }

      /*  operator std::array<T,N>()
        {
            return mVec;
        }*/

        template<int M>
        T getComp(Tag<M> t) const noexcept
        {
            return mVec[M];
        }

        T operator[](ptrdiff_t i) const
        {
            return mVec[i];
        }

        T operator()(ptrdiff_t i) const
        {
            return mVec[i-shift];
        }

        T& operator[](ptrdiff_t i)
        {
            return mVec[i];
        }

        T& operator()(ptrdiff_t i)
        {
            return mVec[i-shift];
        }

        //size_t size() const
        //{
        //    return mVec.size();
        //}

        using Getter = GetComponent;
        static constexpr size_t size = N;


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::array<T,N> mVec;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/templatarray_test.h"
#endif


#endif
