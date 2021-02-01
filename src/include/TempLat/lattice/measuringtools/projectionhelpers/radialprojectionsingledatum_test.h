#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONSINGLEDATUM_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONSINGLEDATUM_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/almostequal.h"

template <typename T>
inline void TempLat::RadialProjectionSingleDatum<T>::Test(TempLat::TDDAssertion& tdd) {

    RadialProjectionSingleDatum<T> a(2, 3, 0, 4, 1.25);
    RadialProjectionSingleDatum<T> b(1, 1, 0.5, 5, 2.25);
    
    auto c = combine(a, b);
    
    tdd.verify(AlmostEqual(c.multiplicity, 3.5));

    tdd.verify(AlmostEqual(c.average, (a.average * 1.25 + b.average * 2.25) / 3.5));

    tdd.verify(AlmostEqual(c.sampleVariance, (3 + 1) / 3.5 - c.average * c.average));
    
    tdd.verify(c.minVal == 0);
    
    tdd.verify(c.maxVal == 5);
    
    /* test the rescaling */
    c *= 3;

    /* multiplicity does not change */
    tdd.verify(AlmostEqual(c.multiplicity, 3.5));

    tdd.verify(AlmostEqual(c.average, 3 * (a.average * 1.25 + b.average * 2.25) / 3.5));

    tdd.verify(AlmostEqual(c.sampleVariance, 9 * (3 + 1) / 3.5 - c.average * c.average));
    
    tdd.verify(c.minVal == 0);
    
    tdd.verify(c.maxVal == 15);

    say << "See TempLat/lattice/measuringtoolsIO/projectionhelpers/radialprojectionresult_test.h\n";

}

#endif
