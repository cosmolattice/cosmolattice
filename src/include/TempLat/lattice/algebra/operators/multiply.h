#ifndef TEMPLAT_LATTICE_ALGEBRA_MULTIPLY_H
#define TEMPLAT_LATTICE_ALGEBRA_MULTIPLY_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <type_traits>
#include <complex>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/helpers/hasgetmethod.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"

#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"

namespace TempLat {

    /** \brief This one is not related to our algebra, but somehow not
     *  by default enabled in C++ standard library.
     */
    template <typename T, typename S>
    typename std::enable_if<std::is_same<T, decltype((T) std::declval<S>())>::value && ! HasGetMethod<S>::value, std::complex<T>>::type
    operator*(std::complex<T> a, S b) {
        return a * (T) b;
    }

    /** \brief This one is not related to our algebra, but somehow not
     *  by default enabled in C++ standard library.
     */
    template <typename T, typename S>
    typename std::enable_if<std::is_same<T, decltype((T) std::declval<S>())>::value && ! HasGetMethod<S>::value, std::complex<T>>::type
    operator*(S b, std::complex<T> a) {
        return a * (T) b;
    }


    /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
    namespace Operators {

        /** \brief A class which multiplies two getters. Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
         *
         * Unit test: make test-multiply
         **/
         template<typename R, typename T>
         class Multiplication : public BinaryOperator<R,T> {
         public:
           using BinaryOperator<R,T>::mR;
           using BinaryOperator<R,T>::mT;
           Multiplication(const R& pR, const T& pT): BinaryOperator<R,T>(pR,pT){}
           auto get(ptrdiff_t i)
           {
             return GetValue::get(mT,i)*GetValue::get(mR,i);
           }

           virtual std::string operatorString() const {
               return "*";
           }

           /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template <typename U>
            inline auto d(const U& other)  {
                return GetDeriv::get(mT, other) * mR + mT * GetDeriv::get(mR, other);
            }

         };

        template<typename R, int N>
        class MultiplicationN : public UnaryOperator<R> {
        public:
            using UnaryOperator<R>::mR;
            MultiplicationN(const R& pR): UnaryOperator<R>(pR){}
            auto get(ptrdiff_t i)
            {
                return N*GetValue::get(mR,i);
            }

            virtual std::string operatorString() const {
                return std::to_string(N)+"*";
            }

            /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template <typename U>
            inline auto d(const U& other)  {
                return N * mR;
            }

        };

    }
    /** \brief A mini struct for instiating the test case. */
    struct MultiplyTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    /** \brief Exposing our newly define multiplication operation to the world. */
    template<typename R, typename T>
    typename ConditionalBinaryGetter<Operators::Multiplication, R, T>::type
    operator*(const R& r, const T& t)
    {
      return Operators::Multiplication<R,T>(r,t);
    }

    template<typename R, int N>
    typename std::enable_if<HasGetMethod<R>::value && !(IsTempLatGettable<0,R>::value || IsSTDGettable<0,R>::value ),Operators::MultiplicationN<R,N>>::type
    operator*(const R& r, Tag<N> n)
    {
        return Operators::MultiplicationN<R,N>(r);
    }

    template<typename R, int N>
    typename std::enable_if<HasGetMethod<R>::value && !(IsTempLatGettable<0,R>::value || IsSTDGettable<0,R>::value ),Operators::MultiplicationN<R,N>>::type
    operator*(Tag<N> n, const R& r)
    {
        return Operators::MultiplicationN<R,N>(r);
    }


    /** \brief Specialize for possible zero input! */
      template <typename T>
      inline
      ZeroType operator*(const T& a, ZeroType b) {
          return b;
      }

      /** \brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
      template <typename T>
      inline
      typename std::enable_if<!std::is_same<T, ZeroType>::value, ZeroType>::type
      operator*(ZeroType a, const T& b) {
          return a;
      }

      /** \brief Specialize for possible zero input! Need to disable one of these for two OneTypes as input. */
      /** \brief Specialize for possible unit input! */
      template <typename T>
      inline
      typename std::enable_if<!std::is_same<T, OneType>::value && !std::is_same<T, ZeroType>::value, T>::type&
      operator*( T&& a, const OneType b) {
          return a;
      }

      /** \brief Specialize for possible unit input! */
      template <typename T>
      inline
      typename std::enable_if<!std::is_same<T, OneType>::value && !std::is_same<T, ZeroType>::value, T>::type&
      operator*(const OneType a,  T&& b) {
          return b;
      }

    /** \brief Specialize for possible unit input! */
    template <typename T>
    inline
    typename std::enable_if<!std::is_same<T, OneType>::value && !std::is_same<T, ZeroType>::value, T>::type
    operator*(const OneType& a,  const T& b) {
        return b;
    }


    /** \brief Specialize for possible unit input! */
    inline
    OneType
    operator*(OneType a, OneType b) {
        return a;
    }
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/multiply_test.h"
#endif


#endif
