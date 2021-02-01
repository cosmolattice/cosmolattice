#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONRESULT_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONRESULT_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


template <typename T>
inline void TempLat::RadialProjectionResult<T>::Test(TempLat::TDDAssertion& tdd) {

    RadialProjectionResult one(10), two(12), three(10);
    
    tdd.verify( Throws<RadialProjectionResultSizeException>([&]() { one += two; }));
 
    /* dummy data */
    for (ptrdiff_t i = 0, iEnd = three.size(); i < iEnd; ++i ) {
        three.add( i, 2 * i, 2 * i );
    }
    
    one += three;
    one += three;
    
    one.finalize(MPICommReference());

    bool allRight = true;
    for (ptrdiff_t i = 0, iEnd = three.size(); i < iEnd; ++i ) {
        allRight = allRight && one[i].getValue().average == 2 * three.mValues.mAverages[i];
        allRight = allRight && one[i].getValue().sampleVariance == 2 * three.mValues.mVariances[i];
        allRight = allRight && one[i].getValue().multiplicity == 2 * three.mMultiplicities[i];
    }
    
    tdd.verify(allRight);

    /* test that this compiles */
    for ( auto&& it : one ) {
        it.getValue().average *= 1;
    }

}

#endif
