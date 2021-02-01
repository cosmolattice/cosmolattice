#ifndef TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_ZEROTYPE_H
#define TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_ZEROTYPE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


  /** \brief A class which represents zero. Attempt to simplify derivative expressions.
   *
   * Unit test: make test-zerotype
   **/
 /* struct ZeroType {
//        operator ptrdiff_t() const { return 0; }
      std::string toString() const { return "(ZeroType)0"; }
      ptrdiff_t get(ptrdiff_t i) { return 0; }
      static constexpr bool ISCONSTANT = true;
  };*/

    struct ZeroType {
//        operator ptrdiff_t() const { return 0; }
        std::string toString() const { return "(ZeroType)0"; }
        ptrdiff_t get(ptrdiff_t i) { return 0; }
        static constexpr bool ISCONSTANT = true;
    };



} /* TempLat */


#endif
