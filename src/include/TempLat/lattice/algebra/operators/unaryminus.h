#ifndef TEMPLAT_LATTICE_ALGEBRA_UNARYMINUS_H
#define TEMPLAT_LATTICE_ALGEBRA_UNARYMINUS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"


namespace TempLat {

    /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
    namespace Operators {
        /** \brief A class which applies a minus sign. Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
         *
         * Unit test: make test-multiply
         **/
        template <typename T>
        class UnaryMinus : public UnaryOperator<T> {
        public:

            /* Yes, need to do this 'using': parent class is template, stuff is not visible to the compiler yet. */
            using UnaryOperator<T>::mR;

            /* Put public methods here. These should change very little over time. */
            UnaryMinus(const T& a) : UnaryOperator<T>(a) {   }

            /** \brief Getter for two instances. */
            inline auto get(ptrdiff_t i)  {
                return -GetValue::get(mR, i);
            }

            /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template <typename U>
            inline auto d(const U& other)  {
                return -GetDeriv::get(mR, other);
            }

            virtual std::string operatorString() const {
                return "-";
            }

        private:
            /* Put all member variables and private methods here. These may change arbitrarily. */


        };

    }
    /** \brief A mini struct for instiating the test case. */
    struct UnaryMinusTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    /** \brief Exposing our newly define multiplication operation to the world. */
    template <typename T>
    inline
    typename ConditionalUnaryGetter<Operators::UnaryMinus, T>::type
    operator-( const T& a) {
        return Operators::UnaryMinus<T>(a);
    }

    /** \brief Specialize for possible zero input! */
    inline
    ZeroType operator-( ZeroType a) {
        return a;
    }

    /** \brief Specialize for double minus signs. */
    template <typename T>
    inline
    auto operator-(Operators::UnaryMinus<Operators::UnaryMinus<T>>&& a) {
        return a;
    }

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/unaryminus_test.h"
#endif


#endif
