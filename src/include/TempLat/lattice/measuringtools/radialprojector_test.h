#ifndef TEMPLAT_LATTICE_MEASUREMENTS_RADIALPROJECTOR_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_RADIALPROJECTOR_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

//#include "TempLat/lattice/algebra/coordinates/wavenumber.h"
//#include "TempLat/lattice/field/field.h"

inline void TempLat::RadialProjectorTester::Test(TempLat::TDDAssertion& tdd) {

  /*  const ptrdiff_t nDim = 3;
    const ptrdiff_t nGrid = 16;
    const ptrdiff_t nGhost = 1;

    auto mToolBox = MemoryToolBox::makeShared(nDim, nGrid, nGhost );

    Field<double> phi("phi", mToolBox);
    auto phi_of_k = phi.inFourierSpace();
    phi_of_k = 1 * WaveNumber().getNorm();

    auto rProj = projectRadially(3 * abs(phi_of_k), SpaceStateInterface::SpaceType::Fourier);

    auto result = rProj.measure();

//    say << "result: \n" << result << "\n";

    ///not sure this really tests the radial projection, but at least it tests MPI.
    bool allRight = true;
    for ( auto&& it : result) {
        allRight = allRight && AlmostEqual(3 * it.getBin().average, it.getValue().average);
        // variance is close to zero, all numerical noise.
        // allRight = allRight && AlmostEqual(3 * it.getBin().sampleVariance, it.getValue().sampleVariance, 1.e-2);
        allRight = allRight && AlmostEqual(3 * it.getBin().minVal, it.getValue().minVal);
        allRight = allRight && AlmostEqual(3 * it.getBin().maxVal, it.getValue().maxVal);
        if ( ! allRight ) {
            say << "Broken: " << it << "\n";
            break;
        }
    }

    tdd.verify( allRight );

    // test the rebinning
    result.rebin(10);
    tdd.verify( result.size() == 10u );

    allRight = true;
    for ( auto&& it : result) {
        allRight = allRight && AlmostEqual(3 * it.getBin().average, it.getValue().average);
        // variance is close to zero, all numerical noise.
        // allRight = allRight && AlmostEqual(3 * it.getBin().sampleVariance, it.getValue().sampleVariance, 1.e-2);
        if ( it.getBin().average != 0 ) {
            allRight = allRight && AlmostEqual(3 * it.getBin().minVal, it.getValue().minVal);
            allRight = allRight && AlmostEqual(3 * it.getBin().maxVal, it.getValue().maxVal);
        }
        if ( ! allRight ) {
            say << "Broken: " << it << "\n";
            break;
        }
    }
    tdd.verify( allRight );

    // test the rescaling

    result.rescale([](auto x) { return x * x; });
    allRight = true;
    for ( auto&& it : result) {
        // variance is close to zero, all numerical noise.
        // allRight = allRight && AlmostEqual(3 * it.getBin().sampleVariance, it.getValue().sampleVariance, 1.e-2);
        if ( it.getBin().average != 0 ) {
            allRight = allRight && AlmostEqual(3 * it.getBin().average, it.getValue().average / it.getBin().average / it.getBin().average);
            allRight = allRight && AlmostEqual(3 * it.getBin().minVal, it.getValue().minVal/ it.getBin().average / it.getBin().average);
            allRight = allRight && AlmostEqual(3 * it.getBin().maxVal, it.getValue().maxVal/ it.getBin().average / it.getBin().average);
        }
        if ( ! allRight ) {
            say << "Broken: " << it << "\n";
            break;
        }
    }
    tdd.verify( allRight );
*/
}

#endif
