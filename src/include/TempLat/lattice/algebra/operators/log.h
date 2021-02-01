#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LOG_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LOG_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <cmath>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/operators/divide.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"

namespace TempLat {

    /** \brief Enable use of this operator without prefixing std:: or TempLat::. The compiler can distinguish between them. */
    using std::log;

    /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
    namespace Operators {
        /** \brief A class which applies a minus sign. Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
         *
         * Unit test: make test-multiply
         **/
        template <typename T>
        class Log : public UnaryOperator<T> {
        public:

            /* Yes, need to do this 'using': parent class is template, stuff is not visible to the compiler yet. */
            using UnaryOperator<T>::mR;

            /* Put public methods here. These should change very little over time. */
            Log(T a) : UnaryOperator<T>(a) {   }

            /** \brief Getter for two instances. */
            inline auto get(ptrdiff_t i)  {
                using namespace std; /* not std::log, but this way, for potential future data types. */
                return log(GetValue::get(mR, i));
            }

            /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template <typename U>
            inline auto d(const U& other)  {
                /* not using pow for 1/mInstanceT because pow imports us, log.h */
                return GetDeriv::get(mR, other) / mR;
            }

            virtual std::string operatorString() const {
                return "log";
            }

        private:
            /* Put all member variables and private methods here. These may change arbitrarily. */


        };

    }
    /** \brief A mini struct for instiating the test case. */
    struct LogTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    /** \brief Exposing our newly define log operation to the world. */
    template <typename T>
    inline
    typename ConditionalUnaryGetter<Operators::Log, T>::type
    log( T a) {
        return Operators::Log<T>(a);
    }

    /** \brief Specialize for possible zero output! */
    inline
    ZeroType log( OneType a) {
        return ZeroType();
    }

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/log_test.h"
#endif


#endif
