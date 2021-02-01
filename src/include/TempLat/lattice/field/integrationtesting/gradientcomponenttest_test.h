#ifndef TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_GRADIENTCOMPONENTTEST_TEST_H
#define TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_GRADIENTCOMPONENTTEST_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/measuringtools/averager.h"
#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/coordinates/coordinatevector.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticeforwardgradient.h"


inline void TempLat::GradientComponentTest::Test(TempLat::TDDAssertion& tdd) {

    ptrdiff_t nDim = 4, nGrid = 16, nGhost = 1;
    bool test;

    auto toolBox = MemoryToolBox::makeShared(nDim, nGrid, nGhost);


    //        toolBox->setVerbose();

    Field<double> phi("phi", toolBox);

  //  SingleCoordinate sc(1, SpaceStateInterface::SpaceType::Configuration);
    SpatialCoordinate x(toolBox);

    toolBox->setVerbose();


    phi = getVectorComponent(x,1);



    auto gradPhi = LatForwardGrad<4>(phi);

    Field<double> checkPhi("checkPhi", toolBox);
    checkPhi = gradPhi.norm();

    say << "phi: " << phi(test, 0, 0, 0, 0) << ", " << phi(test, 0, -1, 0, 0) << ", " << phi(test, 8, -1, 0, 0) << ", " << phi(test, -7, -1, 0, 0) << "\n";

      say << average(abs(gradPhi[0])) ;

    tdd.verify(  AlmostEqual(average(abs(gradPhi[0])), 0) );

//    say << "average(gradPhi[1]).compute(): " << average(abs(gradPhi[1])).compute() << "\n";

    /* need to take absolute value, because phi is set to truly periodic coordinate values,
       which includes jumps from N/2 to -N/2. This means that the average of the plain gradient is exactly zero. The average of the absolute value must be larger than 1, smaller than 2. */
    auto aresult = average(abs(gradPhi[1]));
    say << "Would be between 1 and 2: " << aresult << "\n";
    tdd.verify( aresult > 1 && aresult < 2 );

    tdd.verify( AlmostEqual( average(gradPhi[1]), 0) );

}

#endif
