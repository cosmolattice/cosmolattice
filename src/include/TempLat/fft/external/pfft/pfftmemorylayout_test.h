#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_PFFTMEMORYLAYOUT_TEST_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_PFFTMEMORYLAYOUT_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/parallel/mpi/comm/mpidomainsplit.h"

inline void TempLat::PFFTMemoryLayout::Test(TempLat::TDDAssertion& tdd) {

    MPICommReference world;

//    PFFTMemoryLayout mem; // cannot do this. Need the descending class for that.
    PFFTInterface mem;

    auto&& computeExpectation = [&](const MPICartesianGroup &mGroup,  const std::vector<ptrdiff_t> &nGrid) {

        ptrdiff_t nDim= nGrid.size();
        FFTLayoutStruct expected(nGrid, false, true);
        
        std::vector<ptrdiff_t> confLocalSizes(nGrid);
        std::vector<ptrdiff_t> confLocalStarts(nDim, 0);
        std::vector<ptrdiff_t> fourLocalSizes(nGrid);
        std::vector<ptrdiff_t> fourLocalStarts(nDim, 0);
        std::vector<ptrdiff_t> fourTransposition(nDim);
        std::iota(fourTransposition.begin(), fourTransposition.end(), 0);
        
        fourLocalSizes.back() = fourLocalSizes.back() / 2 + 1;
        confLocalSizes.back() = 2 * fourLocalSizes.back();
        
        expected.fourierSpace = LayoutStruct::createGlobalFFTLayout( nGrid);

        confLocalSizes.back() = 2 * expected.fourierSpace.getLocalSizes().back();


       
        auto decomposition = mGroup.getDecomposition();
        auto position = mGroup.getPosition();

        if ( nDim > 2 && mGroup.size() > 1 && PFFTMemoryLayout::PFFTWITHTRANSPOSITION()) {
            auto nSplit = mGroup.getNumberOfDividedDimensions();
            if ( nSplit > nDim - 1 ) nSplit = nDim - 1;
            for ( ptrdiff_t i = 0; i < nSplit; ++i) {
                fourTransposition[i] = i + 1;
            }
            fourTransposition[nSplit] = 0;
        }

        if ( nDim > 1 ) {
            
            for ( ptrdiff_t i = 0; i < (ptrdiff_t) decomposition.size(); ++i ) {
                confLocalStarts[i] = (nGrid[i] * (ptrdiff_t) position[i]) / (ptrdiff_t) decomposition[i];
                confLocalSizes[i] /= decomposition[i];
                fourLocalStarts[fourTransposition[i]] = (fourLocalSizes[fourTransposition[i]] * (ptrdiff_t) position[i]) / (ptrdiff_t) decomposition[i];
                fourLocalSizes[fourTransposition[i]] /= decomposition[i];
            }
        }


        expected.fourierSpace.setLocalSizes(fourLocalSizes);
        expected.fourierSpace.setLocalStarts(fourLocalStarts);
        expected.fourierSpace.setTranspositionMap_memoryToGlobalSpace(fourTransposition);
        expected.configurationSpace.setLocalSizes(confLocalSizes);
        expected.configurationSpace.setLocalStarts(confLocalStarts);

        
//        say << "expected: " << expected << "\nposition: " << position << "\ndecomposition: " << decomposition << "\n";;
        return expected;
    };

//    say << mem.computeLocalSizes(MPICartesianGroup(2), 4, 128) << "\nexpectation" << computeExpectation(4, 128);

    ptrdiff_t worldSize = MPICommReference().size();
    
    if ( 128 % worldSize ) {
        bool test_Only_Works_For_World_Size_Which_Is_A_Factor_Of_128 = false;
        tdd.verify(test_Only_Works_For_World_Size_Which_Is_A_Factor_Of_128);
    } else {

        
        MPICartesianGroup mGroup4(4, MPIDomainSplit(worldSize, 4, 3));
        MPICartesianGroup mGroup3(3, MPIDomainSplit(worldSize, 3, 2));
        MPICartesianGroup mGroup2(2, MPIDomainSplit(worldSize, 2, 1));
        MPICartesianGroup mGroup1(1, MPIDomainSplit(worldSize, 1, 1));
        tdd.verify( mem.computeLocalSizes(mGroup4, {128,128,128,128}) == computeExpectation(mGroup4, {128,128,128,128}));
        tdd.verify( mem.computeLocalSizes(mGroup4, {128,128,128,128,128}) == computeExpectation(mGroup4, {128,128,128,128,128}));
#ifndef NOMPI
        if ( mGroup4.size() > 3 ) {
            tdd.verify( Throws<PFFTMemoryLayoutException>( [&]() { mem.computeLocalSizes(mGroup4, {256,256}); }));
        }
#endif
        tdd.verify( mem.computeLocalSizes(mGroup2, {128,128}) == computeExpectation(mGroup2, {128,128}));
        tdd.verify( mem.computeLocalSizes(mGroup1, {128,128,128,128,128,128}) == computeExpectation(mGroup1, {128,128,128,128,128,128}));
        

//        tdd.verify( mem.computeLocalSizes(mGroup3, 3, 32) == computeExpectation(mGroup3, 3, 32));
//        say << "decomposition: " << mGroup3.getDecomposition() << "\n" << mem.computeLocalSizes(mGroup3, 3, 32) << "\nexpectation" << computeExpectation(mGroup3, 3, 32);
    }
}

#endif

