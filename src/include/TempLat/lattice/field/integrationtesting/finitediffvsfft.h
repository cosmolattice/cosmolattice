#ifndef TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_FINITEDIFFVSFFT_H
#define TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_FINITEDIFFVSFFT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/cosmolattice.h"

namespace TempLat {

    /** \brief Possibly the ultimate test for the implementations of
     *   various spatial derivatives: compare finite difference results
     *   with FFT results.
     *
     * Unit test: make test-finitediffvsfft
     **/

    class FiniteDiffVSFFT {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/field/integrationtesting/finitediffvsfft_test.h"
#endif


#endif
