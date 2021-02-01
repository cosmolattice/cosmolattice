#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_TRANSPOSITIONMAP_TEST_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_TRANSPOSITIONMAP_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::TranspositionMap::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( Throws<TranspositionMapOutOfBounds>([](){
        TranspositionMap tmap(3);
        tmap.setMap({{1, 2, 3, 5}});
    }) );

    ptrdiff_t nd = 5;

    TranspositionMap map(nd);
    
    bool allRight = true;
    for ( ptrdiff_t i = 0; i < nd; ++i ) {
        allRight = allRight && map.getForward(i) == i && map.getInverse(i) == i;
    }
    tdd.verify(allRight);
    tdd.verify( ! map.isTransposed() );

    std::vector<ptrdiff_t> input(nd);
    for ( ptrdiff_t i = 0; i < nd; ++i ) {
        input[i] = nd - i - 1;
    }
    map.setMap(input);
    
    for ( ptrdiff_t i = 0; i < nd; ++i ) {
        allRight = allRight && map.getForward(map.getInverse(i)) == i ;
    }
    tdd.verify(allRight);
    tdd.verify( map.isTransposed() );

    auto map2 = map;
    tdd.verify(map2 == map);

}

#endif
