#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COMPLEXCONJUGATE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COMPLEXCONJUGATE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <complex>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"

namespace TempLat {

    /** \brief Enable use of this operator without prefixing std:: or TempLat::. The compiler can distinguish between them. */
    using std::conj;

    namespace Operators {

        /** \brief A class which applies a minus sign. Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
         *
         * Unit test: make test-multiply
         **/
        template <typename R>
        class ComplexConjugate : public UnaryOperator<R> {
        public:

            /* Yes, need to do this 'using': parent class is template, stuff is not visible to the compiler yet. */
            using UnaryOperator<R>::mR;

            /* Put public methods here. These should change very little over time. */
            ComplexConjugate(const R& a) : UnaryOperator<R>(a) {   }

            /** \brief Getter for two instances. */
            inline auto get(ptrdiff_t i)  {
                using namespace std;
                return conj(GetValue::get(mR, i));
            }

            /** \brief Complex conjugation and copmlex differentiation aren't friends. */
            template <typename U>
            inline auto d(const U& other) = delete;


        private:
            /* Put all member variables and private methods here. These may change arbitrarily. */


        };

    }
    /** \brief A mini struct for instiating the test case. */
    struct ComplexConjugateTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    /** \brief Exposing our newly define multiplication operation to the world. */
    template <typename T>
    typename ConditionalUnaryGetter<Operators::ComplexConjugate,T>::type
    conj(const T& a) {
        return Operators::ComplexConjugate<T>(a);
    }


}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/complexconjugate_test.h"
#endif


#endif
