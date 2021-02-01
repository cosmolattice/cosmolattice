#ifndef TEMPLAT_UTIL_RANGEITERATION_TAG_H
#define TEMPLAT_UTIL_RANGEITERATION_TAG_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/util/tdd/tdd.h"


namespace TempLat {


    /** \brief A class which implements homemade compile time constants.
     *
     *
     * Unit test: make test-tag
     **/

     //template<int N>
     //using Tag = std::integral_constant<int, N>;

     template<int N>
     class Tag{
     public:

       constexpr operator int() const
       {
             return N;
       }
       static constexpr int value = N;

       std::string toString() const { return "Tag<" + std::to_string(N) + ">"; }


     };


     template<int M, int N>
     Tag<M+N> operator+(Tag<M> t1, Tag<N> t2)
     {
         return Tag<M+N>();
     }

    template<int M, int N>
    Tag<M-N> operator-(Tag<M> t1, Tag<N> t2)
    {
        return Tag<M-N>();
    }

    template<int M>
    Tag<-M> operator-(Tag<M> t1)
    {
        return Tag<-M>();
    }


    template<int M, int N>
    Tag<M*N> operator*(Tag<M> t1, Tag<N> t2)
    {
        return Tag<M*N>();
    }

    template<int M, int N>
    constexpr bool operator==(Tag<M> t1, Tag<N> t2)
    {
        return M==N;
    }
    template<int M, int N>
    constexpr bool operator!=(Tag<M> t1, Tag<N> t2)
    {
        return !std::is_same<Tag<M>,Tag<N>>::value;
    }

    template<int M, int N>
    constexpr
    typename std::enable_if< (M<N), bool>::type
    operator<(Tag<M> t1, Tag<N> t2)
    {
        return true;
    }
    template<int M, int N>
    constexpr
    typename std::enable_if< (M>=N), bool>::type
    operator<(Tag<M> t1, Tag<N> t2)
    {
        return false;
    }

    template<int M, int N>
    constexpr
    typename std::enable_if< (M<=N), bool>::type
    operator<=(Tag<M> t1, Tag<N> t2)
    {
        return true;
    }
    template<int M, int N>
    constexpr
    typename std::enable_if< (M>N), bool>::type
    operator<=(Tag<M> t1, Tag<N> t2)
    {
        return false;
    }

    template<int M, int N>
    constexpr
    typename std::enable_if< (M>N), bool>::type
    operator>(Tag<M> t1, Tag<N> t2)
    {
        return true;
    }
    template<int M, int N>
    constexpr
    typename std::enable_if< (M<=N), bool>::type
    operator>(Tag<M> t1, Tag<N> t2)
    {
        return false;
    }

    template<int M, int N>
    constexpr
    typename std::enable_if< (M>=N), const bool>::type
    operator>=(Tag<M> t1, Tag<N> t2)
    {
        return true;
    }
    template<int M, int N>
    constexpr
    typename std::enable_if< (M<N), const bool>::type
    operator>=(Tag<M> t1, Tag<N> t2)
    {
        return false;
    }

    //Unfortunately above expressions do not works with nested lambda (like ForLoop(ForLoop(If())) ). Work arund, use these macros.

#define IsLess(i ,j ) decltype(i)::value < decltype(j)::value
#define IsLessOrEqual(i ,j ) decltype(i)::value <= decltype(j)::value
#define IsMore(i ,j ) decltype(i)::value > decltype(j)::value
#define IsMoreOrEqual(i ,j ) decltype(i)::value >= decltype(j)::value
#define IsEqual(i,j) std::is_same<decltype(i),decltype(j)>::value







} /* TempLat */




#endif
