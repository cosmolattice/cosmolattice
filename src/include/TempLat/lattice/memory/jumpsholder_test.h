#ifndef TEMPLAT_LATTICE_MEMORY_JUMPSHOLDER_TEST_H
#define TEMPLAT_LATTICE_MEMORY_JUMPSHOLDER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::JumpsHolder::Test(TempLat::TDDAssertion& tdd) {
    
    /* Perhaps a bit elaborate, but it is as consistent as it gets.. */
    /* And in fact it helped getting out the errors. Yes. */
    
    /* single datum on a grid: 24 bytes making up x, y, and z. */
    struct datum {
        ptrdiff_t x, y, z;
    };
    
    /* arbitrary irregular sizing */
    std::vector<ptrdiff_t> nGrid {{ 62, 22, 24 }};
    
    LayoutStruct layout( nGrid);
    
    layout.setLocalSizes(nGrid);
    
    /* arbitrary irregular ghosting */
    std::vector<std::array<ptrdiff_t, 2u>> nGhost(3);
    nGhost[0][0] = 6;
    nGhost[0][1] = 5;
    nGhost[1][0] = 4;
    nGhost[1][1] = 3;
    nGhost[2][0] = 2;
    nGhost[2][1] = 1;
    
    std::vector<datum> memory(
                              (nGrid[0] + nGhost[0][0] + nGhost[0][1])
                              * (nGrid[1] + nGhost[1][0] + nGhost[1][1])
                              * (nGrid[2] + nGhost[2][0] + nGhost[2][1])
    );

    /* setup the controlled known memory; each entry equals its position */
    for ( ptrdiff_t i = -nGhost[0][0]; i < nGrid[0] + nGhost[0][1]; ++i ) {
        ptrdiff_t iPos = (i + nGhost[0][0]) * (nGrid[1] + nGhost[1][0] + nGhost[1][1]) * (nGrid[2] + nGhost[2][0] + nGhost[2][1]);
        for ( ptrdiff_t j = -nGhost[1][0]; j < nGrid[1] + nGhost[1][1]; ++j ) {
            ptrdiff_t jPos = (j + nGhost[1][0]) * (nGrid[2] + nGhost[2][0] + nGhost[2][1]);
            for ( ptrdiff_t k = -nGhost[2][0]; k < nGrid[2] + nGhost[2][1]; ++k ) {
                ptrdiff_t kPos = k + nGhost[2][0];
                ptrdiff_t pos = iPos + jPos + kPos;
                // std::cerr << "Hoi " << pos << " " << i << " " << j << " " << k << "\n";
                memory[pos].x = i;
                memory[pos].y = j;
                memory[pos].z = k;
            }
        }
    }
    
    /* verify the setup, not controlled, but using the jumps which are under scrutiny */
    JumpsHolder jumper(layout, nGhost);
    bool allRight = true;
    std::vector<ptrdiff_t> shifts(3);
    for ( ptrdiff_t i = 0; i < nGrid[0] ; ++i ) {
        shifts[0] = i;
        for ( ptrdiff_t j = 0; j < nGrid[1] ; ++j ) {
            shifts[1] = j;
            for ( ptrdiff_t k = 0; k < nGrid[2] ; ++k ) {
                shifts[2] = k;
                /* here we employ the jumps. */
                ptrdiff_t pos = jumper.toOrigin() + jumper.getJump(shifts);
                const datum& dat = memory[pos];
                /* check that jumps bring us indeed to x, y, z = i, j, k. */
                allRight = allRight && dat.x == i && dat.y == j && dat.z == k;
//                std::cerr << i << ", " << j << ", " << k << " => " << dat.x << ", " << dat.y << ", " << dat.z << "\n";
            }
        }
    }

    
    
    
    tdd.verify( allRight );
    
    tdd.verify( jumper == JumpsHolder(layout, nGhost) );

    nGrid[0] *= 2;
    LayoutStruct layout2(nGrid);
    
    layout.setLocalSizes(nGrid);

    tdd.verify( jumper != JumpsHolder(layout2, nGhost) );

}

#endif
