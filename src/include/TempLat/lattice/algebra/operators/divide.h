#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIVIDE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIVIDE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/operators/subtract.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"

#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"

namespace TempLat {

    /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
    namespace Operators {

        /** \brief A class which multiplies two getters. Holds the expression, only evaluates for a single element when you call Divide::get(pIterCoords).
         *
         * Unit test: make test-divide
         **/
        template<typename R, typename T>
        struct Division : public BinaryOperator<R,T> {
        public:
          using BinaryOperator<R,T>::mR;
          using BinaryOperator<R,T>::mT;
          Division(const R& pR, const T& pT): BinaryOperator<R,T>(pR,pT){}
          auto get(ptrdiff_t i)
          {
            return GetValue::get(mR,i)/GetValue::get(mT,i);
          }
          virtual std::string operatorString() const {
              return "/";
          }
          /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
          template <typename U>
          inline auto d(const U& other)  {
              /* not using pow for mT * mT, because pow imports log which imports us, divide.h */
              return GetDeriv::get(mR, other) / mT - GetDeriv::get(mT, other) * mR / (mT * mT);
          }

        };

        template<typename R, typename T>
        struct SafeDivision : public BinaryOperator<R,T> { //Check  if numerator if roughly zero, don't do the division Useful for spectrum fluctuation, when normalising wiht a cutoff
        public:
            using BinaryOperator<R,T>::mR;
            using BinaryOperator<R,T>::mT;
            SafeDivision(const R& pR, const T& pT): BinaryOperator<R,T>(pR,pT){}
            auto get(ptrdiff_t i)
            {
                auto a = GetValue::get(mR,i);
                auto b = GetValue::get(mT,i);

                decltype(a) zero(0);

                return AlmostEqual(a,zero) ? zero : a/b;
            }
            virtual std::string operatorString() const {
                return "/safe/";
            }
            /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template <typename U>
            inline auto d(const U& other)  {
                /* not using pow for mT * mT, because pow imports log which imports us, divide.h */
                return GetDeriv::get(mR, other) / mT - GetDeriv::get(mT, other) * mR / (mT * mT);
            }

        };

    }
    /** \brief A mini struct for instiating the test case. */
    struct DivideTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    /** \brief Exposing our newly define multiplication operation to the world. */
        template<typename R, typename T>
        typename ConditionalBinaryGetter<Operators::Division, R, T>::type
        operator/(const R& r, const T& t)
        {
          return Operators::Division<R,T>(r,t);
        }

    template<typename R, typename T>
    typename ConditionalBinaryGetter<Operators::SafeDivision, R, T>::type
    safeDivide(const R& r, const T& t)
    {
        return Operators::SafeDivision<R,T>(r,t);
    }

        /** \brief Specialize for possible unit input! Simplify derivatives for example. */
      template <typename T>
      inline
      T operator/(const T& a, OneType b) {
          return a;
      }

      /** \brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
      template <typename T>
      inline
      typename std::enable_if<!std::is_same<T, ZeroType>::value, ZeroType>::type
      operator/(ZeroType a, const T& b) {
          return a;
      }

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/divide_test.h"
#endif


#endif
