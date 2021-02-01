#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_ABSOLUTEVALUE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_ABSOLUTEVALUE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

/* imperative to include this: otherwise abs might be defined for integers only, with possibly desastrous consequences. */
#include <cmath>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"

#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/heavisidestepfunction.h"

namespace TempLat {

    /** \brief Enable use of this operator without prefixing std:: or TempLat::. The compiler can distinguish between them. */
    using std::abs;

    namespace Operators {
        /** \brief A class which applies takes the absolute value. Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
         *
         * Unit test: make test-multiply
         **/
         template <typename R>
         class AbsoluteValue : public UnaryOperator<R> {
         public:
           using UnaryOperator<R>::mR;


             /* Put public methods here. These should change very little over time. */
             AbsoluteValue(const R& a) : UnaryOperator<R>(a)
             {

             }

             /** \brief Getter for two instances. */
             inline typename GetFloatType<typename GetGetReturnType<R>::type>::type
             get(ptrdiff_t i)  {
                 using namespace std;
                 return abs(GetValue::get(mR, i));
             }

             std::string operatorString() const {
                 return "abs";
             }

             /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
             template <typename U>
             inline auto d(const U& other)  {
                 return GetDeriv::get(mR, other) * ( -heaviside( - mR ) + heaviside(mR) );
             }

         private:
             /* Put all member variables and private methods here. These may change arbitrarily. */


         };

    }
    /** \brief A mini struct for instiating the test case. */
    struct AbsoluteValueTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    /** \brief Exposing our newly define multiplication operation to the world. */

    template <typename T>
    inline
    typename ConditionalUnaryGetter<Operators::AbsoluteValue, T>::type
    abs( const T& a) {
      return Operators::AbsoluteValue<T>(a);
    }
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/absolutevalue_test.h"
#endif


#endif
