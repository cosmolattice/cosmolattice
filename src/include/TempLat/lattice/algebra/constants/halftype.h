#ifndef TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_HALFTYPE_H
#define TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_HALFTYPE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class which implement 1/2 as a type. Useful for squareroots.
     *
     *
     * Unit test: make test-halftype
     **/

     struct HalfType {
 //        operator double() const { return 0.5; }
         std::string toString() const { return "(HalfType)1/2"; }
         double get(ptrdiff_t i) { return 0.5; }
         /** \brief Need a static instance of halftype, for constructing `Power(T& a, HalfType& ht);` */
         static HalfType& getStatic() {
             static HalfType ht;
             return ht;
         }
         static constexpr bool ISCONSTANT = true;
     };

} /* TempLat */

#ifdef TEMPLATTEST
//#include "TempLat/lattice/algebra/constants/halftype_test.h"
#endif


#endif
