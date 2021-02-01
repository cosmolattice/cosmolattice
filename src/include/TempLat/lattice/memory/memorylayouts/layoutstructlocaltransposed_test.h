#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCALTRANSPOSED_TEST_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCALTRANSPOSED_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::LayoutStructLocalTransposed::Test(TempLat::TDDAssertion& tdd) {


    /* test the operator== */
    LayoutStructLocalTransposed a({0,0,0});
    LayoutStructLocalTransposed b({0,0,0});
    LayoutStructLocalTransposed c({0,0});
    LayoutStructLocalTransposed d({0,0,0});
    
    d.getLocal().getLocalSizes()[1] = 2;

    tdd.verify( ! (a == c) );
    tdd.verify( (a == b) );
    tdd.verify( ! (a == d) );

    /* */
    a = LayoutStructLocalTransposed({16,16,16});

    std::vector<ptrdiff_t> newLocalStarts {{ 7, 8, 9 }};
    a.getLocal().setLocalStarts(newLocalStarts);

    /* test that these propagate correctly */
    tdd.verify( a.getLocal().getLocalSizes()[0] == a.getSizesInMemory()[0]);
    tdd.verify( a.getLocal().getLocalSizes()[1] == a.getSizesInMemory()[1]);
    tdd.verify( a.getLocal().getLocalSizes()[2] == a.getSizesInMemory()[2]);


    a.setTranspositionMap_memoryToGlobalSpace({{2, 0, 1}});
    /* test that these propagate correctly */
    tdd.verify( a.getLocal().getLocalSizes()[0] == a.getSizesInMemory()[1]);
    tdd.verify( a.getLocal().getLocalSizes()[1] == a.getSizesInMemory()[2]);
    tdd.verify( a.getLocal().getLocalSizes()[2] == a.getSizesInMemory()[0]);

    std::vector<ptrdiff_t> memVec(3), posVec(3), memVec2(3);


    memVec[0] = 1;
    memVec[1] = 2;
    memVec[2] = 3;

    for ( ptrdiff_t i = 0; i < 3; ++i) {
        auto map = a.getSpatialLocationFromMemoryIndex(memVec[i], i);
        posVec[map.atIndex] = map.withValue;
    }

    say << "memVec " << memVec << " -> posVec " << posVec << "\n";
    
    /* mem pos 2 at mem dim 1 -> global dim 0 -> global pos 2 + 7 = 9 -> 9 - 16 = -7 */
    tdd.verify(posVec[0] == -7);

    /* mem pos 3 at mem dim 2 -> global dim 1 -> global pos 3 + 8 = 11 -> 11 - 16 = -5 */
    tdd.verify(posVec[1] == -5);

    /* mem pos 1 at mem dim 0 -> global dim 2 -> global pos 1 + 9 = 10 -> 10 - 16 = -6 */
    tdd.verify(posVec[2] == -6);


    for ( ptrdiff_t i = 0; i < 3; ++i) {
        auto map = a.getMemoryIndexFromSpatialLocation(posVec[i], i);
        memVec2[map.atIndex] = map.withValue;
    }
    say << "posVec " << posVec << " -> memVec2 " << memVec2 << "\n";
    say << "memVec " << memVec << " -> memVec2 " << memVec2 << "\n";

    tdd.verify(memVec[0] == memVec2[0]);
    tdd.verify(memVec[1] == memVec2[1]);
    tdd.verify(memVec[2] == memVec2[2]);



    a = LayoutStructLocalTransposed({12,16,18});

    newLocalStarts = std::vector<ptrdiff_t> {{ 7, 8, 9 }};
    a.getLocal().setLocalStarts(newLocalStarts);

    /* test that these propagate correctly */
    tdd.verify( a.getLocal().getLocalSizes()[0] == a.getSizesInMemory()[0]);
    tdd.verify( a.getLocal().getLocalSizes()[1] == a.getSizesInMemory()[1]);
    tdd.verify( a.getLocal().getLocalSizes()[2] == a.getSizesInMemory()[2]);


    a.setTranspositionMap_memoryToGlobalSpace({{2, 0, 1}});
    /* test that these propagate correctly */
    tdd.verify( a.getLocal().getLocalSizes()[0] == a.getSizesInMemory()[1]);
    tdd.verify( a.getLocal().getLocalSizes()[1] == a.getSizesInMemory()[2]);
    tdd.verify( a.getLocal().getLocalSizes()[2] == a.getSizesInMemory()[0]);

    memVec[0] = 1;
    memVec[1] = 2;
    memVec[2] = 3;

    for ( ptrdiff_t i = 0; i < 3; ++i) {
        auto map = a.getSpatialLocationFromMemoryIndex(memVec[i], i);
        posVec[map.atIndex] = map.withValue;
    }

    say << "memVec " << memVec << " -> posVec " << posVec << "\n";

    /* mem pos 2 at mem dim 1 -> global dim 0 -> global pos 2 + 7 = 9 -> 9 - 12 = -3 */
    tdd.verify(posVec[0] == -3);

    /* mem pos 3 at mem dim 2 -> global dim 1 -> global pos 3 + 8 = 11 -> 11 - 16 = -5 */
    tdd.verify(posVec[1] == -5);

    /* mem pos 1 at mem dim 0 -> global dim 2 -> global pos 1 + 9 = 10 -> 10 - 18 = -8 */
    tdd.verify(posVec[2] == -8);


    for ( ptrdiff_t i = 0; i < 3; ++i) {
        auto map = a.getMemoryIndexFromSpatialLocation(posVec[i], i);
        memVec2[map.atIndex] = map.withValue;
    }
    say << "posVec " << posVec << " -> memVec2 " << memVec2 << "\n";
    say << "memVec " << memVec << " -> memVec2 " << memVec2 << "\n";

    tdd.verify(memVec[0] == memVec2[0]);
    tdd.verify(memVec[1] == memVec2[1]);
    tdd.verify(memVec[2] == memVec2[2]);




}

#endif
