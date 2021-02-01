#ifndef TEMPLAT_UTIL_STRINGIFY_H
#define TEMPLAT_UTIL_STRINGIFY_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"


/** \brief A macro used to parse preprocessor variable as strings, credits https://stackoverflow.com/a/34252436.
 *
 *
 *
 * Unit test: make test-stringify
 **/

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)



#endif
