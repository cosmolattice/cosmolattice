#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORSIZE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORSIZE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasvectorgetmethod.h"

namespace TempLat {


    /** \brief A class which gets the size of a vector like composite object.
     *
     *
     * Unit test: make test-getvectorsize
     **/

     class GetVectorSize {
     /** \brief A simple class which provides a getVectorSize method for basic types.
      *
      *
      *
      *
      **/
     public:
     /* Put public methods here. These should change very little over time. */


         template <typename U>
         static inline
         typename std::enable_if<HasVectorGetMethod<U>::value, ptrdiff_t>::type
         getVectorSize( U& obj) {
             return obj.getVectorSize();
         }

         template <typename U>
         static inline
         typename std::enable_if<!HasVectorGetMethod<U>::value, ptrdiff_t>::type
         getVectorSize( U&& obj) {
             return 1;
         }

     private:
     /* Put all member variables and private methods here. These may change arbitrarily. */
         GetVectorSize() {

         }
       public:
#ifdef TEMPLATTEST
           static inline void Test(TDDAssertion& tdd);
#endif
     };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/getvectorsize_test.h"
#endif


#endif
