#ifndef COSMOINTERFACE_INITIALIZERS_SU2INITIALIZER_TEST_H
#define COSMOINTERFACE_INITIALIZERS_SU2INITIALIZER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "TempLat/util/tdd/tdd.h"

inline void TempLat::SU2Initializer::Test(TempLat::TDDAssertion &tdd) { tdd.verify(true); }

#endif
