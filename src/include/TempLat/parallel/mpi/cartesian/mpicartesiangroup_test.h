#ifndef TEMPLAT_PARALLEL_MPI_COMM_MPICARTESIANGROUP_TEST_H
#define TEMPLAT_PARALLEL_MPI_COMM_MPICARTESIANGROUP_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::MPICartesianGroup::Test(TempLat::TDDAssertion& tdd) {
    
    auto world = MPICommReference();

    MPICartesianGroup mcg(world, 3, {{1, 1, 1}});

    MPICartesianGroup mcg2(world, 1, {{2, 2, 2}});

    tdd.verify( true );

}

#endif
