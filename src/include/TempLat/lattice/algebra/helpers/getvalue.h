#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVALUE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVALUE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasgetmethod.h"

namespace TempLat {

    /** \brief A template-programming class which helps to get the 'get'-value from any type, whether it has the get-method or it is a scalar value.
     *
     * Unit test: make test-getvalue
     **/



   class GetValue {
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
       typename std::enable_if<HasGetMethod<U>::value, decltype(std::declval<U>().get(std::declval<ptrdiff_t>()))>::type
       get( U&& obj, ptrdiff_t i) {
           return obj.get(i);
       }

       template <typename U>
       static inline
       typename std::enable_if<!HasGetMethod<U>::value, U>::type&
       get( U&& obj, ptrdiff_t i) {
           return obj;
       }

   private:
   /* Put all member variables and private methods here. These may change arbitrarily. */
       GetValue() {

       }
     public:
#ifdef TEMPLATTEST
         static inline void Test(TDDAssertion& tdd);
#endif
   };
}







#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/getvalue_test.h"
#endif


#endif
