#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIRACDELTAFUNCTION_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIRACDELTAFUNCTION_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
//#include "TempLat/lattice/algebra/constants/positiveinfinitytype.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"

namespace TempLat {


    namespace Operators {


        /** \brief A class which takes the heaviside step function,
         *  H(x) = 1 if x >= 0 else 0;
         *  This is your tool of choice if you want to create if-statements inside equations.
         *
         * Unit test: make test-diracdeltafunction
         **/
        template <typename R>
        class DiracDeltaFunction : public UnaryOperator<R> {
        public:

            /* Yes, need to do this 'using': parent class is template, stuff is not visible to the compiler yet. */
            using UnaryOperator<R>::mR;

            DiracDeltaFunction(const R& a) : UnaryOperator<R>(a) {   }

            /** \brief Getter for two instances. */
            inline auto get(ptrdiff_t i)  {
                typedef typename GetGetReturnType<R>::type mType;
                mType objValue = GetValue::get(mR, i);
                bool isZero = objValue == mType(0);
                return  isZero ? std::numeric_limits<mType>::infinity() : mType(0);
            }

            /** \brief Does anyone need derivatives of the delta function? If so, go ahead and figure it out. */
            template <typename U>
            void d(const U& other) = delete;
//            {
//                return GetDeriv::get(mInstanceT, other) * DiracDelta(mInstanceT);
//            }

            virtual std::string operatorString() const {
                return "DiracDelta";
            }

        private:
            /* Put all member variables and private methods here. These may change arbitrarily. */


        };

    }
    /** \brief A mini struct for instiating the test case. */
    struct DiracDeltaFunctionTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    /** \brief Exposing our newly define multiplication operation to the world. */
    template <typename T>
    inline
    Operators::DiracDeltaFunction<T>
    DiracDelta( const T& a) {
        return Operators::DiracDeltaFunction<T>(a);
    }


}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/diracdeltafunction_test.h"
#endif


#endif
