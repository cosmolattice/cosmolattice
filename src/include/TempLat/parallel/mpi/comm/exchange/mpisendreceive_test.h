#ifndef TEMPLAT_PARALLEL_MPI_COMM_EXCHANGE_MPISENDRECEIVE_TEST_H
#define TEMPLAT_PARALLEL_MPI_COMM_EXCHANGE_MPISENDRECEIVE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::MPISendReceive::Test(TempLat::TDDAssertion& tdd) {

    std::vector<int> testVec {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }};
    std::array<int, 10> testArr {{ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1}};

    auto&& arrLikeCompare = [](auto& a, auto& b) {
        if ( a.size() != b.size() ) return false;
        for ( int ii = 0, iiend = a.size(); ii < iiend; ++ii ) {
            if ( a[ii] != b[ii] ) return false;
        }
        return true;
    };

    MPICommReference comm;
    if ( comm.size() > 1 ) {
        /* setup simple pairs: ignore the last rank if the comm size is odd. */
        MPISendReceive speaker(comm);
        int size = comm.size();
        int me = comm.rank();
        if ( ! (size % 2 != 0 && me == size - 1) ) {
            /* We are not the last rank in an odd-sized world. */
            int meFirst = me % 2 == 0;
            int them = meFirst ? me + 1 : me - 1;
            
            /* the tests */
            for ( int i = 0; i < 2; ++i) {
                if ( i == me % 2) {
                    /* we send */
                    say << "Rank " << me << " sends to " << them << "\n";
                    speaker.send(&testVec, them, 100);
                    speaker.send(&testArr, them, 102);
                } else {
                    /* we receive */
                    say << "Rank " << me << " receives from " << them << "\n";
                    std::vector<int> cpVec;
                    std::array<int, 10> cpArr;
                    speaker.receive(&cpVec, them, 100);
                    speaker.receive(&cpArr, them, 102);
                    
                    tdd.verify( arrLikeCompare(testVec, cpVec) );
                    tdd.verify( arrLikeCompare(testArr, cpArr) );

                    
                }
            }
        }
    }
    


}

#endif
