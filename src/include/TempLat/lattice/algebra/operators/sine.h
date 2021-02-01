#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SINE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SINE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/cosine.h"

namespace TempLat {


    /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
    namespace Operators {
        /** \brief A class which applies cosine.
         *
         * Unit test: make test-multiply
         **/
        template <typename T>
        class Sine : public UnaryOperator<T> {
        public:

            /* Yes, need to do this 'using': parent class is template, stuff is not visible to the compiler yet. */
            using UnaryOperator<T>::mR;

            /* Put public methods here. These should change very little over time. */
            Sine(T a) : UnaryOperator<T>(a) {   }

            /** \brief Getter for two instances. */
            inline auto get(ptrdiff_t i)  {
                using namespace std;
                return sin(GetValue::get(mR, i));
            }

            /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template <typename U>
            inline auto d(const U& other)  {
                return GetDeriv::get(mR, other) * cos(mR);
            }

            virtual std::string operatorString() const {
                return "sin";
            }

        private:
            /* Put all member variables and private methods here. These may change arbitrarily. */


        };

    }
    /** \brief A mini struct for instiating the test case. */
    struct SineTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    /** \brief Exposing our newly define exp operation to the world. */
    template <typename T>
    inline
    typename ConditionalUnaryGetter<Operators::Sine, T>::type
    sin( T a) {
        return Operators::Sine<T>(a);
    }

    /** \brief Specialize for possible zero input! */
    inline
    ZeroType sin( ZeroType a) {
        return ZeroType();
    }

} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/sine_test.h"
#endif


#endif
