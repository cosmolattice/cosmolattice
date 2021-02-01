#ifndef TEMPLAT_UTIL_TempLatVECTOR_H
#define TEMPLAT_UTIL_TempLatVECTOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include <initializer_list>
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/nakedtype.h"


namespace TempLat {


    /** \brief A class which makes vector compatible with GetComponent algebra and defines a operator(int i).
     *
     * 
     * 
     * Unit test: make test-templatvector
     **/


    template<typename T, int shift = 0>
    class TempLatVector {
    public:
        /* Put public methods here. These should change very little over time. */
        TempLatVector(){}

        TempLatVector(const std::vector<T>& vec) :
        mVec(vec)
        {}
        TempLatVector(std::vector<T>&& vec) :
        mVec(vec)
        {}

        template<typename... Args>
        TempLatVector(Args... args):
        mVec({args...})
        {

        };
        TempLatVector(std::initializer_list<T> l):
                mVec(l)
        {

        };
        TempLatVector(size_t n):
                mVec(n)
        {

        };
        TempLatVector(size_t n, T val):
                mVec(n, val)
        {

        };
        std::vector<T>& asVec()
        {
            return mVec;
        }

       /* operator std::vector<T>()
        {
            return mVec;
        }*/

        template<int N>
        T getComp(Tag<N> t) const noexcept
        {
            return mVec[N];
        }

        T operator[](ptrdiff_t i) const
        {
            return mVec[i];
        }

        T& operator[](ptrdiff_t i)
        {
            return mVec[i];
        }

        T operator()(ptrdiff_t i) const
        {
            return mVec[i - shift];
        }

        T& operator()(ptrdiff_t i)
        {
            return mVec[i - shift];
        }


        size_t size() const
        {
            return mVec.size();
        }

        void push_back(const T& val)
        {
            mVec.push_back(val);
        }

        void push_back(T&& val)
        {
            mVec.push_back(std::forward<T>(val) );
        }

        auto emplace_back(T&& val)
        {
            return mVec.emplace_back(std::forward<T>(val));
        }

        auto& back()
        {
            return mVec.back();
        }


        using Getter = GetComponent;


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::vector<T> mVec;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    template<typename... Args>
    auto make_templatvector(Args... args)
    {
        auto list = make_list(args...);
        return TempLatVector<typename NakedType<decltype(list.getComp(0_c))>::type>(args...);
    }




} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/templatvector_test.h"
#endif


#endif
