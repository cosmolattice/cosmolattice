#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SQUAREROOT_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SQUAREROOT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/power.h"

#include "TempLat/lattice/algebra/constants/halftype.h"


namespace TempLat {

    /** \brief Enable use of this operator without prefixing std:: or TempLat::. The compiler can distinguish between them. */
    using std::sqrt;



    /** \brief A mini struct for instiating the test case. */
    struct SqrtTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    /** \brief A function which applies a square root, by forwarding to power(x, 0.5), with 0.5 templated explicitly. Holds the expression, only evaluates for a single element when you call SquareRoot::get(pIterCoords).
     *
     * Unit test: make test-multiply
     **/
namespace Operators {
     template<typename R>
     struct SafeSqrt: public UnaryOperator<R> { //Check  if numerator if roughly zero, don't do the division Useful for spectrum fluctuation, when normalising wiht a cutoff
     public:
         using UnaryOperator<R>::mR;

         SafeSqrt(const R& pR): UnaryOperator<R>(pR){}
         auto get(ptrdiff_t i)
         {
             auto a = GetValue::get(mR,i);

             decltype(a) zero(0);

             return (a < zero) ? zero : sqrt(a);
         }
         virtual std::string operatorString() const {
             return "safe_sqrt";
         }
         /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
         template <typename U>
         void d(const U& other) = delete;

     };
   }


     template<typename R>
     typename ConditionalUnaryGetter<Operators::SafeSqrt, R>::type
     safeSqrt(const R& r)
     {
         return Operators::SafeSqrt<R>(r);
     }


    template <typename T>
    inline
    typename ConditionalBinaryGetter<Operators::Power, T, HalfType, ! std::is_arithmetic<T>::value>::type
    sqrt( T a) {
        return Operators::Power<T, HalfType>(a, HalfType());
    }

    /** \brief Specialize for possible zero input! */
    inline
    ZeroType sqrt( ZeroType a) {
        return a;
    }

    /** \brief Specialize for possible unit input! */
    inline
    OneType sqrt( OneType a) {
        return a;
    }

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/squareroot_test.h"
#endif


#endif
