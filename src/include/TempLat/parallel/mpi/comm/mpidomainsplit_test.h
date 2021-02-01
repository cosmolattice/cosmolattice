#ifndef TEMPLAT_PARALLEL_MPI_COMM_MPIDOMAINSPLIT_TEST_H
#define TEMPLAT_PARALLEL_MPI_COMM_MPIDOMAINSPLIT_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::MPIDomainSplit::Test(TempLat::TDDAssertion& tdd) {


    tdd.verify(
               DoesNotThrow<MPIDomainSplitException>(
                                                     []() {

                                                         MPIDomainSplit dom(28, 4, 3);
                                                         MPIDomainSplit dom1(28, 4, 2);
                                                         MPIDomainSplit dom2(32, 3, 2);
                                                         MPIDomainSplit dom3(28, 2, 1);
                                                     }

                                                     )
               );

}

#endif
