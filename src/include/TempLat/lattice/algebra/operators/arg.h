#ifndef TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_ARG_H
#define TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_ARG_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/constants.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"

#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexfieldget.h"


namespace TempLat {


    /** \brief A class which compute the argument of a complex field.
     *
     * 
     * Unit test: make test-arg
     **/


    namespace Operators {

        template <typename R, typename  T>
        class Arg : public BinaryOperator<R, T> {
        public:

            /* Yes, need to do this 'using': parent class is template, stuff is not visible to the compiler yet. */
            using BinaryOperator<R,T>::mR;
            using BinaryOperator<R,T>::mT;

            /* Put public methods here. These should change very little over time. */
            Arg(R r, T t) : BinaryOperator<R,T>(r,t) {   }

            /** \brief Getter for two instances. */
            inline auto get(ptrdiff_t i)  {
                using namespace std; /* this way, for potential future data types. */
                auto res = atan2(GetValue::get(mR, i), GetValue::get(mR, i));
                return  AlmostEqual(res,0) ? 0 : ((res>0) ? res :res + 2 * Constants::pi<double> );
            }

            /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
            template <typename U>
            void d(const U& other) = delete;

            virtual std::string operatorString() const {
                return "arg";
            }

        private:
            /* Put all member variables and private methods here. These may change arbitrarily. */
        };

    }

    struct ArgTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template <typename R, typename T>
    //typename std::enable_if<HasComplexFieldGet<R>::value, Operators::Arg<decltype(std::declval<R>().ComplexFieldGet(std::declval<Tag<0>()>)),decltype(std::declval<R>().ComplexFieldGet(std::declval<Tag<1>()>))>>::type
    typename std::enable_if<HasGetMethod<R>::value && HasGetMethod<T>::value , Operators::Arg<R,T>>::type
    arg(R r, T t)
    {
        return {r,t};
    }



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/arg_test.h"
#endif


#endif
