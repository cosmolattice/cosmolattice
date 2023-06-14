#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_POWER_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_POWER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <cmath>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/operators/add.h"
#include "TempLat/lattice/algebra/operators/subtract.h"
#include "TempLat/lattice/algebra/conditional/conditionalbinarygetter.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"
#include "TempLat/util/rangeiteration/tagliteral.h"


namespace TempLat {

    /** \brief Enable use of this operator without prefixing std:: or TempLat::. The compiler can distinguish between them. */
    using std::pow;

    /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
    namespace Operators {

        /** \brief A class which takes one getter to the power of another getter. Holds the expression, only evaluates for a single element when you call Power::get(pIterCoords).
         *
         * Unit test: make test-power
         **/
         template<typename R, typename T>
         class Power : public BinaryOperator<R,T> {
         public:
           using BinaryOperator<R,T>::mR;
           using BinaryOperator<R,T>::mT;
           Power(const R& pR, const T& pT): BinaryOperator<R,T>(pR,pT){}
           auto get(ptrdiff_t i)
           {
             using namespace std;
             return pow(GetValue::get(mR,i),GetValue::get(mT,i));
           }
           virtual std::string operatorString() const {
               return "^";
           }
           /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
           template <typename U>
           inline auto d(const U& other)  {
               using namespace std; /* so the compiler chooses without problems between std::log and TempLat::Operators::log */
               return GetDeriv::get(mR, other) * pow(mR, mT - OneType()) + GetDeriv::get(mT, other) * (*this) * log(mT);
           }
         };


//This class is to make sure std::pow get a static integer. Might be a more elegant way to do it!
         template<int N, typename R>
         class PowerN : public UnaryOperator<R>{
         public:
           using UnaryOperator<R>::mR;
           PowerN(const R& pR): UnaryOperator<R>(pR){}
           auto get(ptrdiff_t i)
           {
             using namespace std;
             return pow(GetValue::get(mR,i),N);
           }

           virtual std::string toString() const {
               return "("+GetString::get(mR) + ")^" + std::to_string(2);
           }

           /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template <typename U>
            inline auto d(const U& other)  {
                using namespace std; /* so the compiler chooses without problems between std::log and TempLat::Operators::log */
                return Tag<N>() * PowerN<N-1,R>(mR) * GetDeriv::get(mR, other);
            }

         };

        template<typename R>
        class PowerN<2,R> : public UnaryOperator<R>{
        public:
            using UnaryOperator<R>::mR;
            PowerN(const R& pR): UnaryOperator<R>(pR){}
            auto get(ptrdiff_t i)
            {
                return GetValue::get(mR,i) * GetValue::get(mR,i);
            }

            virtual std::string toString() const {
                return "("+GetString::get(mR) + ")^" + std::to_string(2);
            }

            /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template <typename U>
            inline auto d(const U& other)  {
                using namespace std; /* so the compiler chooses without problems between std::log and TempLat::Operators::log */
                return 2_c * mR * GetDeriv::get(mR,other);
            }

        };

        template<typename R>
        class PowerN<3,R> : public UnaryOperator<R>{
        public:
            using UnaryOperator<R>::mR;
            PowerN(const R& pR): UnaryOperator<R>(pR){}
            auto get(ptrdiff_t i)
            {
                return GetValue::get(mR,i) * GetValue::get(mR,i) * GetValue::get(mR,i);
            }

            virtual std::string toString() const {
                return "("+GetString::get(mR) + ")^" + std::to_string(2);
            }

            /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template <typename U>
            inline auto d(const U& other)  {
                using namespace std; /* so the compiler chooses without problems between std::log and TempLat::Operators::log */
                return 3_c * mR * mR * GetDeriv::get(mR,other);
            }

        };

        template<typename R>
        class PowerN<4,R> : public UnaryOperator<R>{
        public:
            using UnaryOperator<R>::mR;
            PowerN(const R& pR): UnaryOperator<R>(pR){}
            auto get(ptrdiff_t i)
            {
                auto tmp = GetValue::get(mR,i);
                tmp = tmp *tmp;
                return  tmp * tmp;
            }

            virtual std::string toString() const {
                return "("+GetString::get(mR) + ")^" + std::to_string(2);
            }

            /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template <typename U>
            inline auto d(const U& other)  {
                using namespace std; /* so the compiler chooses without problems between std::log and TempLat::Operators::log */
                return 4_c * mR * mR * mR * GetDeriv::get(mR,other);
            }

        };
    }


     template<typename R, typename T>
     inline
     typename ConditionalBinaryGetter<Operators::Power, R, T>::type
     pow(const R& r, const T& t)
     {
       return Operators::Power<R,T>(r,t);
     }
	
	  template <int N>
      inline
      ZeroType
      pow(ZeroType) {
          return ZeroType();
      }

     template <typename T>
     inline
     OneType pow(const T& a, ZeroType b) {
         return OneType();
     }

     /** \brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
     template <typename T>
     inline
     typename std::enable_if<!std::is_same<T, ZeroType>::value, ZeroType>::type
     pow(ZeroType a, const T& b) {
         return ZeroType();
     }


    //enable if is just so that we can overload to consitently write pow<3>(4)  for std::pow(4,3);
    template<int N, typename R>
    typename std::enable_if<HasGetMethod<R>::value && N!=1 && N!=0 , Operators::PowerN<N,R>>::type
    pow(const R& r)
    {
      return Operators::PowerN<N,R>(r);
    }



    //overload so that we can sonsitently write pow<3>(4)  for std::pow(4,3);
    template<int N, typename R>
    typename std::enable_if<!HasGetMethod<R>::value && N != 0 && N != 1 && !(IsTempLatGettable<0,R>::value || IsSTDGettable<0,R>::value ),R>::type
    pow(const R& r)
    {
      return std::pow(r,N);
    }

    /** \brief Specialize for possible zero input! */
      template <int N, typename T>
      inline
      typename std::enable_if<N == 0,OneType>::type
      pow(const T& a) {
          return OneType();
      }

      /** \brief Specialize for possible one input! */
        template <int N, typename T>
        inline
        typename std::enable_if<N == 1,T>::type
        pow(const T& a) {
            return a;
        }


    /** \brief A mini struct for instiating the test case. */
    struct PowerTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/power_test.h"
#endif


#endif
