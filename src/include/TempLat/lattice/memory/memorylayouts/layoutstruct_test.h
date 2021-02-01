#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_LAYOUTSTRUCT_TEST_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_LAYOUTSTRUCT_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::LayoutStruct::Test(TempLat::TDDAssertion& tdd) {
    
    /* test the operator== */
    LayoutStruct a({0,0,0});
    LayoutStruct b({0,0,0});
    LayoutStruct c({0,0});
    LayoutStruct d({0,0,0});
    
    d.getLocalSizes()[1] = 2;

    tdd.verify( ! (a == c) );
    tdd.verify( (a == b) );
    tdd.verify( ! (a == d) );

    /* test the transposition */
    a = LayoutStruct({16,16,16});
    std::vector<ptrdiff_t> newLocalSizes {{ 4, 5, 6 }};
    a.setLocalSizes(newLocalSizes);

    /* test that these propagate correctly */
    tdd.verify( a.getLocalSizes()[0] == a.getSizesInMemory()[0]);
    tdd.verify( a.getLocalSizes()[1] == a.getSizesInMemory()[1]);
    tdd.verify( a.getLocalSizes()[2] == a.getSizesInMemory()[2]);

    std::vector<ptrdiff_t> newLocalStarts {{ 7, 8, 9 }};
    a.setLocalStarts(newLocalStarts);

    a.setTranspositionMap_memoryToGlobalSpace({{2, 0, 1}});



    std::vector<ptrdiff_t> memVec(3), posVec(3), memVec2(3);
    
    memVec[0] = 1;
    memVec[1] = 2;
    memVec[2] = 3;

    a.putSpatialLocationFromMemoryIndexInto(memVec[0], 0, posVec);
    a.putSpatialLocationFromMemoryIndexInto(memVec[1], 1, posVec);
    a.putSpatialLocationFromMemoryIndexInto(memVec[2], 2, posVec);
    
    say << "memVec " << memVec << " -> posVec " << posVec << "\n";
    
    /* mem pos 2 at mem dim 1 -> global dim 0 -> global pos 2 + 7 = 9 -> 9 - 16 = -7 */
    tdd.verify(posVec[0] == -7);

    /* mem pos 3 at mem dim 2 -> global dim 1 -> global pos 3 + 8 = 11 -> 11 - 16 = -5 */
    tdd.verify(posVec[1] == -5);

    /* mem pos 1 at mem dim 0 -> global dim 2 -> global pos 1 + 9 = 10 -> 10 - 16 = -6 */
    tdd.verify(posVec[2] == -6);

    a.putMemoryIndexFromSpatialLocationInto(posVec[0], 0, memVec2);
    a.putMemoryIndexFromSpatialLocationInto(posVec[1], 1, memVec2);
    a.putMemoryIndexFromSpatialLocationInto(posVec[2], 2, memVec2);
    
    say << "posVec " << posVec << " -> memVec2 " << memVec2 << "\n";
    say << "memVec " << memVec << " -> memVec2 " << memVec2 << "\n";
    say << " from layout:\n" << a << "\n";
    tdd.verify(memVec[0] == memVec2[0]);
    tdd.verify(memVec[1] == memVec2[1]);
    tdd.verify(memVec[2] == memVec2[2]);
    
    say << "layout: " << a << "\n";

}

#endif
