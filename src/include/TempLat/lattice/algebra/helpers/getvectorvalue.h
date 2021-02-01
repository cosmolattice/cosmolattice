#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORVALUE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORVALUE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasvectorgetmethod.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"

namespace TempLat {


    /** \brief A getter for the vector-like algebra.
     *
     *
     * Unit test: make test-getvectorvalue
     **/

     class GetVectorValue {
     /** \brief A simple class which provides a get method for basic types.
      * Now you can use an int as if it had a get method for example.
      *
      *
      *
      **/
     public:
     /* Put public methods here. These should change very little over time. */


         template <typename U>
         static inline
         typename std::enable_if<HasVectorGetMethod<U>::value, decltype(std::declval<U>().vectorGet(std::declval<ptrdiff_t>(),std::declval<ptrdiff_t>()))>::type
         vectorGet( U& obj, ptrdiff_t i, ptrdiff_t j) {
             return obj.vectorGet(i,j);
         }

         template <typename U>
         static inline
         typename std::enable_if<!HasVectorGetMethod<U>::value, decltype((GetValue::get(std::declval<U>(),std::declval<ptrdiff_t>())))>::type
         vectorGet( U&& obj, ptrdiff_t i, ptrdiff_t j) {
             return GetValue::get(obj,i);
         }

     private:
     /* Put all member variables and private methods here. These may change arbitrarily. */
         GetVectorValue() {

         }
       public:
#ifdef TEMPLATTEST
           static inline void Test(TDDAssertion& tdd);
#endif
     };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/getvectorvalue_test.h"
#endif


#endif
