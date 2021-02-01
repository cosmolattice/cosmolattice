#ifndef TEMPLAT_PARALLEL_MPI_COMM_EXCHANGE_MPIALLREDUCE_TEST_H
#define TEMPLAT_PARALLEL_MPI_COMM_EXCHANGE_MPIALLREDUCE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::MPIAllReduce::Test(TempLat::TDDAssertion& tdd) {

#ifndef NOMPI

    MPIAllReduce me(MPI_COMM_WORLD);
    
    int commSize = 1;
    int commRank = 0;
    
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &commRank);

    int allSum = 0;
    for ( int i = 0; i < commSize; ++i) allSum += i;

    tdd.verify( me.computeAllSum(commRank) == allSum );

    tdd.verify( me.computeAllMax(commRank) == commSize - 1 );

    tdd.verify( me.computeAllMin(commRank) == 0 );

    tdd.verify( ! me.computeAllAnd(commRank) );

    tdd.verify( me.computeAllOr(commRank) );


#else
    
    tdd.verify( true );

#endif


}

#endif
