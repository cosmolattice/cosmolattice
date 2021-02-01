#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_FFTWMEMORYLAYOUT_TEST_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_FFTWMEMORYLAYOUT_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::FFTWMemoryLayout::Test(TempLat::TDDAssertion& tdd) {
    
    MPICommReference world;
    
    //    FFTWMemoryLayout mem; // cannot do this. Need the descending class for that.
    FFTWInterface mem;
    
    auto&& computeExpectation = [&](const std::vector<ptrdiff_t> &nGrid) {
        FFTLayoutStruct expected(nGrid, true, false);
        ptrdiff_t nDim = nGrid.size();
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


       
        
        
        if ( nDim > 1 ) {
            confLocalStarts[0] = (nGrid[0] * (ptrdiff_t) world.rank()) / (ptrdiff_t) world.size();
            confLocalSizes[0] /= world.size();
            fourLocalStarts[0] = (nGrid[0] * (ptrdiff_t) world.rank()) / (ptrdiff_t) world.size();
            fourLocalSizes[0] /= world.size();
        }
        
        if ( nDim > 2 && world.size() > 1 ) {
            std::swap(fourLocalStarts[0], fourLocalStarts[1]);
            std::swap(fourLocalSizes[0], fourLocalSizes[1]);
            std::swap(fourTransposition[0], fourTransposition[1]);
        }


        expected.fourierSpace.setLocalSizes(fourLocalSizes);
        expected.fourierSpace.setLocalStarts(fourLocalStarts);
        expected.fourierSpace.setTranspositionMap_memoryToGlobalSpace(fourTransposition);
        expected.configurationSpace.setLocalSizes(confLocalSizes);
        expected.configurationSpace.setLocalStarts(confLocalStarts);

        return expected;
    };
    
    //    say << mem.computeLocalSizes(MPICartesianGroup(2), 4, 128) << "\nexpectation" << computeExpectation(4, 128);
    
    ptrdiff_t wSize = world.size();
    if ( 128 % wSize ) {
        bool test_Only_Works_For_World_Size_Which_Is_A_Factor_Of_128 = false;
        tdd.verify(test_Only_Works_For_World_Size_Which_Is_A_Factor_Of_128);
    } else {
        
        auto&& doSingleTest = [&](std::vector<ptrdiff_t> ng) {
            ptrdiff_t nd = ng.size();
            std::vector<int> decompose(nd, 1);
            decompose[0] = world.size();
            auto fromLibrary = mem.computeLocalSizes(MPICartesianGroup(nd, decompose),  ng);
            auto expected = computeExpectation(ng);
            bool result = fromLibrary == expected;
            tdd.verify(fromLibrary == expected);
            if ( ! result) {
                say << "Failure for layouts, from library:\n" << fromLibrary <<"\bexpected:\n" << expected << "\n\n";
            }
        };
        
        doSingleTest({128,128});
        doSingleTest( {128,128,128,128});
        doSingleTest({128,128,128,128,128,128});

//        tdd.verify( mem.computeLocalSizes(MPICartesianGroup(4, {{ world.size(), 1, 1, 1}} ), 4, 128) == computeExpectation(4, 128));
//        tdd.verify( mem.computeLocalSizes(MPICartesianGroup(2, {{ world.size(), 1}} ), 2, 128) == computeExpectation(2, 128));
//        tdd.verify( mem.computeLocalSizes(MPICartesianGroup(1, {{ world.size(), 1, 1, 1, 1, 1}} ), 6, 128) == computeExpectation(6, 128));
    }
    
    
//    say << mem.computeLocalSizes(MPICartesianGroup(4, {{ world.size(), 1, 1, 1}} ), 4, 128) << "\nexpectation:\n" << computeExpectation(4, 128);

}

#endif

