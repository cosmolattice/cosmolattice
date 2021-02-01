#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYBLOCK_TEST_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYBLOCK_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


template <typename T>
inline void TempLat::MemoryBlock<T>::Test(TempLat::TDDAssertion& tdd) {

    MemoryBlock<T> test(128);
    
    /* verified that we get segfault on i == test.size(). */
    for ( ptrdiff_t i = 0, iEnd = test.size(); i < iEnd; ++i) {
        test[i] = i;
    }

    tdd.verify( true );

}

#endif
