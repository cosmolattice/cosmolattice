#ifndef TEMPLAT_LATTICE_MEMORY_TRIPLESTATELAYOUTS_TEST_H
#define TEMPLAT_LATTICE_MEMORY_TRIPLESTATELAYOUTS_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::TripleStateLayouts::Test(TempLat::TDDAssertion& tdd) {
    
    FFTLibrarySelector theLibrary(MPICartesianGroup(4, {{ MPICommReference().size(), 1, 1, 1 }}), {256,256,256,256});
    
    TripleStateLayouts tsl(theLibrary.getLayout(), 10);
    
    if ( TDDRegister::isSingleUnitTest() ) {
        
        say << tsl << "\n";
        
    }
    
    /* compute the expected memory size: */
    ptrdiff_t d1Local = 256 / (ptrdiff_t) MPICommReference().size();
    
    ptrdiff_t expectedMem = (d1Local + 2 * 10) * std::pow(256 + 2 * 10, 4 - 1);
    
    
    
    tdd.verify( tsl.getNecessaryMemoryAllocation() == expectedMem );
    
}

#endif
