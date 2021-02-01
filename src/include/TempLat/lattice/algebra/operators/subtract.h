#ifndef TEMPLAT_LATTICE_ALGEBRA_SUBTRACT_H
#define TEMPLAT_LATTICE_ALGEBRA_SUBTRACT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/operators/unaryminus.h"
#include "TempLat/lattice/algebra/constants/halftype.h"


namespace TempLat {

    /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
    namespace Operators {
        /** \brief A class which subtracts two getters. Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
         *
         * Unit test: make test-multiply
         **/

        template<typename R, typename T>
        class Substraction : public BinaryOperator<R,T> {
        public:
          using BinaryOperator<R,T>::mR;
          using BinaryOperator<R,T>::mT;
          Substraction(const R& pR, const T& pT): BinaryOperator<R,T>(pR,pT){}
          auto get(ptrdiff_t i)
          {
            return GetValue::get(mR,i)-GetValue::get(mT,i);
          }
          virtual std::string operatorString() const {
              return "-";
          }
          /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
          template <typename U>
          inline auto d(const U& other)  {
              return GetDeriv::get(mR, other) - GetDeriv::get(mT, other);
          }
        };


    }

    template<typename R, typename T>
    inline
    typename ConditionalBinaryGetter<Operators::Substraction, R, T>::type
    operator-(const R& r, const T& t)
    {
      return Operators::Substraction<R,T>(r,t);
    }


    /** \brief Specialize for possible zero input! */
        template <typename T>
        inline
        T& operator-( T&& a,  ZeroType b) {
            return a;
        }

        /** \brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
    //    template <typename T, typename S>
    //    inline
    //    typename std::enable_if<HasGetMethod<T>::value && std::is_same<T, S>::value, ZeroType>::type
    //    operator-( T&& a, T&& b) {
    //        return ZeroType();
    //    }

        /** \brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
        template <typename T>
        inline
        typename std::enable_if<!std::is_same<T, ZeroType>::value, Operators::UnaryMinus<T> >::type
        operator-( ZeroType a, const T& b) {
            return -b;
        }

        /** \brief Specialize for unary minus. */
        template <typename T, typename S>
        inline
        auto operator-( T&& a, Operators::UnaryMinus<S>&& b) {
            return a + (-b); /* let the double-unary-minus detection take care of peeling b out if it */
        }

        /** \brief Specialize for possible half input! */
        inline
        HalfType operator-( const OneType a, const HalfType b) {
            return b;
        }

        /** \brief Specialize for possible half input! */
        inline
        auto operator-( HalfType a, OneType b) {
            return -a;
        }

        /** \brief A mini struct for instiating the test case. */
        struct SubtractTester {
#ifdef TEMPLATTEST
            static inline void Test(TDDAssertion& tdd);
#endif
        };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/subtract_test.h"
#endif


#endif
