#ifndef TEMPLAT_LATTICE_ALGEBRA_ADD_H
#define TEMPLAT_LATTICE_ALGEBRA_ADD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasgetmethod.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/halftype.h"

namespace TempLat {

    /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
    namespace Operators {
        /** \brief A class which adds two getters. Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
         *
         * Unit test: make test-multiply
         **/
         template<typename R, typename T>
         class Addition : public BinaryOperator<R,T> {
         public:
           using BinaryOperator<R,T>::mR;
           using BinaryOperator<R,T>::mT;
           Addition(const R& pR, const T& pT): BinaryOperator<R,T>(pR,pT){}
           auto get(ptrdiff_t i)
           {
             return GetValue::get(mT,i)+GetValue::get(mR,i);
           }
           virtual std::string operatorString() const {
               return "+";
           }
           /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template <typename U>
            inline auto d(const U& other) {
                return GetDeriv::get(mT, other) + GetDeriv::get(mR, other);
            }
         };



    }

    /** \brief A mini struct for instiating the test case. */
    struct AddTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<typename R, typename T>
    typename ConditionalBinaryGetter<Operators::Addition, R, T>::type
    operator+(const R& r, const T& t)
    {
      return Operators::Addition<R,T>(r,t);
    }

    /** \brief Specialize for possible zero input! */
    template <typename T>
    inline
    T operator+( T a, const ZeroType b) {
        return a;
    }

    /** \brief Specialize for possible half input! */
    inline
    OneType operator+( const HalfType a, const HalfType b) {
        return OneType();
    }

    /** \brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
    template <typename T>
    inline
    typename std::enable_if<!std::is_same<T, ZeroType>::value, T>::type
    operator+(const ZeroType a,  T b) {
        return b;
    }


}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/add_test.h"
#endif


#endif
