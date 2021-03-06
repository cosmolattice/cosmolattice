#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCAL_TEST_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCAL_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::LayoutStructLocal::Test(TempLat::TDDAssertion& tdd) {


    /* test the operator== */
    LayoutStructLocal a({0,0,0});
    LayoutStructLocal b({0,0,0});
    LayoutStructLocal c({0,0});
    LayoutStructLocal d({0,0,0});
    
    d.getLocalSizes()[1] = 2;

    tdd.verify( ! (a == c) );
    tdd.verify( (a == b) );
    tdd.verify( ! (a == d) );


    a = LayoutStructLocal( {16,16,16});

    std::vector<ptrdiff_t> newLocalStarts {{ 7, 8, 9 }};
    a.setLocalStarts(newLocalStarts);

    std::vector<ptrdiff_t> memVec(3), posVec(3), memVec2(3);


    memVec[0] = 1;
    memVec[1] = 2;
    memVec[2] = 6;

    for ( ptrdiff_t i = 0; i < 3; ++i) posVec[i] = a.memoryIndexToSpatialCoordinate(memVec[i], i);

    say << "memVec " << memVec << " -> posVec " << posVec << "\n";
    
    tdd.verify(posVec[0] == 8);

    tdd.verify(posVec[1] == -6);

    tdd.verify(posVec[2] == -1);

    for ( ptrdiff_t i = 0; i < 3; ++i) memVec2[i] = a.spatialCoordinateToMemoryIndex(posVec[i], i);

    say << "posVec " << posVec << " -> memVec2 " << memVec2 << "\n";
    say << "memVec " << memVec << " -> memVec2 " << memVec2 << "\n";

    tdd.verify(memVec[0] == memVec2[0]);
    tdd.verify(memVec[1] == memVec2[1]);
    tdd.verify(memVec[2] == memVec2[2]);


    a = LayoutStructLocal( {12,16,18});

    newLocalStarts =std::vector<ptrdiff_t>  {{ 7, 8, 9 }};
    a.setLocalStarts(newLocalStarts);

    memVec[0] = 1;
    memVec[1] = 2;
    memVec[2] = 6;

    for ( ptrdiff_t i = 0; i < 3; ++i) posVec[i] = a.memoryIndexToSpatialCoordinate(memVec[i], i);

    say << "memVec " << memVec << " -> posVec " << posVec << "\n";

    tdd.verify(posVec[0] == -4);

    tdd.verify(posVec[1] == -6);

    tdd.verify(posVec[2] == -3);

    for ( ptrdiff_t i = 0; i < 3; ++i) memVec2[i] = a.spatialCoordinateToMemoryIndex(posVec[i], i);

    say << "posVec " << posVec << " -> memVec2 " << memVec2 << "\n";
    say << "memVec " << memVec << " -> memVec2 " << memVec2 << "\n";

    tdd.verify(memVec[0] == memVec2[0]);
    tdd.verify(memVec[1] == memVec2[1]);
    tdd.verify(memVec[2] == memVec2[2]);



}

#endif
