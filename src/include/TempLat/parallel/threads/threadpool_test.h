#ifndef TEMPLAT_PARALLEL_THREADS_THREADPOOL_TEST_H
#define TEMPLAT_PARALLEL_THREADS_THREADPOOL_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::ThreadPool::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */

    ThreadPool tP;

    auto test = std::make_shared<std::array<size_t, 10>>();

    for(size_t i=0; i < 4;++i)
    {
        tP.addThread([test,i]{(*test)[2*i] = i; (*test)[2*i + 1] = i;});
    }
    tP.join();

    bool allGood = true;
    for(size_t i = 0; i < test->size() ;++i)
    {
        allGood = allGood && (*test)[i] == (i < 8u ? i/2 : 0);
    }

    tdd.verify( allGood );

}

#endif
