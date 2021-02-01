#ifndef TEMPLAT_LATTICE_IO_HDF5_HDF5TESTER_H
#define TEMPLAT_LATTICE_IO_HDF5_HDF5TESTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#ifdef HDF5

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class which tests the hdf5 routines.
     *
     * 
     * Unit test: make test-hdf5tester
     **/

    class HDF5Tester {
    public:
        /* Put public methods here. These should change very little over time. */
        HDF5Tester() {

        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/IO/HDF5/hdf5tester_test.h"
#endif

#endif

#endif
